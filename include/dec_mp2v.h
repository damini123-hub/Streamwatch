/**
\file  dec_mp2v.h
\brief  MPEG-1/2 Video Decoder API

\verbatim
File: dec_mp2v.h
Desc: MPEG-1/2 Video Decoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
\endverbatim
**/

#if !defined (__DEC_MP2V_API_INCLUDED__)
#define __DEC_MP2V_API_INCLUDED__

#include "bufstrm.h"
#include "mcdefs.h"
#include "mcapiext.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
  sample usage of streaming MPEG-decoder

  int n;
  struct SEQ_Params *pSEQ;
  struct PIC_Params *pPIC;
  frame_tt output_frame;
  unsigned long state;


  vs = open_MPEGin_Video_stream();
  vs->auxinfo(vs,0,PARSE_SEQ_HDR,NULL,0);
  n = vs->copybytes(chunk_ptr,chunk_size);
  if(n==0)
  {
    MPF(DBG_ERR,("Could not find sequence header"));
    exit_on_error();
  }
  vs->auxinfo(vs,0,GET_SEQ_PARAMSP,&pSEQ,sizeof(pSEQ));

  // here:
  // check header parameters, initialize play/framebuffer, etc.

  if(vs->auxinfo(vs,0,INIT_FRAME_PARSER,NULL,0))
  {
    MPF(DBG_ERR,("Could not find initialize frame parser"));
    exit_on_error();
  };

  // here:
  // Create framebuffer and fill frame_tt structure with output-buffer
  // information

  vs->auxinfo(vs,skip_frame_code,PARSE_FRAMES,&output_frame,sizeof(output_frame));

  state=0;

put_chunks_into_parser:

  // here:
  // put next stream-chunk into parser
  // we need to repeat this step to put all frame-data into parser

  n=vs->copybytes(chunk_ptr, chunk_size);

  // n - how many bytes from the chunk were used. It is possible (and normal)
  // to obtain 0 here: it means that no bytes from this chunk were used to
  // finish frame-decode. One need to call copybytes with this chunk to
  // start next frame decode.

  state=vs->auxinfo(vs,0,GET_PARSE_STATE,NULL,0);

  if(state | PARSE_DONE_FLAG)
  {
    //end of frame achieved
    if(state | PIC_VALID_FLAG)
    {
      vs->auxinfo(vs,0,GET_PIC_PARAMSP,&pPIC,sizeof(pPIC));
      // frame_buffer is valid
      // do something with it and contine/stop stream parcing.
      // parameters from pPIC can be used to do frame reordering
    }
  }
  if(!eos)
    goto put_chunks_into_parser;

  // we have no more chunks.
  // to finish decode we need to continue on cached part of stream:

  n=vs->copybytes(NULL, 0);

  //n means (opposite to normal copybytes-call) how many bytes are still cached in
  // parser

  state=vs->auxinfo(vs,0,GET_PARSE_STATE,NULL,0);
  if(state | PARSE_DONE_FLAG)
  {
    //end of frame achieved
    if(state | PIC_VALID_FLAG)
    {
      vs->auxinfo(vs,0,GET_PIC_PARAMSP,&pPIC,sizeof(pPIC));
      // frame_buffer is valid
      // do something with it and contine/stop stream parcing.
      // parameters from pPIC can be used to do frame reordering
    }
  }

  close_bufstream(vs);
*/


#define MPEG_DEC_OPT_MT_OFF        0x00000010L	/**< \brief  turn off multithreading in decoder \hideinitializer  */
#define MPEG_DEC_OPT_I_ONLY		   0x00000020L	/**< \brief I-only decoder. Obsolete option flag, it is not supported since 8.0 \hideinitializer  */


/**
\brief Get video decoder version.
\return  The video decoder version.
 */
int MC_EXPORT_API MPEGin_decoder_version();

/**
\brief Call this function to get a bufstream interface for the MPEG-2/1 video decoder.
\return  The video decoder instance.
 */
bufstream_tt * MC_EXPORT_API open_MPEGin_Video_stream(void); //quick and dirty default initialization

