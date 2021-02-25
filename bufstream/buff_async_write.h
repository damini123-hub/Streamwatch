/* ----------------------------------------------------------------------------
 * File: buff_async_write.h
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

#include "bufstrm.h"


#ifdef _BS_UNICODE
bufstream_tt *open_file_buf_async_write(const wchar_t *bs_filename,
                                  uint32_t bufsize,
                                  uint32_t max_buffers, 
                                  void (*DisplayError)(char *txt));
#else
bufstream_tt *open_file_buf_async_write(const char *bs_filename,
                                  uint32_t bufsize,
                                  uint32_t max_buffers, 
                                  void (*DisplayError)(char *txt));
#endif

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__QNX__)
void close_file_buf_async_write(bufstream_tt* &bs, int32_t Abort);
#else
void close_file_buf_async_write(bufstream_tt* bs, int32_t Abort);
#endif
