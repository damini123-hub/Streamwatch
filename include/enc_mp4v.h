/**
@file: enc_mp4v.h
@brief MPEG-4 Video Encoder API

@verbatim
File: enc_mp4v.h
Desc: MPEG-4 Video Encoder API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/


#if !defined (__ENC_MP4V_API_INCLUDED__)
#define __ENC_MP4V_API_INCLUDED__

#include "mctypes.h"
#include "mcprofile.h"
#include "v_frame.h"
#include "bufstrm.h"
#include "mcapiext.h"
#include "mcdefs.h"

typedef struct bufstream     bufstrm_tt;
typedef struct m4_v_encoder  m4venc_tt;
typedef struct m4_v_settings m4_v_settings_tt;

#pragma pack(push,1)

/**
 * @name Settings Structure
 * @{
 **/

 /**
 *@brief These parameters are used to configure the encoder.
 */

struct m4_v_settings
{
  int32_t   profile_id;                 /**<@brief  MPEG4_PROFILE_SIMPLE or MPEG4_PROFILE_SIMPLE_ADV */
  int32_t   level_id;                   /**<@brief  MPEG4_LEVEL_0 -> MPEG4_LEVEL_5 */

 /**
 * @name Picture size
 * @{
 **/
  int32_t   width;                      /**<@brief  Picture width  */
  int32_t   height;                     /**<@brief  Picture height */
  /**@}*/

  int32_t   video_type;                 /**<@brief @brief One of @ref MPEG4_VIDEO_TYPES */

/**
 * @name Interlace options
 * @{
 **/
  int32_t   deinterlacing_enable;       /**<@brief  Enable/disable deinterlacing */
  int32_t   field_order;                /**<@brief  Field order                  */
  int32_t   alternative_scan;           /**<@brief  Alternative scan mode        */
/**@}*/

/**
 * @name Quantizer options
 * @{
 **/
  int32_t   quant_type;                 /**<@brief  H.263 or MPEG quantization         */
  int32_t   quant_i;                    /**<@brief  Quantization parameter for I-Frame */
  int32_t   quality;                    /**<@brief  [in] use for RC_Q_QUALITY          */
  int32_t   max_quantizer;              /**<@brief  The upper limit of the quantizer   */
  int32_t   min_quantizer;              /**<@brief  The lower limit of the quantizer   */
  int32_t   adapt_quant;                /**<@brief  Adaptive quntization               */
  int32_t   quant_p;                    /**<@brief  Quantization parameter for P-Frame */
  int32_t   quant_b;                    /**<@brief  Quantization parameter for B-Frame */

  int32_t   reserved_1[15];
/**@}*/

  int32_t   interlace_mode;             /**<@brief  Interlace mode                     */
  int32_t   video_pulldown_flag;        /**<@brief  Video Pulldown flag                */

  int32_t   reserved_11[12];

  int32_t   no_limit_bitrate_by_level;  /**<@brief  0 - encoder restricts bit-rate by level, 1 - encoder does not restrict bit-rate by level */
  int32_t   target_bitrate;             /**<@brief  Average target bitrate */
  int32_t   rc_mode;                    /**<@brief  Rate control mode */

/**
 * @name GOP structure
 * @{
 **/
  int32_t   bvop_auto;                  /**<@brief         */
  int32_t   bvop_count;                 /**<@brief         */
  int32_t   scene_change;               /**<@brief  Enable/disable scene change detection   */
  int32_t   max_key_interval;           /**<@brief  The maximum interval between key frames. Value 0 - Enable infinite GOP*/
/**@}*/

/**
 * @name VBV Parameters
 * @{
 **/

  int32_t   max_bitrate;                /**<@brief  Max bitrate, used in VBR mode */
  int32_t   vbv_buffer;                 /**<@brief  VBV buffer size               */
  int32_t   write_vbv_params;           /**<@brief  Write VBV buffer parameters   */
  int32_t   vbv_occupancy;              /**<@brief VBV occupancy                  */
  int32_t   reserved_2[27];
/**@}*/
  double    frame_rate;                 /**<@brief  Use this instead of out_fps, available values like 25.00, 29.97 */
  int32_t   out_fps;                    /**<@brief   */
  int32_t   fixed_vop_rate;             /**<@brief  Set fixed_vop_rate to 1 in VOL header, use constant fixed_vop_time_increment in VOP header */

