/* ----------------------------------------------------------------------------
 * File: buf_seek.h
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

#include "bufstrm.h"


#ifdef __cplusplus
extern "C" {
#endif

bufstream_tt *bseek_open_file_buf(bufstream_tt *main_bs,
							    void (*DisplayError)(char *txt));

void bseek_close_file_buf(bufstream_tt* bs, int32_t Abort);

bufstream_tt *bseek_open_fifo_buf(bufstream_tt *main_bs,
								void (*DisplayError)(char *txt));

void bseek_close_fifo_buf(bufstream_tt* bs, int32_t Abort);

#ifdef __cplusplus
}
#endif
