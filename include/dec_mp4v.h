/**
\file   dec_mp4v.h
\brief   MPEG-4 Video Decoder API

\verbatim
File: dec_mp4v.h
Desc:  MPEG-4 Video Decoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
\endverbatim
**/

#if !defined (__DEC_MP4V_API_INCLUDED__)
#define __DEC_MP4V_API_INCLUDED__

#include "mcdefs.h"
#include "mcfourcc.h"
#include "bufstrm.h"
#include "mcapiext.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 \brief Call this function to get a bufstream interface for the mpeg4 video decoder.
 \return  The video decoder instance.
 \param[in] get_rc  callbacks to external message handler and memory manager functions
 \param[in] reserved1 reserved parameter
 \param[in] reserved2 reserved parameter
 */
bufstream_tt * MC_EXPORT_API open_mpeg4in_Video_stream_ex(void *(MC_EXPORT_API *get_rc)(const char* name), long reserved1, long reserved2);

/**
 \brief Call this function to get a bufstream interface for the mpeg4 video decoder.
 \return  The video decoder instance.
 */
bufstream_tt * MC_EXPORT_API open_mpeg4in_Video_stream(void);

/**
\brief Provides access to extended module API.
\return  pointer to requested function or NULL
\param[in] func	identifier of module extended function
 */
APIEXTFUNC     MC_EXPORT_API mpeg4in_Video_GetAPIExt(uint32_t func);

/**\name Decoder's return codes.   */
#define M4D_UNSUPPORTED  								-1	/**< \brief unsuported format \hideinitializer */
#define M4DS_OK                                           0x00000000	/**< \brief no errors \hideinitializer */
#define M4DS_FALSE                                        0x00000001	/**< \brief Unknown error \hideinitializer */
#define M4DE_FAILED                                       0x80004005	/**< \brief error during bitstream parsing \hideinitializer */
#define M4DE_NOT_SUPPORTED                                0x80004006	/**< \brief reserved \hideinitializer */
#define M4DE_PARAM_UNKNOWN                                0x80004007	/**< \brief unkown parameter \hideinitializer */
#define M4DE_PARSE                                        0x80004008	/**< \brief error during bitstream parsing \hideinitializer */
#define M4DE_INVALID_ARGUMENT                             0x80070057	/**< \brief bad arguments passed  \hideinitializer */
#define M4DE_POINTER                                      0x80004003	/**< \brief bad pointer passes \hideinitializer */
#define M4DE_OUTOFMEMORY                                  0x80004009	/**< \brief not enough memory  \hideinitializer */
#define M4D_NULL                                          0x00000000	/**< \brief no errors \hideinitializer */
/** \} */

/**\name Default values.   */
#define DEFAULT_BRIGHTNESS  128	/**< \brief Default brighteness value \hideinitializer */
#define DEFAULT_CONTRAST    128	/**< \brief Default contrast value  \hideinitializer */
/** \} */

/**\name auxinfo options   */
#define M4D_GET_DECODER_STATISTIC                         0x80001005	/**< \brief Get statistic information  in m4d_statistic_tt \hideinitializer */
/** \} */

/**\name Deinterlace modes.   */
#define M4D_DEINTERLACE_WEAVE                 0x00000000	/**< \brief Do not deinterlace – output interleaved fields \hideinitializer */
#define M4D_DEINTERLACE_VERTICAL_FILTER       0x00000001	/**< \brief Select vertical deinterlace function \hideinitializer */
#define M4D_DEINTERLACE_FIELD_INTERPOLATION   0x00000002	/**< \brief Deinterlace by interpolation \hideinitializer */
#define M4D_DEINTERLACE_AUTO                  0x00000003	/**< \brief Auto mode, if stream interlace - field interpolation   \hideinitializer */
/** \} */

/**\name Error resilince modes.   */
#define M4D_ERROR_RESILIENCE_SKIP_TILL_INTRA  0x00000000	/**< \brief In case of bit stream errors, skips all slices till next intra vop \hideinitializer */
#define M4D_ERROR_RESILIENCE_PROCEED_ANYWAY   0x00000001	/**< \brief Ignores bit stream errors \hideinitializer */
/** \} */

/**\name The types of frames that the decoder will skip.   */
#define M4D_SKIP_MODE_NONE                    0x00000000  /**< \brief  Decode all \hideinitializer */
#define M4D_SKIP_MODE_DECODE_REFERENCE        0x00000001  /**< \brief Decode I, P \hideinitializer */
#define M4D_SKIP_MODE_DECODE_INTRA            0x00000002  /**< \brief Decode I only  \hideinitializer */
#define M4D_SKIP_MODE_OBEY_QUALITY_MESSAGES   0x00000003	/**< \brief Obey quality messages (skip out of time B-frames) \hideinitializer */
/** \} */

/**\name IDCT algorithm type modes   */
#define M4D_IDCT_PRECISION_INTEGER            0x00000000 	/**< \brief optimized CPU-specific IEEE-1180 compliant integer IDCT  \hideinitializer */
#define M4D_IDCT_PRECISION_FLOAT              0x00000001 	/**< \brief double precision IDCT algorithm from the IEEE 1180 \hideinitializer */
/** \} */

/** \brief Video decoder settings structure (input to parser) */
typedef struct m4d_decoder_settings
{
    int32_t   post_process_flag;    /**< \brief apply deblock */
    int32_t   deblock_strength;		/**< \brief sets deblock intensity */
    int32_t   reserved_1[10];		/**< \brief Reserved */

    int32_t   display_osd_flag;		/**< \brief display statistic info */
    int32_t   reserved_2[10];		/**< \brief Reserved */

    int32_t   skip_mode;			/**< \brief sets skip mode */
    int32_t   error_resilience_mode;/**< \brief sets error resilience mode */
    int32_t   deinterlace_mode;		/**< \brief sets deinterlace method */
    int32_t   idct_precision_mode; 	/**< \brief allows selecting precision of inverse DCT procedure */
    int32_t   stream_order;			/**< \brief allows to switch on\off b-frames reordering in decoder */
    int32_t   reserved_3[29];		/**< \brief Reserved */

    int32_t   brightness;     		/**< \brief sets the brightness level */
    int32_t   contrast;				/**< \brief sets the contrast level */
    int32_t   reserved_4[10];		/**< \brief Reserved */

    int32_t   reserved_5[100];		/**< \brief Reserved */

} m4d_decoder_settings_tt;
/** \} */

/** \brief decoder statistic structure */
typedef struct m4d_statistic
{
    int32_t   number_frames_played;		/**< \brief Number of decoded frames */
    int32_t   number_frames_skipped;	/**< \brief Number of skipped frames */
}m4d_statistic_tt;
/** \} */

#ifdef __cplusplus
}
#endif

/** \brief Delayed color conversion for more smooth playback */
typedef struct get_pic_delayed_s {
	void * frame_obj;								/**< \brief Picture object pointer */
	void (*get_pic)(void *,frame_tt *, uint32_t);	/**< \brief Get Picture callback */
	void (*release)(void *);						/**< \brief Picture  release callback*/
} get_pic_delayed_t;
/** \} */

#endif // #if !defined (__DEC_MP4V_API_INCLUDED__)

