/* ----------------------------------------------------------------------------
 * File: buff_async_write.c
 *
 * Desc: Stream Buffer implementation for file output. 
 *       Buffers data and proccesses file access asynchronously.
 *       Buffers seek/write requests until the bufstream is closed (implements same functionality as buf_seek).
 *
 *       Use where multiple files are to written in parallel to avoid performance degradation because of 
 *       concurent disk access, e.g P2 mxf multiplexing or multiple encoding sessions
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 * This software is protected by copyright law and international treaties.  Unauthorized 
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

// NOTE: even though the extension is .c this file should be compiled with a C++ compiler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__QNX__)
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <list>


#define MODNAME ASYC_WRITE
#include "srdbg.h"


static int inst_count = 0;

#include "bufstrm.h" 

#include "auxinfo.h"
#include "buff_async_write.h"

#include "mcthreads.h"

#if !defined(__APPLE__) && !defined(__linux__)
	#define bseek_file_close _close
	#define bseek_file_read _read 
	#define bseek_file_seek _lseeki64
	#define bseek_file_write _write
#else
	#ifdef __linux__
		#define bseek_file_seek lseek64
	#else
		#define bseek_file_seek lseek
	#endif

	#define bseek_file_read  read 
	#define bseek_file_close close
	#define bseek_file_write write
#endif

struct seek_info_t
{
  int64_t      seek_pos;
  uint8_t     *bfr;
  uint32_t     bfr_size;
  seek_info_t *next;
};

#define BS_SMALL_FILE_SIZE 1024*5
#define atom_ftype 0
#define atom_moov  1
#define atom_mdat  2

struct atom_info_t {
	int64_t atom_pos;
	uint64_t atom_size;
	int32_t  atom_type;
};


struct output_data {
  uint32_t data_size;
  uint8_t *data_buf;
};


typedef std::list <output_data*> CBuffersList;

//implementation structure
struct impl_stream
{
  FILE *io;


  output_data* actual_buffer;

  uint32_t bfr_size;    // allocated size
  uint32_t bfr_count;   // filled size 
  uint32_t max_buffers; // maximum number of buffers to hold in memory
  uint32_t num_buffers; // number of allocated buffers
  uint32_t chunk_size;  
  uint64_t bytecount;   // bytes received so far
  uint64_t file_size;

  int32_t write_error;

  seek_info_t *seek_info;
  seek_info_t *last_seek_info;
  atom_info_t *atom_info;

#ifdef _BS_UNICODE
  wchar_t filename[_BS_MAX_PATH];
#else
  char filename[_BS_MAX_PATH];
#endif

  CBuffersList *pFreeBuffersList;
  CBuffersList *pFullBuffersList;

  MCCriticalSection csFreeListLock;
  MCCriticalSection csFullListLock;
  MCCriticalSection csInputLock;

  MCSemaphore hSem;
  MCThread hThread;
  uint32_t terminate;

  uint32_t inst_no;
};

static uint32_t  fw_usable_bytes(bufstream_tt *bs)
{
  return 0;
}

//request the buffer with at least numbytes-bytes
static uint8_t *fw_request(bufstream_tt *bs, uint32_t numbytes)
{
  return NULL;
}


//confirm numbytes-bytes filled in in requested after last "request"-call
static uint32_t  fw_confirm(bufstream_tt *bs, uint32_t numbytes)
{
  return 0;
}


static uint32_t  fw_copybytes(bufstream_tt *bs, uint8_t *ptr, uint32_t numbytes)
{
  struct impl_stream* p = bs->Buf_IO_struct;

  if(p->write_error || !ptr || numbytes == 0)
    return 0;

  uint32_t data_left = numbytes;

  MCCriticalSectionEnter(&p->csInputLock); 

  // collect data in buffer
  while(data_left)
  {
    if(!p->actual_buffer)
    {
      // check free list size
      MCCriticalSectionEnter(&p->csFreeListLock);
      size_t list_size = p->pFreeBuffersList->size();
      MCCriticalSectionLeave(&p->csFreeListLock);
 
      if(list_size == 0)
      {
        // dynamically alloc one new buffer till max
        if(p->num_buffers < p->max_buffers)
        {
          output_data *pData = (output_data*)malloc(sizeof(output_data));
          if(!pData)
          {
            p->write_error = 1;
            return 0;
          }
          pData->data_size = 0;
           
          pData->data_buf = (uint8_t*)malloc(p->chunk_size); 
          if(!pData->data_buf)
          {
            p->write_error = 1;
            return 0;
          }
          MCCriticalSectionEnter(&p->csFreeListLock);
          p->pFreeBuffersList->push_back(pData);
          MCCriticalSectionLeave(&p->csFreeListLock);
          p->num_buffers++;
          MPF(DBG_TRC1,("copybytes - inst %d - num buffers allocated: %d", p->inst_no, p->num_buffers));
        }
        else // reached max buffers
        {
          // block until there are free buffers again
          MPF(DBG_TRC1,("copybytes - inst %d - wait for free buffers, actual %d in list", p->inst_no, list_size));

          MCEvent event;
          MCEventCreate(&event, 0, 0);

          while( list_size == 0)
          {
            //MCSleep(50);
            MCEventWait(&event, 50);
       
            MCCriticalSectionEnter(&p->csFreeListLock);
            list_size = p->pFreeBuffersList->size();
            MCCriticalSectionLeave(&p->csFreeListLock);
            if(p->write_error)
            {
              MCEventDestroy(&event);
              return 0;
            }
          }
          MCEventDestroy(&event);
          MPF(DBG_TRC1,("copybytes - inst %d - wait for free buffers done, actual %d in list", p->inst_no, list_size));
        }
      } 

      // get one free buffer
      MCCriticalSectionEnter(&p->csFreeListLock);
      CBuffersList::iterator it = p->pFreeBuffersList->begin();
      if(it != p->pFreeBuffersList->end())
      {
        p->actual_buffer = (output_data*)(*it);
        p->pFreeBuffersList->pop_front();
      }    
      MCCriticalSectionLeave(&p->csFreeListLock);  
      if(!p->actual_buffer)
      {
        MPF(DBG_TRC1,("copybytes - inst %d - failed to get free buffer", p->inst_no));
        p->write_error = 1;
        return 0;
      }  
    }    

    uint32_t to_copy = p->chunk_size - p->bfr_count;
    if (data_left < to_copy)
      to_copy = data_left;
    memcpy(p->actual_buffer->data_buf + p->bfr_count, ptr + numbytes - data_left, to_copy);
    p->bfr_count += to_copy;
    p->bytecount += to_copy;
    data_left -= to_copy;

    if(p->bfr_count == p->chunk_size) // deliver full buffer to the thread;
    {
      p->actual_buffer->data_size = p->bfr_count;
      //MPF(DBG_TRC1,("copybytes - inst %d - put into output list data %d", p->inst_no, p->data_buffer->data_size));
      MCCriticalSectionEnter(&p->csFullListLock);
      p->pFullBuffersList->push_back(p->actual_buffer);
      //MPF(DBG_TRC1,("copybytes - inst %d - buffers in output list %d", p->inst_no, p->pFullBuffersList->size()));
      MCCriticalSectionLeave(&p->csFullListLock);
      p->actual_buffer = NULL;

      MCSemaphoreRelease(&bs->Buf_IO_struct->hSem, NULL);
      p->bfr_count = 0;
    }
  }

  MCCriticalSectionLeave(&p->csInputLock);
  return numbytes;
}



// maximum chunk-size in buffer-mode (i.e. for "request"-call)
static uint32_t fw_async_chunksize(bufstream_tt *bs)
{
  return bs->Buf_IO_struct->chunk_size;
}

static int32_t flush_buf(bufstream_tt *bs)
{

  struct impl_stream* p = bs->Buf_IO_struct;

  MCCriticalSectionEnter(&p->csInputLock);

  // send actual buffer

  if(p->actual_buffer)
  {
    p->actual_buffer->data_size = p->bfr_count;
    MCCriticalSectionEnter(&p->csFullListLock);
    p->pFullBuffersList->push_back(p->actual_buffer);
    MCCriticalSectionLeave(&p->csFullListLock);
    p->actual_buffer = NULL;
    p->bfr_count = 0;
  }

  MCCriticalSectionLeave(&p->csInputLock);

  MCSemaphoreRelease(&bs->Buf_IO_struct->hSem, NULL);

  return BS_OK;
}


//inform bufstream about some additional info - for example
// to inform MUXer about encoding-units
static uint32_t fw_async_auxinfo(bufstream_tt *bs, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size)
{
  struct impl_stream* p = bs->Buf_IO_struct;
  buf_seek_info *bf_seek_info;

  switch(info_ID)
  {  
    case BYTECOUNT_INFO:
      {
        uint64_t *ptr = (uint64_t*)info_ptr;
        if(ptr && (info_size == sizeof(uint64_t)))
          *ptr = bs->Buf_IO_struct->bytecount;
      }
      break;

    case FLUSH_BUFFER:
      flush_buf(bs);
      break;
    
    case BUF_SEEK_INFO:
      bf_seek_info = (buf_seek_info*)info_ptr;
      if (info_size == sizeof(buf_seek_info))
      {
        seek_info_t *new_seek_info = (seek_info_t*)malloc(sizeof(seek_info_t));
        if (new_seek_info)
        {
          new_seek_info->bfr = (uint8_t*)malloc(bf_seek_info->bfr_size);
		      if (new_seek_info->bfr)
		      {
		        memcpy(new_seek_info->bfr, bf_seek_info->bfr, bf_seek_info->bfr_size);
		        new_seek_info->bfr_size = bf_seek_info->bfr_size;
		        new_seek_info->seek_pos = bf_seek_info->seek_pos;
		        new_seek_info->next = NULL;
		        if (p->last_seek_info)
		          p->last_seek_info->next = new_seek_info;
		        else
		          p->seek_info = new_seek_info;
		        p->last_seek_info = new_seek_info;
		      }
		      else
		        free(new_seek_info);
	      }
	    }
	    break;

    case BUF_SEEK_REORDER:
      if(info_ptr)
      {
		    p->atom_info = (atom_info_t*)malloc(info_size);
		    memset(p->atom_info, 0, info_size);
		    memcpy(p->atom_info, info_ptr, info_size);
      }
      break;

  }
  return BS_OK;
}


uint8_t fw_async_reorder(bufstream_tt *bs, atom_info_t *info_ptr, uint32_t info_size)
{
	int32_t fp;
	atom_info_t *header_ptr, *media_data_ptr, *file_header;
	uint64_t file_size;

	if(!bs || !info_ptr)
		return 1;

	file_header = info_ptr;
	header_ptr = info_ptr+1;
	media_data_ptr = info_ptr+2;

	if(!header_ptr || !media_data_ptr || !file_header)
		return 1;

	#if !defined(__APPLE__) && !defined(__linux__)
		#ifdef _BS_UNICODE
			if ((fp = _wopen(bs->Buf_IO_struct->filename, _O_RDWR | _O_BINARY)) == -1)
		#else
			if ((fp = _open(bs->Buf_IO_struct->filename, _O_RDWR | _O_BINARY)) == -1)
		#endif
	#else
			if ((fp = open(bs->Buf_IO_struct->filename, O_RDWR, 0777 )) == -1)
	#endif
			return 1;

	file_size = file_header->atom_size + media_data_ptr->atom_size + header_ptr->atom_size;


	if(file_size <= BS_SMALL_FILE_SIZE){
		
		if( bseek_file_seek(fp, media_data_ptr->atom_pos, SEEK_SET) == -1)
			return 1;
		uint8_t *media_data		= (uint8_t *)malloc((size_t)media_data_ptr->atom_size);
		if(bseek_file_read(fp, media_data, (uint32_t)media_data_ptr->atom_size))
			return 1;

		if(bseek_file_write(fp, media_data, (uint32_t)media_data_ptr->atom_size))
			return 1;

		free(media_data);
	}else{
		
		uint8_t *read_bfr, *header_buf, res;
		uint32_t bfr_size = 1048576, read_bfr_size = bfr_size, proc_size;

		read_bfr = (uint8_t*)malloc(bfr_size);
		memset(read_bfr, 0, bfr_size);

		//rewrite headers		
		uint64_t cur_read_pos = header_ptr->atom_pos - bfr_size, cur_write_pos = file_size - bfr_size;

		//read headers
		header_buf = (uint8_t*)malloc((size_t)header_ptr->atom_size);
		bseek_file_seek(fp, (uint32_t)header_ptr->atom_pos, SEEK_SET);
		bseek_file_read(fp, header_buf, (uint32_t)header_ptr->atom_size);

		while(true){

			if( bseek_file_seek(fp, cur_read_pos, SEEK_SET) == -1)
				return 1;
			proc_size = bseek_file_read(fp, read_bfr, read_bfr_size);

			if( bseek_file_seek(fp, cur_write_pos, SEEK_SET) == -1) 
				return 1;

			proc_size = bseek_file_write(fp, read_bfr, read_bfr_size);
			if(cur_read_pos<read_bfr_size)
				break;

			cur_read_pos  -= read_bfr_size;
			cur_write_pos -= read_bfr_size;
		}

		uint32_t nTail = (uint32_t)(cur_read_pos - media_data_ptr->atom_pos);
		cur_read_pos = cur_read_pos - nTail;
		memset(read_bfr, 0, bfr_size);
		if(bseek_file_seek(fp, cur_read_pos, SEEK_SET) == -1) return 1;
		res = bseek_file_read(fp, read_bfr, nTail);

		cur_write_pos = cur_write_pos - nTail;
		if(bseek_file_seek(fp, cur_write_pos, SEEK_SET) == -1) return 1;
		res = bseek_file_write(fp, read_bfr, nTail);

		if(bseek_file_seek(fp, file_header->atom_size, SEEK_SET) == -1) return 1;
		res = bseek_file_write(fp, header_buf, (uint32_t)header_ptr->atom_size);

		free(read_bfr);
		free(header_buf);
	}

	bseek_file_close(fp);

	return 0;
}



static int fw_async_write_info(bufstream_tt *bs, seek_info_t *seek_info)
{
  int32_t i, fp;
  seek_info_t *sinfo;

#if !defined(__APPLE__) && !defined(__linux__)
 #ifdef _BS_UNICODE
  if ((fp = _wopen(bs->Buf_IO_struct->filename, _O_RDWR | _O_BINARY)) == -1)
 #else
  if ((fp = _open(bs->Buf_IO_struct->filename, _O_RDWR | _O_BINARY)) == -1)
 #endif
#else
    if ((fp = open(bs->Buf_IO_struct->filename, O_RDWR, 0777 )) == -1)
#endif
    return 1;

  sinfo = seek_info;

  while (sinfo)
  {
	// seek to the spot we need to modify
#if !defined(__APPLE__) && !defined(__linux__)
	if (_lseeki64(fp, sinfo->seek_pos, SEEK_SET) == -1L)
#else
#ifdef __linux__
    if (lseek64(fp, sinfo->seek_pos, SEEK_SET) == -1L)
#else
	if (lseek(fp, sinfo->seek_pos, SEEK_SET) == -1L)
#endif
#endif
	{
#if !defined(__APPLE__) && !defined(__linux__)
      _close(fp);
#else
      close(fp);
#endif
      return 1;
	}

	// write out the data
#if !defined(__APPLE__) && !defined(__linux__)
	i = _write(fp, (void*)&sinfo->bfr[0], sinfo->bfr_size);

#ifdef _DEBUG
	int ix = _commit(fp);
#endif

#else
    i = write(fp,(void*)&sinfo->bfr[0], sinfo->bfr_size);
#endif
    if (i != sinfo->bfr_size)
	{
#if !defined(__APPLE__) && !defined(__linux__)
      _close(fp);
#else
      close(fp);
#endif
      return 1;
	}

	sinfo = sinfo->next;
  }

#if !defined(__APPLE__) && !defined(__linux__)
  _close(fp);
#else
  close(fp);
#endif

  return 0;
}


static   void  fw_async_done  (bufstream_tt *bs, int32_t Abort)
{
  struct impl_stream* p = bs->Buf_IO_struct;

  seek_info_t *sinfo = 0;

  MPF(DBG_TRC1,("fw_async_done"));

  // terminate the thread
  p->terminate = 1;
 
  flush_buf(bs);
  MCSemaphoreRelease(&bs->Buf_IO_struct->hSem, NULL);

  MCThreadWait(&p->hThread, -1);
  MCThreadDestroy(&p->hThread);

  MCSemaphoreDestroy(&p->hSem);

  fclose(p->io);

  MCCriticalSectionDestroy(&p->csInputLock);
  MCCriticalSectionDestroy(&p->csFullListLock);
  MCCriticalSectionDestroy(&p->csFreeListLock);

  // free the output list and anything thats left over (shouldn't)
  if(p->pFullBuffersList)
  {
    MPF(DBG_TRC1,("fw_async_done - inst %d - buffers left in full list %d !!", p->inst_no, p->pFullBuffersList->size()));
    CBuffersList::iterator it = p->pFullBuffersList->begin();
    while(it != p->pFullBuffersList->end())
    {
      output_data *pData= (output_data*)(*it);
      if(pData)
      { 
        if(pData->data_buf)
          free(pData->data_buf);
        free(pData);
      }
      p->pFullBuffersList->pop_front();
      it = p->pFullBuffersList->begin();
    }
    delete(p->pFullBuffersList);
  }

  if(p->pFreeBuffersList)
  {
    MPF(DBG_TRC1,("fw_async_done - inst %d - buffers left in free list %d!!", p->inst_no, p->pFreeBuffersList->size()));
    CBuffersList::iterator it = p->pFreeBuffersList->begin();
    while(it != p->pFreeBuffersList->end())
    {
      output_data *pData= (output_data*)(*it);
      if(pData)
      { 
        if(pData->data_buf)
          free(pData->data_buf);
        free(pData);
      }
      p->pFreeBuffersList->pop_front();
      it = p->pFreeBuffersList->begin();
    }
    delete(p->pFreeBuffersList);
  }

  if(p->actual_buffer)
  {
    if(p->actual_buffer->data_buf)
      free(p->actual_buffer->data_buf);
    free(p->actual_buffer);
  }

  if (p->seek_info)
  {
    //if (!Abort)
      fw_async_write_info(bs, p->seek_info);
    sinfo = p->seek_info;
	  while (sinfo)
	  {
        seek_info_t *next_sinfo = sinfo->next;
	    if (sinfo->bfr)
	      free(sinfo->bfr);
	    free(sinfo);
	    sinfo = next_sinfo;
	  }
  }

  if(p->atom_info)
  {
	  fw_async_reorder(bs, p->atom_info, sizeof(atom_info_t)*3);
	  free(p->atom_info);
  }


  free(p);
  bs->Buf_IO_struct = NULL;
}


static void  fw_async_free  (bufstream_tt *bs)
{
  if(bs->Buf_IO_struct)
    bs->done(bs,0);

  free(bs);
}

// thread func to write data to disk
MCThreadFunc(thread_proc, lpParam)
{
  bufstream_tt *bs = (bufstream_tt*)lpParam;
  if(!bs)
    return 0;

  struct impl_stream* p = bs->Buf_IO_struct;
  if(!p)
    return 0;
  
  while (true)
  {     
    MCSemaphoreWait(&p->hSem, MCTHREAD_WAIT_INFINITE);

    // get available buffers and write to disk
    if(p->pFullBuffersList)
    {
      output_data *pData = NULL;
  
      MCCriticalSectionEnter(&p->csFullListLock);
      CBuffersList::iterator it = p->pFullBuffersList->begin();
      if(it != p->pFullBuffersList->end())
      {
        pData= (output_data*)(*it);
        p->pFullBuffersList->pop_front();
      }
      MCCriticalSectionLeave(&p->csFullListLock);

      while(pData)
      {
        if(pData->data_buf)
        {
          uint32_t n = (uint32_t)fwrite(pData->data_buf, sizeof(uint8_t), pData->data_size, p->io);
          // put the buffer back to the free list
          MCCriticalSectionEnter(&p->csFreeListLock);
          p->pFreeBuffersList->push_back(pData);
          MCCriticalSectionLeave(&p->csFreeListLock);

          p->file_size += n;
          if(n != pData->data_size)
          {
            p->write_error = 1;
            return 0;
          }

          fflush(p->io);
        }

        pData = NULL;
    
        MCCriticalSectionEnter(&p->csFullListLock);
        it = p->pFullBuffersList->begin();
        if(it != p->pFullBuffersList->end())
        {
          pData = (output_data*)(*it);
          p->pFullBuffersList->pop_front();
        }
        MCCriticalSectionLeave(&p->csFullListLock);
      }
    }
  
    if(p->terminate)
    {
      fflush(p->io);
      break;
    }
  }

  return 0;
}

static int32_t init_async_write_common(bufstream_tt *bs, uint32_t bufsize, uint32_t max_buffers)
{
  memset(&bs->Buf_IO_struct->hSem, 0, sizeof(MCSemaphore));
	memset(&bs->Buf_IO_struct->hThread, 0, sizeof(MCThread));

  bs->Buf_IO_struct->actual_buffer        = NULL;
  bs->Buf_IO_struct->bfr_size           = bufsize;
  bs->Buf_IO_struct->bfr_count          = 0;
  bs->Buf_IO_struct->num_buffers        = 0;
  bs->Buf_IO_struct->chunk_size         = bufsize;
  bs->Buf_IO_struct->bytecount          = 0;
  bs->Buf_IO_struct->file_size          = 0;

  if(max_buffers < 2)
    bs->Buf_IO_struct->max_buffers = 2;
  else
    bs->Buf_IO_struct->max_buffers = max_buffers;

  bs->Buf_IO_struct->seek_info          = 0;
  bs->Buf_IO_struct->last_seek_info     = 0;
  bs->Buf_IO_struct->atom_info          = 0;

  bs->usable_bytes = fw_usable_bytes;
  bs->request      = fw_request;
  bs->confirm      = fw_confirm;
  bs->copybytes    = fw_copybytes;
  bs->split        = NULL;
  bs->chunksize    = fw_async_chunksize;
  bs->free         = fw_async_free;
  bs->auxinfo      = fw_async_auxinfo;
  bs->done         = fw_async_done;
  bs->drive_ptr    = NULL;
  bs->drive        = NULL;

  bs->state        = NULL;
  bs->flags        = BS_FLAGS_DST;

  bs->Buf_IO_struct->write_error = 0;
  bs->Buf_IO_struct->terminate = 0;


  bs->Buf_IO_struct->pFreeBuffersList = new CBuffersList;
  if(!bs->Buf_IO_struct->pFreeBuffersList)
  {
    bs->free(bs);
    return BS_ERROR;
  }
 
  bs->Buf_IO_struct->pFullBuffersList = new CBuffersList;
  if(!bs->Buf_IO_struct->pFullBuffersList)
  {
    bs->free(bs);
    return BS_ERROR;
  }
  MCCriticalSectionCreate(&bs->Buf_IO_struct->csInputLock);
  MCCriticalSectionCreate(&bs->Buf_IO_struct->csFullListLock);
  MCCriticalSectionCreate(&bs->Buf_IO_struct->csFreeListLock);
  
  MCSemaphoreCreate(&bs->Buf_IO_struct->hSem, 0, 0x7FFFFFFF);

  MCThreadCreate(&bs->Buf_IO_struct->hThread, thread_proc, bs, NULL);

  // preallocate 2 buffers - more will be allocated dynamicaly up to max_buffer
  for (int32_t i = 0; i < 2; i++)
  {
    output_data* pData = (output_data*)malloc(sizeof(output_data));
    if(!pData)
    {
      bs->free(bs);
      return BS_ERROR;
    }
    pData->data_buf = (uint8_t*)malloc(bs->Buf_IO_struct->bfr_size);
    if(!pData->data_buf)
    {
      free(pData);
      bs->free(bs);
      return BS_ERROR;
    }
    bs->Buf_IO_struct->pFreeBuffersList->push_back(pData);
    bs->Buf_IO_struct->num_buffers++;
  }

  inst_count++;

  bs->Buf_IO_struct->inst_no = inst_count;

  return BS_OK;
}


#ifdef _BS_UNICODE
int32_t init_file_buf_async_write(bufstream_tt *bs,
                            const wchar_t* bs_filename,
                            uint32_t bufsize,
                            uint32_t max_buffers, 
                            void (*DisplayError)(char *txt))
#else
int32_t init_file_buf_async_write(bufstream_tt *bs,
                            const char *bs_filename,
                            uint32_t bufsize,
                            uint32_t max_buffers, 
                            void (*DisplayError)(char *txt))
#endif
{
  char tmpStr[256];

  if(!(bs->Buf_IO_struct = (struct impl_stream*)malloc(sizeof(struct impl_stream))))
  {
    sprintf(tmpStr, "BUFSTRM:Unable to allocate bytes for bufstream object");
    if(DisplayError)
      DisplayError(tmpStr);
    return BS_ERROR;
  }

#ifdef _BS_UNICODE
  if(!(bs->Buf_IO_struct->io = _wfopen(bs_filename, L"wbc")))
 #else
  #if !defined(__linux__) && !defined(__QNX__)
  if(!(bs->Buf_IO_struct->io = fopen(bs_filename, "wbc")))
  #else
  if(!(bs->Buf_IO_struct->io = fopen64(bs_filename, "wbc")))
  #endif
#endif
  {
    free(bs->Buf_IO_struct);
    if(DisplayError)
    {
      sprintf(tmpStr, "BUFSTRM:Unable to open file for writing");
      DisplayError(tmpStr);
    }
    return BS_ERROR;
  }

#ifdef _BS_UNICODE
  wcscpy(bs->Buf_IO_struct->filename, bs_filename);
#else
  strcpy(bs->Buf_IO_struct->filename, bs_filename);
#endif

  return init_async_write_common(bs, bufsize, max_buffers);
}

#ifdef _BS_UNICODE
bufstream_tt *open_file_buf_async_write(const wchar_t* bs_filename,
                                  uint32_t bufsize,
                                  uint32_t max_buffers, 
                                  void (*DisplayError)(char *txt))
#else
bufstream_tt *open_file_buf_async_write(const char *bs_filename,
                                  uint32_t bufsize,
                                  uint32_t max_buffers, 
                                  void (*DisplayError)(char *txt))
#endif
{
  bufstream_tt *p;
  p=(bufstream_tt*)malloc(sizeof(bufstream_tt));
  if(p)
  {
    if(BS_OK != init_file_buf_async_write(p, bs_filename, bufsize, max_buffers, DisplayError))
    {
      free(p);
      p = NULL;
    }
  }
  return p;
}


#if !defined(__APPLE__) && !defined(__linux__) && !defined(__QNX__)
void close_file_buf_async_write(bufstream_tt* &bs, int32_t Abort)
#else
void close_file_buf_async_write(bufstream_tt* bs, int32_t Abort)
#endif
{
  bs->done(bs, Abort);
  bs->free(bs);
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__QNX__)
  bs = NULL;
#endif
}