  int32_t   reserved_22;

/**
 * @name Bitstream options
 * @{
 **/
  int32_t   h263_mode;                  /**<@brief  0 - MPEG-4  bitstream, 1 - MPEG-4 short header bitstream, 2 - ITU-T H.263 bistream */

  int32_t   data_partitioned;
  int32_t   resync_marker;
  int32_t   slice_count;                /**<@brief  Number of slices per picture */
/**@}*/

/**
 * @name Stream headers
 * @{
 **/
  int32_t   closed_gov;
  int32_t   vol_every_i_vop;
  int32_t   user_data_low_delay;

  int32_t   pixel_aspect_ratio;         /**<@brief  Value of pixel aspect ratio           */
  int32_t   pixel_ar_width;             /**<@brief  Horizontal size of pixel aspect ratio */
  int32_t   pixel_ar_height;            /**<@brief  Vertical size of pixel aspect ratio   */

  int32_t   h263_reserved;

  int32_t   gov_every_i_vop;

  int32_t   h263_unrestricted_mv;
  int32_t   h263_advanced_intra_coding;
  int32_t   h263_deblocking_filter;
  int32_t   h263_slice_structured;
  int32_t   h263_modified_quantization;

  int32_t   reserved_3[22];
/**@}*/

/**
 * @name Motion estimation options
 * @{
 **/
  int32_t   me_4mv;
  int32_t   me_subpel;                  /**<@brief  Subpixel motion estimation. 0: full pixel locations, 1: half pixel, 2: quarter pixel           */
  int32_t   me_f_code;                  /**<@brief  0 - auto. 1,2,3,5,6,7 - fixed f_code value                                                     */
  int32_t   rounding_control;           /**<@brief  0 - rounding_control value is 0, 1 - rounding_control is varying                               */
  int32_t   me_fast;                    /**<@brief  0 - default, 1 - enable some speed optimizations                                               */
  int32_t   me_extended_predictors;     /**<@brief  0 - easy, 1 - standard(default), 2 - check additional prediction vectors at motion searching   */
  int32_t	reserved_4[28];
/**@}*/

/**
 * @name Prediction options
 * @{
 **/
  int32_t   ac_prediction;
  int32_t   reserved_5[16];
/**@}*/

/**
 * @name Advanced settings
 * @{
 **/
  int32_t   send_b_vop_mode;

  int32_t   reserved_50;
  int32_t   reserved_51;

  int32_t   calc_quality;             /**<@brief  Calculate PSNR */
  int32_t   num_threads;              /**<@brief  Number of threads */
  int32_t   cc_pix_range;                 /**<@brief  Pixel range conversion, changes the range of pixels values.
                                                     <table>
                                                     <tr>
                                                         <th>cc_clamp</th>
                                                         <th>Encoded pixels range</th>
                                                     </tr>
                                                     <tr>
                                                         <td>0</td>
                                                         <td>No conversion. Indicates what input stream is full range [0..255] and encoded stream should be full range.</td>
                                                     </tr>
                                                     <tr>
                                                         <td>1</td>
                                                         <td>Conversion to short range [16..235]. Indicates what input stream is full range and encoded stream should be convert to short range.</td>
                                                     </tr>
                                                     <tr>
                                                         <td>2</td>
                                                         <td>Keep short range [16..235]. No conversion, but doesn't allow to go beyond short range when use color resampling (for example when convert from 4:2:0 to 4:2:2 format)</td>
                                                     </tr>
                                                     <tr>
                                                         <td>3</td>
                                                         <td>Conversion to full range [0..255]. Indicates what input stream is short range and encoded stream should be convert to full range.</td>
                                                     </tr>
                                                     </table>
                                        @hideinitializer*/

