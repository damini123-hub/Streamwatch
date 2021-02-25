/**
@file: bufstrm.h
@brief stream base class (used by encoder and decoders)

@verbatim
File: bufstrm.h
Desc: stream base class (used by encoder and decoders)

Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
 **/

#if !defined (__BUFSTREAM_API_INCLUDED__)
#define __BUFSTREAM_API_INCLUDED__

#include "mctypes.h"

// comment this out to use non-UNICODE functions
// in a UNICODE project. Meant for Windows platforms
// if commented out, leave _BS_MAX_PATH defined as 2048
#ifdef _UNICODE
#define _BS_UNICODE
#define _BS_MAX_PATH  32768
#else
#define _BS_MAX_PATH  2048
#endif

#define BS_OK       (0)
#define BS_IO_ERROR (1)
#define BS_ERROR    (-1)         /* (0xFFFFFFFF) return incorrect value in 64 bits systems. */


#define BS_FLAGS_DST 0x00000001         /**<@brief Owns AuxInfo channel @hideinitializer*/
#define BS_FLAGS_ACT 0x00000002         /**<@brief Owns Drive channel, i.e. part calling HT-channel to drive information @hideinitializer*/


typedef struct bufstream bufstream_tt;

struct bufstream
{

/**
 @brief Call to inform how many bytes can be put_into/read_from buffer. Used if "request"-call fails, fifo_w_empty/fifo_r_filled
 @param[in] bs             - pointer to the object of bufstream class
 @return Number of usable bytes
*/
  uint32_t  (MC_EXPORT_API * usable_bytes)(bufstream_tt *bs);

/**
 @brief Call to request the buffer with at least numbytes-bytes
 @param[in] bs                   - Pointer to the object of bufstream class
 @param[in] numbytes             - The number of byte requested
 @return Pointer to the buffer or NULL if request fail.
*/
  uint8_t * (MC_EXPORT_API * request)(bufstream_tt *bs, uint32_t numbytes);

/**
 @brief Call to confirm numbytes-bytes filled in in requested after last "request"-call (fifo_w_commit/fifo_r_remove)
 @param[in] bs                   - Pointer to the object of bufstream class
 @param[in] numbytes             - The number of byte for confirm
*/
  uint32_t  (MC_EXPORT_API * confirm)(bufstream_tt *bs, uint32_t numbytes);

/**
 @brief Call to put/get numbytes-bytes into/from bufsteam (fifo_w_sampput/fifo_r_sampget)
 @param[in] bs                   - Pointer to the object of bufstream class
 @param[in] ptr                  - Pointer to source buffer
 @param[in] numSamples           - Number of samples
*/
  uint32_t  (MC_EXPORT_API * copybytes)(bufstream_tt *bs, uint8_t *ptr, uint32_t numSamples);

/**
 @brief Call to get the maximal chunk-size in buffer-mode (i.e. for "request"-call)
 @param[in] bs                   - Pointer to the object of bufstream class
 @return Maximal possible chunk-size for "request"-call
*/
  uint32_t  (MC_EXPORT_API * chunksize)(bufstream_tt *bs);



//
// Can one asssume that area 0...offs-1 (after last request-call)
// is filled with valid part of stream? Do we need it?
//
//

/**
@anchor bufstream_auxinfo
@code
uint32_t (MC_EXPORT_API * auxinfo)(bufstream_tt *bs, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);
@endcode
@brief Call to inform bufstream about some additional info - for example to inform MUXer about encoding-units
@param[in] bs             - pointer to the object of bufstream class
@param[in] offs           - offset from last request-call (if called after confirm/copybyte means next copybyte-chunk, i.e. we need to call auxinfo() before copybyte() or confirm() if offs != 0). AcessUnit-auxinfo is alway called with offs=0 after confirm()/copybyte()-call! Other aux-info can be putted earlier then stream arrive.
@param[in] info_ID        - specifies the type of the information
@param[in] info_ptr       - pointer to the retrieved data
@param[in] info_size      - specifies the data size
*/
  uint32_t (MC_EXPORT_API * auxinfo)(bufstream_tt *bs, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);

// allow to split output-stream {if(split!=NULL) - splittable}
// return 0 = OK
  uint32_t (MC_EXPORT_API * split) (bufstream_tt *bs);
  void  (MC_EXPORT_API * done)  (bufstream_tt *bs, int32_t Abort);
  void  (MC_EXPORT_API * free)  (bufstream_tt *bs);

// used to drive io-pipe, undefined = NULL, currently unused
  struct drive_struct *drive_ptr;
  uint32_t  (MC_EXPORT_API * drive) (bufstream_tt *bs, uint32_t info_ID, void *info_ptr, uint32_t info_size);

  uint32_t flags;                                       /**< Currently unused, setted once on create/initialisation time */
  uint32_t (MC_EXPORT_API * state) (bufstream_tt *bs);  /**< Currently unused */

  struct impl_stream* Buf_IO_struct;
};


// close_bufstream is generic done and destroy makro
//
#define close_bufstream(bs, abort) \
do { bs->done(bs, abort); bs->free(bs); bs = NULL; } while(0)
//
// bs = NULL; is very important!
//
// It force usage of close_bufstream()-macro
// on pointer-variables and produce Compiler-error
// on static/stack/part_of_struct allocated objects


#endif // #if !defined (__BUFSTREAM_API_INCLUDED__)

