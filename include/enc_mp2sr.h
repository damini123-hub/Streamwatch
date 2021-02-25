/**
@file: enc_mp2sr.h
@brief MPEG-1/2 Smartrender Wrapper API

@verbatim
File: enc_mp2sr.h
Desc: MPEG-1/2 Smartrender Wrapper API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__ENC_MP2SR_API_INCLUDED__)
#define __ENC_MP2SR_API_INCLUDED__

#if !defined (API_EXT_PARAM_LIST)
#define API_EXT_PARAM_LIST
#endif

#include "enc_mp2v.h"
#include "enc_mpa.h"
#include "mfimport.h"

/**
 * @name Smartrender Error Flags
 * Additional error flags for smart rendering. They are returned by the
 * srWrapperInputCheck and srWrapperInputAttach functions and describe if
 * and why smartrendering is available or not.
 * @{
 */
#define mpegOutVidSR           0x00000010   /**<@brief Video can be smart rendered @hideinitializer*/
#define mpegOutAudSR           0x00000020   /**<@brief Audio can be smart rendered @hideinitializer*/
#define mpegOutVidSRRequant    0x00000040   /**<@brief Video will be requantized (bitrates do not match) @hideinitializer*/

#define mpegOutVidError        0x00000100  /**<@brief Video error flag @hideinitializer*/
#define mpegOutVidNoVid        0x00000300  /**<@brief No video available @hideinitializer*/
#define mpegOutVidHSize        0x00000500  /**<@brief Horizontal sizes do not match @hideinitializer*/
#define mpegOutVidVSize        0x00000900  /**<@brief Vertical sizes do not match @hideinitializer*/
#define mpegOutVidM            0x00001100  /**<@brief MPEG M's do not match @hideinitializer*/
#define mpegOutVidN            0x00002100  /**<@brief MPEG N's do not match @hideinitializer*/
#define mpegOutVidType         0x00004100  /**<@brief MPEG type (1 or 2) do not match @hideinitializer*/
#define mpegOutVidBitrateType  0x00008100  /**<@brief The bitrate types (constant/variable) do not match @hideinitializer*/
#define mpegOutVidBitrate      0x00010100  /**<@brief The bitrates do not match @hideinitializer*/
#define mpegOutVidFieldOrder   0x00020100  /**<@brief The progressive frame or field order do not match @hideinitializer*/
#define mpegOutVidFrameRate    0x00040100  /**<@brief The frame rate does not match @hideinitializer*/
#define mpegOutChromaFormat    0x00080100  /**<@brief Chroma format does not match @hideinitializer*/

#define mpegOutAudError        0x00100000  /**<@brief Audio error flag @hideinitializer*/
#define mpegOutAudNoAud        0x00300000  /**<@brief No audio avaliable @hideinitializer*/
#define mpegOutAudLayer        0x00500000  /**<@brief Layers do not match @hideinitializer*/
#define mpegOutAudFreq         0x00900000  /**<@brief Frequencies do not match @hideinitializer*/
#define mpegOutAudBitrate      0x01100000  /**<@brief Bitrates do not match @hideinitializer*/
#define mpegOutAudCRC          0x02100000  /**<@brief CRC's (enabled/disabled) do not match @hideinitializer*/
#define mpegOutAudMode         0x04100000  /**<@brief Mode's (stereo, mono, etc.) do not match @hideinitializer*/
/** @} */

// smartrender wrapper object
/**
 * @name Smartrender Wrapper Instance
 * This type is used as handle for the smartrender wrapper instance.
 * @{
 */
typedef struct sr_wrapper srwrapper_tt;
/** @} */