  int32_t   reserved[509];
/**@}*/
}; // end of m4_v_settings
/**@}*/
#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fill m4_v_settings-structure with defaults values.
 * @param[out] set       - destination for encoder settings
 * @param[in] video_type - encoder preset - any from @ref MPEG4_SP_L0 .. @ref MPEG4_CUSTOM
 * @param[in] PAL        - video standard - PAL (1) or NTSC (0)
 */
int8_t * MC_EXPORT_API m4OutVideoDefaults(struct  m4_v_settings *set,
                                          int32_t   video_type,
                                          int32_t   PAL);

/**
 * @brief Change performance-related part of m4_v_settings-structure
 * @param[out] set    - A pointer to a m4_v_settings structure to be changed
 * @param[in] mode    - Not used so far, set to 0
 * @param[in] level   - Speed/quality encoding preset: from 0 (the fastest encoding speed) to 15 (the best  quality), recommended value is 9
 * @return  NONE
 */

void MC_EXPORT_API m4OutVideoPerformance(struct m4_v_settings *set,
                                         int32_t               mode,
                                         int32_t               level,
                                         int32_t               reserved);

/**
* @brief Call this function to create an m4venc_tt video encoding object which is used in other video encoding calls.
 * @param[in] get_rc  - A pointer to a get_rc function
 * @param[in] set     - A pointer to a filled in m4_v_settings structure
 * @param[in] options - Reserved, set to 0
 * @param[in] CPU     - Set to 0xFFFFFFFF for autodetect (reserved for CPU-flags)
 * @param[in] frame0  - First frame number to encode
 * @param[in] nframes - Number of frames to encode, can be 0 for streaming mode
 * @return A pointer to a m4venc_tt object if successful,  NULL if unsuccessful
 */
m4venc_tt * MC_EXPORT_API m4OutVideoNew(void *(MC_EXPORT_API       *get_rc)(const char*  name),
                                        const struct m4_v_settings *set,
                                        int32_t                     options,
                                        int32_t                     CPU,
                                        int32_t                     frame0,
                                        int32_t                     nframes);

/**
 * @brief Call this function to free an m4venc_tt object created with the m4OutVideoNew function.
 * @param[in] instance - A pointer to a m4venc_tt object created with a m4OutVideoNew call
 * @return NONE
 */
void MC_EXPORT_API m4OutVideoFree(m4venc_tt * instance);


#ifdef RC_2VBR
#define INIT_OPT_VBR_ANALYSE     0x00000900
#define INIT_OPT_VBR_ENCODING    0x00000F00
#define INIT_OPT_VBR_PASSES_MASK 0x00000F00
#define INIT_OPT_VBR_EXT_STORAGE 0x00000100
#endif

#define INIT_OPT_CHAPTER_LIST   0x00001000

struct mpeg4_chapter_position
{
    int32_t frame_nr;
    int32_t flags;
};

#define CHAPTER_END_OF_LIST    (~0L)

/**
* @brief call to initialize encoding settings.
* @param[in] instance      - The video encoder instance.
* @param[in] videobs       - Bitstream object for writing data. Must be initialized before.
* @param[in] options_flags - Options flags
* @param[in] opt_ptr       - Options pointers
* @return Return one of defined errors (@ref M4ERROR_NONE or @ref M4ERROR_FILED)
*/
int32_t MC_EXPORT_API m4OutVideoInit(m4venc_tt        *instance,
                                     struct bufstream *videobs,
                                     uint32_t          options_flags,
                                     void            **opt_ptr);

/**
* @brief Call to finish a video encoding session, set the abort flag to a non-zero if video encoding is being aborted.
 * @param[in] instance - A pointer to a m4venc_tt object created with a m4OutVideoNew call
 * @param[in] abort    - Abort If abort equals 0, finish any leftover video and clean up, else just clean up.
 * @return M4ERROR_NONE if successful, M4ERROR_FAILED if unsuccessful
 */
int32_t MC_EXPORT_API m4OutVideoDone(m4venc_tt * instance, int abort);

