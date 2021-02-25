/**
 @file trans_video_imagescaler.h
 @brief Image Scaler API

 @verbatim
 File: trans_video_imagescaler.h

 Desc: Image Scaler API

	Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

	MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
	This software is protected by copyright law and international treaties.  Unauthorized
	reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/

#if !defined (__ISCALER_API_INCLUDED__)
#define __ISCALER_API_INCLUDED__

#include "scaler.h"



struct RECT_t
{
    int32_t    left;
    int32_t    top;
    int32_t    right;
    int32_t    bottom;
};

struct POINT_t
{
    int32_t  x;
    int32_t  y;
};


/**
 * @name Picture information
 * @{
 **/

 /**
   * @brief Picture information */
struct CONFIG_PICTURE
{
    uint32_t    dwFOURCCSubType; 	/**< @brief Color space format
									* @brief Color space format
									*<table>
									* <tr><th>FourCC</th><th>Description</th></tr>
									* <tr><td>@ref FOURCC_R555</td><td>RGB 555, two bytes</td></tr>
									* <tr><td>@ref FOURCC_R565</td><td>RGB 565, two bytes</td></tr>
									* <tr><td>@ref FOURCC_BGR3</td><td>RGB 4:4:4, three bytes</td></tr>
									* <tr><td>@ref FOURCC_BGR4</td><td>RGB 4:4:4, four bytes</td></tr>
									* <tr><td>@ref FOURCC_UYVY</td><td>YUV 4:2:2</td></tr>
									* <tr><td>@ref FOURCC_YUY2</td><td>YUV 4:2:2</td></tr>
									* <tr><td>@ref FOURCC_YV12</td><td>YUV 4:2:0 planar (Y->V->U)</td></tr>
									* <tr><td>@ref FOURCC_I420</td><td>YUV 4:2:0 planar (Y->U->V)</td></tr>
									* <tr><td>@ref FOURCC_YV16</td><td>YUV 4:2:2 planar (Y->U->V)</td></tr>
									* <tr><td>@ref FOURCC_IYUV</td><td>YUV 4:2:0 planar (Y->U->V)</td></tr>
									* </table>*/

    int32_t     lStride;			/**< @brief Size of one line in byte in buffer */
    int32_t     bNegativeDIB;       /**< @brief Negativ DIB flag
									* @brief If this field is set 1 then DIB is negative. This field is set 0 by default.*/

    POINT_t     pntSizePicture;		/**< @brief Picture resolution
										@brief Picture resolution. Minimum resolution is 16x16, maximum is 16384x16384*/
};
/** @} */

/**
 * @name Transformation options
 * @{
 **/

 /**
   * @brief Transformation options */
struct CONFIG_TRANSFORM
{
    int32_t    bCrop;              /**< @brief Cropping flag
									* @brief If this field is set 1 then cropping is need, if 0 then not*/

    int32_t    bResize;            /**< @brief Picture scaling flag
									* @brief If this field is set 1 then picture scaling is need, if 0 then not*/

    int32_t    bProgressive;       /**< @brief Progressive flag
									* @brief If this field is set 1 then picture is progressive, if 0 then interlaced*/

    RECT_t     rctCrop;            /**< @brief Rectangle of crop image if cropping is need */
    POINT_t    pntResizeSize;		/**< @brief Picture resolution of scaling if picture scaling is need */

    RECT_t     rctSizeOfAppendix;  /**< @brief Size of black stripes in bytes will be added top, bottom, left and right of picture */

	int32_t    unResizeType;		/**< @brief Not used */

    uint32_t   ulProcAffinityMask;	/**< @brief Not used*/

	int32_t	   iThreadsNumber;		/**< @brief Number of threads
									* @brief If this field is set 0 then number of threads is equal to the core count.<br>
									*		 Maximum number of threads is 16. If this field is more than 16 then number of threads is equal to 16.*/
	int32_t	   reserved[10];

};
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creating a new Image Scaler instance.
 * @brief Call to create a new Image Scaler instance.
 * @return pointer to Image Scaler object if successful.
 * @return NULL if unsuccessful.
 */
void * MC_EXPORT_API CreateIScaler(int32_t reserved);

/**
 * @brief Getting or setting of input picture configurations.
 * @brief Call to get or set an input configurations of a picture.
 * @param[in] Scaler pointer to Image Scaler object.
 * @param cpIn pointer to an input @ref CONFIG_PICTURE structure.
 * @param[in] act set/get action, if this parameter is 1 then getting of configuration, if 0 then setting of configuration.
 * @return 0 if successful.
 * @return 1 if unsuccessful.
 */
int32_t MC_EXPORT_API ISConfigInput(void *Scaler, CONFIG_PICTURE *cpIn, int32_t act);

/**
 * @brief Getting of output picture configurations.
 * @brief Call to get an output configurations of a picture.
 * @param[in] Scaler pointer to Image Scaler object.
 * @param[out] cpOut pointer to an output @ref CONFIG_PICTURE structure.
 * @param[in] act set/get action, this parameter should be 1, only getting of configuration.
 * @return 0 if successful.
 * @return 1 if unsuccessful.
 */
int32_t MC_EXPORT_API ISConfigOutput(void *Scaler, CONFIG_PICTURE *cpOut, int32_t act);

/**
 * @brief Getting or setting of transform options.
 * @brief Call to get or set an transform options.
 * @param[in] Scaler pointer to Image Scaler object.
 * @param ctTransform pointer to an output @ref CONFIG_TRANSFORM structure.
 * @param[in] act set/get action, if this parameter is 1 then getting of transform options, if 0 then setting of transform options.
 * @return 0 if successful.
 * @return 1 if unsuccessful.
 */
int32_t MC_EXPORT_API ISConfigTransform(void *Scaler, CONFIG_TRANSFORM *ctTransform, int32_t act);

/**
 * @brief Video frame transformation.
 * @brief Call to transform one video frame.
 * @param[in] Scaler pointer to Image Scaler object.
 * @param[in] pSource pointer to a source frame buffer.
 * @param[in] pDest pointer to a destination frame buffer.
 * @return 0 if successful.
 * @return 1 if unsuccessful.
 */
int32_t MC_EXPORT_API ISTransform(void *Scaler, uint8_t *pSource, uint8_t *pDest);

/**
 * @brief Destroying the Image Scaler object.
 * @brief Call to destroy the Image Scaler object.
 * @param[in] Scaler pointer to Image Scaler object.
 */
void MC_EXPORT_API DeleteIScaler(void *Scaler);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // __ISCALER_API_INCLUDED__