#ifdef __cplusplus
extern "C" {
#endif

//------------------- general routines -----------------

/**
 * @brief srWrapperInputCheck check if an input file and export settings are compatible to enable smartrendering.
 * @param[in] v_settings
 * @param[in] a_settings
 * @param[in] in_info
 * @param[in] audio_rate
 * @return               returns a combination of codes defined above.
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperInputCheck(struct mpeg_v_settings *v_settings,
                                          struct mpeg_a_settings *a_settings,
                                          struct mpegInInfo      *in_info,
                                          uint32_t                audio_rate);

/**
 * @brief srWrapperInputAttach attach new input file to the smartrender wrapper.
 *                             Must be called before any frame is passed to the encoder (srwrapper).
 * @param[in] instance         pointer to the smartrender instance.
 * @param[in] in_info          mpegin structure that has already be filled by mpegInOpen.
 * @return                     returns a combination of codes defined above.
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperInputAttach(srwrapper_tt      *instance,
                                           struct mpegInInfo *in_info);

/**
 * @brief srWrapperInputDetach detach current input file from the smartrender wrapper.
 *                             Can also be done by calling srWrapperDone.
 * @param[in] instance         pointer to the smartrender instance.
 * @return                     returns a combination of codes defined above.
 *
 * @code
 * ...
 * @endcode
 */
void MC_EXPORT_API srWrapperInputDetach(srwrapper_tt *instance);

/**
 * @brief srWrapperDone finish a smartrender session.
 * @param[in] instance  pointer to the smartrender instance.
 * @param[in] abort - 0 = finish any leftover video and clean up, else just clean up
 * @return @ref mpegOutErrNone if successful, @ref mpegOutError if not
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperDone(srwrapper_tt *instance,
                                    int32_t       abort);

/**
 * @brief srWrapperFree destroy a smartrender instance.
 * @param[in] instance  pointer to the smartrender instance.
 *
 * @code
 * ...
 * @endcode
 */
void MC_EXPORT_API srWrapperFree(srwrapper_tt *instance);

/**
 * @brief srWrapperVideoDefaults fill settings structure with default values.
 * @param[out] set               pointer to the settings structure.
 * @param[in]  video_type        type used for loading default values (e.g. MPEG_DVD)
 * @param[in]  PAL               video format, 1 = PAL, 0 = NTSC
 * @return                       returns type describing character string or NULL if video type is not supported.
 *
 * @code
 * struct mpeg_v_settings set = {0};
 *
 * srWrapperVideoDefaults(&set, MPEG_DVD, VM_PAL);
 * @endcode
 */
char * MC_EXPORT_API srWrapperVideoDefaults(struct mpeg_v_settings *set,
                                            int32_t                 video_type,
                                            int32_t                 PAL);

/**
 * @brief srWrapperVideoChkSettings check if settings are valid.
 * @param[in] get_rc
 * @param[in] set                   pointer to the settings structure.
 * @param[in] options
 * @param[in] app
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoChkSettings(void                         *(MC_EXPORT_API *get_rc)(const char* name),
                                                const struct mpeg_v_settings *set,
                                                uint32_t                      options,
                                                void                         *app);

/**
 * @brief srWrapperVideoPerformance adjust settings for quality/perfornace related purposes
 * @param[out] set                  pointer to the settings structure.
 * @param[in]  mode                 can be PERF_ONLINE, PERF_OFFLINE or PERF_MOTIONAREA
 * @param[in]  level                performance/quality level
 * @note                            this function should be called immediately before calling srWrapperVideoNew.
 *
 * @code
 * struct mpeg_v_settings set = {0};
 *
 * srWrapperVideoDefaults(&set, MPEG_DVD, VM_PAL);
 *
 * ... changing resolution etc. ...
 *
 * srWrapperVideoPerformance(&set, PERF_OFFLINE, 16);
 * @endcode
 */
void MC_EXPORT_API srWrapperVideoPerformance(struct mpeg_v_settings *set,
                                             int32_t                 mode,
                                             int32_t                 level,
                                             int32_t                 reserved);

/**
 * @brief srWrapperVideoNew create a video smartrender wrapper object. Only video is setup for
 *                          smart rendering with this function, for adding audio smart rendering
 *                          the srWrapperAudioNew function must be called.
 * @param[in] get_rc        pointer to set of callbacks used for memory allocation, printing messages etc.
 * @param[in] set           pointer to the settings structure.
 * @param[in] options       option flags
 * @param[in] cpu           cpu flags
 * @return                  returns a pointer to a video smartrender instance, used for futher functionality.
 *
 * @code
 * ...
 * srwrapper_tt *instance = srWrapperVideoNew(get_rc, &set, 0, 0xFFFFFFFF, 0, 0);
 * @endcode
 */
srwrapper_tt * MC_EXPORT_API srWrapperVideoNew(void                         *(MC_EXPORT_API *get_rc)(const char* name),
                                               const struct mpeg_v_settings *set,
                                               int32_t                       options,
                                               int32_t                       cpu,
                                               int32_t                       frame0,
                                               int32_t                       nframes);

/**
 * @brief srWrapperVideoInit initialize the smartrender wrapper for an encoding session, this
 *                           function must be called before the PutFrame or PutByte functions are called.
 *                           Only video is setup for smart rendering with this function, for initializing
 *                           audio smartrenderer the srWrapperAudioInit function must be called.
 * @param[in] instance       pointer to the smartrender instance.
 * @param[in] bs             pointer to a bufstream object.
 * @param[in] options
 * @param[in] opt_ptr
 * @return                  @ref mpegOutErrNone if successful, @ref mpegOutError or @ref mpegOutDecline if not
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoInit(srwrapper_tt  *instance,
                                         bufstream_tt  *bs,
                                         int32_t        options,
                                         void         **opt_ptr);

/**
 * @brief srWrapperVideoPutFrame pass a new video frame to the smartrenderer.
 * @param[in] instance           pointer to the smartrender instance.
 * @param[in] frame_num          if rendered is 1 and this frame is not from the source
 *                               file (i.e. this frame is being inserted between frames
 *                               of the source file), set this to -1. Else set this to
 *                               the source file frame number
 * @param[in] rendered           if 0, frame is smartrendered if possible. If 1, frame is completely new encoded.
 * @param[in] pb_src             source buffer, used if frame_num = -1
 * @param[in] stride             stride of source picture
 * @param[in] width              width of source picture
 * @param[in] height             height of source picture
 * @param[in] fourcc             colorspace of source picture (FOURCC)
 * @param[in] options            options flag
 * @param[in] opt_ptr            pointer to options data
 * @return                       @ref mpegOutErrNone if successful, @ref mpegOutError or mpegOutDecline if not
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoPutFrame(srwrapper_tt   *instance,
                                             int32_t         frame_num,
                                             int32_t         rendered,
                                             unsigned char  *pb_src,
                                             int32_t         stride,
                                             uint32_t        width,
                                             uint32_t        height,
                                             uint32_t        fourcc,
                                             uint32_t        options,
                                             void          **opt_ptr);

/**
 * @brief srWrapperVideoPut pass a new video frame to the smartrenderer (but with built-in FOURCC_BGR4 colorspace).
 * @param[in] instance      pointer to the smartrender instance.
 * @param[in] frame_num     if rendered is 1 and this frame is not from the source
 *                          file (i.e. this frame is being inserted between frames
 *                          of the source file), set this to -1. Else set this to
 *                          the source file frame number
 * @param[in] rendered      if 0, frame is smartrendered if possible. If 1, frame is completely new encoded.
 * @param[in] pb_src        source buffer, used if frame_num = -1
 * @return                  @ref mpegOutErrNone if successful, @ref mpegOutError or mpegOutDecline if not
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoPut(srwrapper_tt  *instance,
                                        int32_t        frame_num,
                                        int32_t        rendered,
                                        unsigned char *pb_src);


/**
 * @brief srWrapperVideoPutFrameV pass a new video frame to the smartrenderer (using a frame structure).
 * @param[in] instance            pointer to the smartrender instance.
 * @param[in] frame_num           if rendered is 1 and this frame is not from the source
 *                                file (i.e. this frame is being inserted between frames
 *                                of the source file), set this to -1. Else set this to
 *                                the source file frame number
 * @param[in] rendered            if 0, frame is smartrendered if possible. If 1, frame is completely new encoded.
 * @param[in] pframe_v            pointer to pframe_v_tt object
 * @return                        @ref mpegOutErrNone if successful, @ref mpegOutError or mpegOutDecline if not
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoPutFrameV(srwrapper_tt *instance,
                                              int32_t       frame_num,
                                              int32_t       rendered,
                                              pframe_v_tt   pframe_v);

/**
 * @brief srWrapperVideoGetMaxBitrate get maximum bitrate achieved during a session.
 * @param[in] instance                pointer to the smartrender instance.
 * @return                            the maximum bitrate achieved until now.
 *
 * @code
 * ...
 * @endcode
 */
int32_t MC_EXPORT_API srWrapperVideoGetMaxBitrate(srwrapper_tt *instance);

/**
 * @brief srWrapperAudioNew create an audio smartrender wrapper object or just initialize the audio part
 *                          of smartrender wrapper. Only video is setup for
 *                          smart rendering with this function, for adding audio smart rendering
 *                          the srWrapperAudioNew function must be called.
 * @param[in] get_rc        pointer to set of callbacks used for memory allocation, printing messages etc.
 * @param[in] instance      pointer to the video smartrender object, can be 0 if no video is being smartrendered.
 * @param[in] set           pointer to the settings structure.
 * @param[in] options       option flags
 * @param[in] cpu           cpu flags
 * @param[in] cpu           audio rate
 * @return                  returns a pointer to a smartrender instance, used for futher functionality.
 *
 * @code
 * ...
 * srwrapper_tt *instance = srWrapperVideoNew(get_rc, &set, 0, 0xFFFFFFFF, 0, 0);
 * @endcode
 */
srwrapper_tt * MC_EXPORT_API srWrapperAudioNew(void                         *(MC_EXPORT_API *get_rc)(const char* name),
                                               srwrapper_tt                 *instance,
                                               const struct mpeg_a_settings *m_set,
                                               int32_t                       options,
                                               int32_t                       cpu,
                                               int32_t                       audio_rate);

int32_t MC_EXPORT_API srWrapperAudioInit(srwrapper_tt *instance,
                                         bufstream_tt *bs);


void MC_EXPORT_API srWrapperAudioDefaults(struct mpeg_a_settings *set,
                                          int32_t                 audio_type);

int32_t MC_EXPORT_API srWrapperAudioChkSettings(void                         *(MC_EXPORT_API *get_rc)(const char* name),
                                                const struct mpeg_a_settings *set,
                                                int32_t                       mpeg_type,
                                                int32_t                       audio_rate,
                                                uint32_t                      options,
                                                void                         *app);

int32_t MC_EXPORT_API srWrapperAudioPutBytes(srwrapper_tt  *srwrap,
                                             uint32_t       sample,
                                             int32_t        samples,
                                             int32_t        rendered,
                                             unsigned char *pb_src,
                                             uint32_t       bytes);

/**
 * The API Extension control.
 * @param func one of the following API function identifiers:
 *    MCAPI_GetModuleInfo
 * @return pointer to the requested API function or NULL, if not supported.
 */
APIEXTFUNC MC_EXPORT_API srWrapperGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif // #if !defined (__ENC_MP2SR_API_INCLUDED__)

