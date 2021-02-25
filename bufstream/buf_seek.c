/* ----------------------------------------------------------------------------
 * File: buf_seek.c
 *
 * Desc: Buffers seek/write requests until the bufstream is closed
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 * This software is protected by copyright law and international treaties.  Unauthorized 
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buf_seek.h"
#include "auxinfo.h"

#ifdef WIN32IO
  #include <io.h>
  #include <fcntl.h>
#else
  #if !defined(__APPLE__) && !defined(__linux__)
    #include <io.h>
  #else
    #include <unistd.h>
  #endif
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/types.h>
#endif


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
  struct seek_info_t *next;
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


//implementation structure
struct impl_stream
{
  bufstream_tt *main_bs;        // main bufstream
  struct seek_info_t *seek_info;
  struct seek_info_t *last_seek_info;
  struct atom_info_t *atom_info;
#ifdef _BS_UNICODE
  wchar_t filename[_BS_MAX_PATH];
#else
  char filename[_BS_MAX_PATH];
#endif
};


static uint32_t bseek_usable_bytes(bufstream_tt *bs)
{
  return bs->Buf_IO_struct->main_bs->usable_bytes(bs->Buf_IO_struct->main_bs);
}

static uint8_t *bseek_request(bufstream_tt *bs, uint32_t numbytes)
{
  return bs->Buf_IO_struct->main_bs->request(bs->Buf_IO_struct->main_bs, numbytes);
}

static uint32_t bseek_confirm(bufstream_tt *bs, uint32_t numbytes)
{
  return bs->Buf_IO_struct->main_bs->confirm(bs->Buf_IO_struct->main_bs, numbytes);
}

static uint32_t bseek_copybytes(bufstream_tt *bs, uint8_t *ptr, uint32_t numbytes)
{
  return bs->Buf_IO_struct->main_bs->copybytes(bs->Buf_IO_struct->main_bs, ptr, numbytes);
}

static uint32_t bseek_split(bufstream_tt *bs)
{
  return bs->Buf_IO_struct->main_bs->split(bs->Buf_IO_struct->main_bs);
}

static uint32_t bseek_chunksize(bufstream_tt *bs)
{
  return bs->Buf_IO_struct->main_bs->chunksize(bs->Buf_IO_struct->main_bs);
}


uint8_t bseek_reorder(bufstream_tt *bs, struct atom_info_t *info_ptr, uint32_t info_size)
{
	int32_t fp;
	struct atom_info_t *header_ptr, *media_data_ptr, *file_header;
	uint64_t file_size;

	if(!bs || !info_ptr)
		return 1;

    if (!info_size){};  // remove compile warning

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
		uint8_t *media_data;

		if( bseek_file_seek(fp, media_data_ptr->atom_pos, SEEK_SET) == -1)
			return 1;
		media_data		= (uint8_t *)malloc((size_t)media_data_ptr->atom_size);
		if(bseek_file_read(fp, media_data, (uint32_t)media_data_ptr->atom_size))
			return 1;
				
		//if(bseek_write(fp, headers_data, header_ptr->atom_size))
		//	return 1;

		if(bseek_file_write(fp, media_data, (uint32_t)media_data_ptr->atom_size))
			return 1;

		free(media_data);
	}else{
		uint64_t data_size, end_header_pos, cur_read_pos, write_pos, read_pos,cur_write_pos;
		uint8_t *read_bfr, *header_buf;
		int32_t res;
		uint32_t bfr_size = 1048576, read_bfr_size = bfr_size, proc_size, nTail;

		read_bfr = (uint8_t*)malloc(bfr_size);
		memset(read_bfr, 0, bfr_size);

		//rewrite headers
		data_size = header_ptr->atom_size, write_pos = file_size - bfr_size, read_pos = header_ptr->atom_pos - bfr_size;
		end_header_pos = file_header->atom_size + header_ptr->atom_size;
		
		cur_read_pos = header_ptr->atom_pos - bfr_size, cur_write_pos = file_size - bfr_size;

		//read headers
		header_buf = (uint8_t*)malloc((size_t)header_ptr->atom_size);
		bseek_file_seek(fp, (uint32_t)header_ptr->atom_pos, SEEK_SET);
		bseek_file_read(fp, header_buf, (uint32_t)header_ptr->atom_size);

		while(read_bfr_size > 0)  // remove compiler warning
		{
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

		nTail = (uint32_t)(cur_read_pos - media_data_ptr->atom_pos);
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


static uint32_t bseek_auxinfo(bufstream_tt *bs, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size)
{
  struct impl_stream* p = bs->Buf_IO_struct;
  struct buf_seek_info *bf_seek_info;

  switch (info_ID)
  {
    case BUF_SEEK_INFO:
      bf_seek_info = (struct buf_seek_info*)info_ptr;
      if (info_size == sizeof(struct buf_seek_info))
      {
        struct seek_info_t *new_seek_info = (struct seek_info_t*)malloc(sizeof(struct seek_info_t));
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
		p->atom_info = (struct atom_info_t*)malloc(info_size);
		memset(p->atom_info, 0, info_size);
		memcpy(p->atom_info, info_ptr, info_size);
		//free(info_ptr);
		break;

	default:
	  // pass it on if we don't handle it
	  if (p->main_bs)
        p->main_bs->auxinfo(p->main_bs, offs, info_ID, info_ptr, info_size);
  }
  return BS_OK;
}


static int bseek_write_info(bufstream_tt *bs, struct seek_info_t *seek_info)
{
  int32_t i, fp;
  struct seek_info_t *sinfo;
#ifdef _DEBUG
  int32_t ix;
#endif

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
	ix = _commit(fp);
#endif

#else
    i = write(fp,(void*)&sinfo->bfr[0], sinfo->bfr_size);
#endif
    if (i != (int32_t)sinfo->bfr_size)
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


static void bseek_done(bufstream_tt *bs, int32_t Abort)
{
  struct impl_stream* p = bs->Buf_IO_struct;
  struct seek_info_t *sinfo;
  
  if (p->seek_info)
  {
    if (!Abort)
      bseek_write_info(bs, p->seek_info);
    sinfo = p->seek_info;
	while (sinfo)
	{
      struct seek_info_t *next_sinfo = sinfo->next;
	  if (sinfo->bfr)
	    free(sinfo->bfr);
	  free(sinfo);
	  sinfo = next_sinfo;
	}
  }

  if(p->atom_info)
  {
    bseek_reorder(bs, p->atom_info, sizeof(struct atom_info_t)*3);
    free(p->atom_info);
  }

  free(p);
  bs->Buf_IO_struct = NULL;
}


static void bseek_free(bufstream_tt *bs)
{
  if(bs->Buf_IO_struct)
    bs->done(bs,0);

  free(bs);
}


static int32_t bseek_init_file_buf(bufstream_tt *bs,
                                   bufstream_tt *main_bs,
                                   void (*DisplayError)(char *txt))
{
  if (DisplayError){};  // remove compile warning

  if (!main_bs)
  {
    return BS_ERROR;
  }

  memset (bs, 0, sizeof (bufstream_tt));

  bs->Buf_IO_struct = (struct impl_stream*)malloc(sizeof(struct impl_stream));
  if (!bs->Buf_IO_struct)
  {
    return BS_ERROR;
  }
  memset (bs->Buf_IO_struct, 0, sizeof(struct impl_stream));

  bs->usable_bytes = bseek_usable_bytes;
  bs->request      = bseek_request;
  bs->confirm      = bseek_confirm;
  bs->copybytes    = bseek_copybytes;
  bs->split        = bseek_split;
  bs->chunksize    = bseek_chunksize;
  bs->free         = bseek_free;
  bs->auxinfo      = bseek_auxinfo;
  bs->done         = bseek_done;
  bs->state        = NULL;
  bs->drive_ptr    = NULL;
  bs->drive        = NULL;
  bs->flags        = 0;

  bs->Buf_IO_struct->main_bs = main_bs;

#ifdef _BS_UNICODE
  main_bs->auxinfo(main_bs, 0, FILENAME_INFO, bs->Buf_IO_struct->filename, _BS_MAX_PATH * 2);
  if (!wcslen(bs->Buf_IO_struct->filename))
#else
  main_bs->auxinfo(main_bs, 0, FILENAME_INFO, bs->Buf_IO_struct->filename, _BS_MAX_PATH);
  if (!strlen(bs->Buf_IO_struct->filename))
#endif
  {
    free(bs->Buf_IO_struct);
    return BS_ERROR;
  }

  return BS_OK;
}


bufstream_tt *bseek_open_file_buf(bufstream_tt *main_bs,
                                  void (*DisplayError)(char *txt))
{
  bufstream_tt *p;
  p = (bufstream_tt*)malloc(sizeof(bufstream_tt));
  if(p)
  {
    if (BS_OK != bseek_init_file_buf(p, main_bs, DisplayError))
    {
      free(p);
      p = NULL;
    }
  }
  return p;
}


void bseek_close_file_buf(bufstream_tt* bs, int32_t Abort)
{
  bs->done(bs, Abort);
  bs->free(bs);
}


static int32_t bseek_init_fifo_buf(bufstream_tt *bs,
								   bufstream_tt *main_bs,
								   void (*DisplayError)(char *txt))
{
	if (DisplayError){};  // remove compile warning

	if (!main_bs)
	{
		return BS_ERROR;
	}

	memset (bs, 0, sizeof (bufstream_tt));

	bs->Buf_IO_struct = (struct impl_stream*)malloc(sizeof(struct impl_stream));
	if (!bs->Buf_IO_struct)
	{
		return BS_ERROR;
	}
	memset (bs->Buf_IO_struct, 0, sizeof(struct impl_stream));

	bs->usable_bytes = bseek_usable_bytes;
	bs->request      = bseek_request;
	bs->confirm      = bseek_confirm;
	bs->copybytes    = bseek_copybytes;
	bs->split        = bseek_split;
	bs->chunksize    = bseek_chunksize;
	bs->free         = bseek_free;
	bs->auxinfo      = bseek_auxinfo;
	bs->done         = bseek_done;
	bs->state        = NULL;
	bs->drive_ptr    = NULL;
	bs->drive        = NULL;
	bs->flags        = 0;

	bs->Buf_IO_struct->main_bs = main_bs;

	return BS_OK;
}


bufstream_tt *bseek_open_fifo_buf(bufstream_tt *main_bs,
								  void (*DisplayError)(char *txt))
{
	bufstream_tt *p;
	p = (bufstream_tt*)malloc(sizeof(bufstream_tt));
	if(p)
	{
		if (BS_OK != bseek_init_fifo_buf(p, main_bs, DisplayError))
		{
			free(p);
			p = NULL;
		}
	}
	return p;
}


void bseek_close_fifo_buf(bufstream_tt* bs, int32_t Abort)
{
	bs->done(bs, Abort);
	bs->free(bs);
}

