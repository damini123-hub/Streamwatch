/* ----------------------------------------------------------------------------
 * File: buf_rlbn.h
 *
 * Desc: Buffered stream splitter for DVD RLBN output
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#include "bufstrm.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

bufstream_tt *open_bufstream_write_with_rlbn_index (
  bufstream_tt *main_bs,
  bufstream_tt *rlbn_idx,
  void (*DisplayError)(char *txt));

void switch_main_bs(bufstream_tt *bs, bufstream_tt *main_bs);

void close_bufstream_write_with_rlbn_index (
  bufstream_tt* bs,
  int32_t Abort);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