// For all extended option define flags OPT_EXT_PARAM_... please see "mcdefs.h" now.

/**
 * @brief Call to encode one video frame.
 * @param[in] instance      - The video encoder instance.
 * @param[in] pbSrc         - Pointer to the frame buffer to be encoded.
 * @param[in] src_line_size - Stride of source frame
 * @param[in] src_width     - Width of source frame
 * @param[in] src_height    - Height of source frame
 * @param[in] fourcc        - Source colorspace
 * @param[in] opt_flags     - Options flags
 * @param[in] ext_info      - Extended info
 * @return    Return one of defined errors (@ref M4ERROR_NONE .. @ref M4ERROR_FAILED)
 */

int32_t MC_EXPORT_API m4OutVideoPutFrame(m4venc_tt *instance,
                                         uint8_t   *pb_src,
                                         int32_t    src_line_size,
                                         int32_t    src_width,
                                         int32_t    src_height,
                                         uint32_t   fourcc,
                                         uint32_t   opt_flags,
                                         void     **ext_info);

/**
 * @brief Call to encode one video frame. Similar to above method.
 * @param[in] instance  - the video encoder instance.
 * @param[in] pframe_v  - the frame to be encoded..
 * @param[in] opt_flags - options flags
 * @param[in] ext_info  - extended info
 * @return    Return one of defined errors (@ref M4ERROR_NONE .. @ref M4ERROR_FAILED)
 */
int32_t MC_EXPORT_API m4OutVideoPutFrameV(m4venc_tt      *instance,
                                          video_frame_tt *pframe_v,
                                          uint32_t        opt_flags,
                                          void          **ext_info);

/**
* @brief Call this function to update the video settings during encoding. For now it is possible to change all settings except width and height.
* @param[in] instance - A pointer to a m4venc_tt object
* @param[in] set      - A pointer to a filled in m4_v_settings structure
* @return   M4ERROR_NONE if successful, M4ERROR_FAILED if unsuccessful
*/
int32_t MC_EXPORT_API m4OutVideoUpdateSettings(m4venc_tt                  *instance,
                                               const struct m4_v_settings *set);

/**
* @brief Call this function to check whether the video settings are valid. Error messages are sent to the err_printf (wrn_printf) callback for every error (warning) found.
* @param[in] get_rc  - The get_rc callback to provide an err_printf callback to get error messages that can be localized.
* @param[in] set     - A pointer to a filled in m4_v_settings structure
* @param[in] options - One or more of the CHECK_* defines
* @param[in] app     - reserved, set to NULL
* @return M4ERROR_NONE if successful, M4ERROR_FAILED if unsuccessful
*/
int32_t MC_EXPORT_API m4OutVideoChkSettings(void *(MC_EXPORT_API *get_rc)(const char* name),
                                            struct m4_v_settings *set,
                                            uint32_t              options,
                                            void                 *app);


/**
* @brief Returns buffer with parameter sets and its length
* @param[in]     instance - the video encoder instance.
* @param[in]     set      - the encoder settings.
* @param[out]    buffer   - output buffer
* @param[in/out] length   - the size of the buffer "buffer" / fullness of the buffer
* @return Return one of defined errors (@ref M4ERROR_NONE or @ref M4ERROR_FAILED)
*/
int32_t MC_EXPORT_API m4OutVideoGetParSets(m4venc_tt            *instance,
                                           struct m4_v_settings *set,
                                           uint8_t              *buffer,
                                           int32_t              *length);

/**
 * @brief flushes encoder queue
 * @param[in] instance - the video encoder instance.
 * @param[in] abort    - abort flag
 */
int32_t MC_EXPORT_API m4OutVideoFlush(m4venc_tt * instance, int abort);


/**
 * @brief Get extended API function pointers
 * @param[in] ID of function to return
 */
APIEXTFUNC  MC_EXPORT_API m4OutVideoGetAPIExt(uint32_t func);


#ifdef __cplusplus
}
#endif


#endif // #if !defined (__ENC_MP4V_API_INCLUDED__)

