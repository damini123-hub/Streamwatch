/*!
 * @file mfimport_defs.h
 * @brief Media Format Importer extended sample information definitions
 * File: mfimport_defs.h
 *
 * Desc: Media Format Importer extended sample information definitions
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 */

#if !defined (__MFIMPORT_DEFS_INCLUDED__)
#define __MFIMPORT_DEFS_INCLUDED__

#include "mcdefs.h"
#include "mcmediatypes.h"
#include "dec_avc.h"
//#include "dec_hevc.h"
#include "dv_info.h"
#include "dec_vc1.h"
#include "dec_aac.h"
#include "dec_dts.h"


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

//! @name video extended sample information
//!@{

/*!
 @brief Extended sample information for mp2v. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
*/
typedef struct mfimport_mp2v_sample_info_s
{
    // native info
    struct SEQ_ParamsEx seq_params;                 //!< video sequence header, defined in mcdefs.h
    struct SEQ_Ext_Params seq_ext_params;           //!< video sequnce extension, defined in mcdefs.h
    struct SEQ_Disp_Ext_Params seq_disp_ext_params; //!< sequence display extension, defined in mcdefs.h
    struct GOP_Params gop_params;                   //!< group of pictures header, defined in mcdefs.h
    struct PIC_ParamsEx pic_params;                 //!< video picture header and coding extension, defined in mcdefs.h
    UserData_Params user_data;                      //!< user data from the decoder associated with current decoded picture, defined in mcdefs.h

} mfimport_mp2v_sample_info_t;


/*!
 @brief Extended sample information for avc. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_avc_sample_info_s
{
    // native info
    h264_seq_par_set_t seq_par_set;              //!< sequence paramter set, defined in dec_avc.h, see ISO/IEC 14496-10 for reference
    h264_pic_par_set_t pic_par_set;              //!< picture parameter set, defined in dec_avc.h, See ISO/IEC 14496-10 for reference
    user_data_t user_data;                       //!< user data, defined in dec_avc.h

    // for compatiblity
    struct GOP_Params gop_params;                //!< group of pictures header, defined in mcdefs.h
    struct PIC_ParamsEx pic_params;              //!< video picture header and coding extension, defined in mcdefs.h

} mfimport_avc_sample_info_t;


/*!
 @brief Extended sample information for hevc. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_hevc_sample_info_s
{
    // native info not available yet
//    hevc_vid_par_set_t vid_par_set;              //!< video paramter set, defined in dec_hevc.h
//    hevc_seq_par_set_t seq_par_set;              //!< sequence paramter set, defined in dec_hevc.h
//    hevc_pic_par_set_t pic_par_set;              //!< picture parameter set, defined in dec_hevc.h
//    hevc_user_data_t user_data;                  //!< user data, defined in dec_hevc.h

    // for compatiblity
    struct SEQ_ParamsEx seq_params;              //!< video sequence header, defined in mcdefs.h
    struct PIC_ParamsEx pic_params;              //!< video picture header and coding extension, defined in mcdefs.h

} mfimport_hevc_sample_info_t;


/*!
 @brief Extended sample information for dv. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_dv_sample_info_s
{
    // native info
    dv_video_info_ex frame_info;                 //!<  extended dv info - video system parameters, defined in dv_info.h

} mfimport_dv_sample_info_t;


/*!
 @brief Extended sample information for mp4v. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_mp4v_sample_info_s
{
    // native info
    UserData_Info user_data;                     //!< information about user data found in the video stream, defined in mcdefs.h

    // for compatiblity
    struct SEQ_ParamsEx seq_params;              //!< video sequence header, defined in mcdefs.h
    struct PIC_ParamsEx pic_params;              //!< video picture header and coding extension, defined in mcdefs.h

} mfimport_mp4v_sample_info_t;


/*!
 @brief Extended sample information for vc1. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_vc1_sample_info_s
{
    // native info
    vc1_sequence_layer seq_layer;                //!< sequence and layer parameters, defined in dec_vc1.h, see SMPTE 421M-2006 for reference

    // for compatiblity
    struct PIC_ParamsEx pic_params;              //!< video picture header and coding extension, defined in mcdefs.h

} mfimport_vc1_sample_info_t;


/*!
 @brief Extended sample information for vc3. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_vc3_sample_info_s
{
    // for compatiblity
    struct SEQ_ParamsEx seq_params;              //!< video sequence header, defined in mcdefs.h
    struct PIC_ParamsEx pic_params;              //!< video picture header and coding extension, defined in mcdefs.h

} mfimport_vc3_sample_info_t;

//!@}

//! @name audio extended sample information
//!@{

/*!
 @brief Extended sample information for aac. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_aac_sample_info_s
{
    // native info
    aac_decoded_frame_info frame_info;           //!< information about the decoded AAC frame, defined in dec_aac.h

} mfimport_aac_sample_info_t;


/*!
 @brief Extended sample information for Dolby Digital. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_dd_sample_info_s
{
    // native info
    ac3_frame_hdr frame_info;                    //!< information about the audio stream, defined in mcdefs.h
    dd_gen_info_tt gen_info;                     //!< information about the audio stream, defined in mcdefs.h
    dd_mix_info_tt mix_info;                     //!< information about the audio stream, defined in mcdefs.h
    dd_prod_info_tt prod_info;                   //!< information about the audio stream, defined in mcdefs.h
    dd_timecode_info_tt timecode_info;           //!< information about the audio stream, defined in mcdefs.h
    dd_addbsi_info_tt addbsi_info;               //!< information about the audio stream, defined in mcdefs.h
    dd_auxdata_info_tt auxdata_info;             //!< information about the audio stream, defined in mcdefs.h

} mfimport_dd_sample_info_t;


/*!
 @brief Extended sample information for DTS. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_dts_sample_info_s
{
    // native info
    dts_decoded_frame_info frame_info;           //!< DTS decoder audio frame information, defined in dec_dts.h

} mfimport_dts_sample_info_t;


/*!
 @brief Extended sample information for mpeg. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_mpa_sample_info_s
{
    // native info
    aud_frame_hdr frame_info;                    //!< information about audio found in audio stream, defined in mcdefs.h

} mfimport_mpa_sample_info_t;


/*!
 @brief Extended sample information for mp3. See mfi_sample_info_t.p_ext_info and
 mfi_sample_info_t.ext_info_len.
 @details Contains the info defined by the decoders which would normally be
 returned by the decoders auxinfo interface.
 */
typedef struct mfimport_mp3_sample_info_s
{
    // native info
    mp3d_audio_header frame_info;                //!< MP3 main header, defined in mcdefs.h

} mfimport_mp3_sample_info_t;

//!@}

#pragma pack(pop)

#endif // #if !defined (__MFIMPORT_DEFS_INCLUDED__)