/**
\brief Call this function to get a bufstream interface for the MPEG-2/1 video decoder.
\return  The video decoder instance.
\param[in] get_rc  callbacks to external message handler and memory manager functions
\param[in] reserved1 reserved parameter
\param[in] reserved2 reserved parameter
 */
bufstream_tt * MC_EXPORT_API open_MPEGin_Video_stream_ex(void *(MC_EXPORT_API *get_rc)(const char* name), long options, long reserved2);


/**
\brief Provides access to extended module API.
\return  pointer to requested function or NULL
\param[in] func	identifier of module extended function
*/
APIEXTFUNC MC_EXPORT_API MPEGin_Video_GetAPIExt(uint32_t func);

/** \brief data structure used for DVD sub-picture blending */
typedef struct {
	uint8_t *	decoded_bmp;		/**< \brief Sub-picture data buffer*/
	uint8_t		palette[16][3];		/**< \brief Sub-picture 16-entry YUV palette*/
	uint8_t	*	palette_color;		/**< \brief Sub-picture palette colors*/
	uint8_t	*	palette_contrast;	/**< \brief Sub-picture palette contrasts*/
	uint32_t	sp_top;				/**< \brief Sub-picture top position*/
	uint32_t	sp_left;			/**< \brief Sub-picture left position*/
	uint32_t	sp_bottom;			/**< \brief Sub-picture bottom position*/
	uint32_t	sp_right;			/**< \brief Sub-picture right position*/
	uint32_t	highlight;			/**< \brief Turn on/off highlight sub-picture area*/
	uint32_t	highlight_top;		/**< \brief Hightlight area top position*/
	uint32_t	highlight_left;		/**< \brief Hightlight area left position*/
	uint32_t	highlight_bottom;	/**< \brief Hightlight area bottom position*/
	uint32_t	highlight_right;	/**< \brief Hightlight area right position*/
} dvd_subpic_params_t;
/** \} */

/** \brief extended frame structure */
typedef struct {
	frame_tt * frame;					/**< \brief basic frame information*/
	dxva_surface_t ** dxva_surface;		/**< \brief dxva surface, only when hardaware acceleration used*/
	dvd_subpic_params_t * subpic_params;/**< \brief DVD sub picture parameters*/
} adv_frame_tt;
/** \} */

/** \brief extended frame structure */
typedef struct {
  frame_tt frame_data;						/**< \brief basic frame information*/
  UserData_Params * pic_user_data_params;	/**< \brief user data information*/
  long pic_coding_order;					/**< \brief GOP picture order count*/
  long reserved[64];						/**< \brief reserved*/
} frame_ext2_tt;
/** \} */
#ifdef __cplusplus
}
#endif

/** \brief time code information. See IEC standard publication 60461 for "time and control codes for video tape recorders" for reference */
typedef struct {
	int hours;
	int minutes;
	int seconds;
	int pictures;
} mpeg_pic_timecode_t;
/** \} */

/** \brief Sequence header structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_seq_hdr_s {
	int		horizontal_size_value;
	int		vertical_size_value;
	int		aspect_ratio_information;
	int		frame_rate_code;
	int		bit_rate_value;
	int		marker_bit;
	int		vbv_buffer_size_value;
	int		constrained_parameters_flag;
	int		load_intra_quantiser_matrix;
	uint8_t	intra_quantiser_matrix[64];
	int		load_non_intra_quantiser_matrix;
	uint8_t	non_intra_quantiser_matrix[64];
	int		valid_seq_hdr;
} mpgvdec_seq_hdr_t;
/** \} */

/** \brief Sequence extension structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_seq_ext_hdr_s {
	int	profile_and_level_indication;
	int	progressive_sequence;
	int	chroma_format;
	int	horizontal_size_extension;
	int	vertical_size_extension;
	int	bit_rate_extension;
	int	marker_bit;
	int	vbv_buffer_size_extension;
	int	low_delay;
	int	frame_rate_extension_n;
	int	frame_rate_extension_d;
	int valid_seq_ext_hdr;
} mpgvdec_seq_ext_hdr_t;
/** \} */

/** \brief Sequence display extension structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_seq_disp_ext_hdr_s {
	int	video_format;
	int	colour_description;
	int	colour_primaries;
	int	transfer_characteristics;
	int	matrix_coefficients;
	int	display_horizontal_size;
	int	marker_bit;
	int	display_vertical_size;
	int valid_seq_disp_ext_hdr;
} mpgvdec_seq_disp_ext_hdr_t;
/** \} */

/** \brief Sequence scalable extension structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_seq_scalable_ext_hdr_s {
	int scalable_mode;
	int layer_id;
	int lower_layer_prediction_horizontal_size;
	int marker_bit;
	int lower_layer_prediction_vertical_size;
	int horizontal_subsampling_factor_m;
	int	horizontal_subsampling_factor_n;
	int	vertical_subsampling_factor_m;
	int	vertical_subsampling_factor_n;
	int picture_mux_enable;
	int mux_to_progressive_sequence;
	int picture_mux_order;
	int picture_mux_factor;
	int valid_seq_scalable_ext_hdr;
} mpgvdec_seq_scalable_ext_hdr_t;
/** \} */

/** \brief Group of pictures header structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_group_of_pictures_hdr_s {
	int	drop_flag;
	int	hour;
	int	minute;
	int marker_bit;
	int	sec;
	int	frame;
	int	closed_gop;
	int	broken_link;
	int valid_gop_hdr;
} mpgvdec_group_of_pictures_hdr_t;
/** \} */

/** \brief Picture header structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_picture_hdr_s {
	int	temporal_reference;
	int	picture_coding_type;
	int	vbv_delay;
	int	full_pel_forward_vector;
	int	forward_f_code;
	int	full_pel_backward_vector;
	int	backward_f_code;
	int valid_pic_hdr;
} mpgvdec_picture_hdr_t;
/** \} */

/** \brief Picture coding extension structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_picture_coding_ext_hdr_s {
	int	f_code_0_0;
	int	f_code_0_1;
	int	f_code_1_0;
	int	f_code_1_1;
	int	intra_dc_precision;
	int	picture_structure;
	int	top_field_first;
	int	frame_pred_frame_dct;
	int	concealment_motion_vectors;
	int	q_scale_type;
	int	intra_vlc_format;
	int	alternate_scan;
	int	repeat_first_field;
	int	chroma_420_type;
	int	progressive_frame;
	int	composite_display_flag;
	int	v_axis;
	int	field_sequence;
	int	sub_carrier;
	int	burst_amplitude;
	int	sub_carrier_phase;
	int valid_pic_coding_ext_hdr;
} mpgvdec_picture_coding_ext_hdr_t;
/** \} */

/** \brief Quant matrix extension structure. See ISO/IEC 13818-2  for reference.  */
typedef struct mpgvdec_quant_matrix_ext_hdr_s {
	int		load_intra_quantiser_matrix;
	uint8_t	intra_quantiser_matrix[64];
	int		load_non_intra_quantiser_matrix;
	uint8_t non_intra_quantiser_matrix[64];
	int		load_chroma_intra_quantiser_matrix;
	uint8_t chroma_intra_quantiser_matrix[64];
	int		load_chroma_non_intra_quantiser_matrix;
	uint8_t chroma_non_intra_quantiser_matrix[64];
	int		valid_quant_matrix_ext_hdr;
} mpgvdec_quant_matrix_ext_hdr_t;
/** \} */

/** \brief Picture display extension structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_picture_disp_ext_hdr_s {
	int		number_of_frame_centre_offsets;
	int		frame_centre_horizontal_offset[3];
	int		frame_centre_vertical_offset[3];
	int		valid_pic_disp_ext_hdr;
} mpgvdec_picture_disp_ext_hdr_t;
/** \} */

/** \brief Slice structure. See ISO/IEC 13818-2  for reference. */
typedef struct mpgvdec_slice_hdr_s {
	int slice_vertical_position_extension;
	int quantiser_scale_code;
	int intra_slice_flag;
	int intra_slice;
	int valid_slice_hdr;
} mpgvdec_slice_hdr_t;
/** \} */

#endif /* __DEC_MP2V_API_INCLUDED__ */

