/** 
 @file  trans_video_framerate.h
 @brief  Frame Rate Converter main header
 
 @verbatim
 File: trans_video_framerate.h

 Desc: Frame Rate Converter main header
 
	Copyright (c) 2005-2006 Mainconcept Russia
	All rights are reserved.  Reproduction in whole or in part is prohibited
	without the written consent of the copyright owner.

	Mainconcept Russia reserves the right to make changes without
	notice at any time. Mainconcept Russia makes no warranty, expressed,
	implied or statutory, including but not limited to any implied
	warranty of merchantability of fitness for any particular purpose,
	or that the use will not infringe any third party patent, copyright
	or trademark.

	Mainconcept Russia must not be liable for any loss or damage arising
	from its use.
 @endverbatim
 **/

/////////////////////////////////////////////////////////////////////////////
// API functions
/////////////////////////////////////////////////////////////////////////////

#if !defined (__TRANS_VIDEO_FRAMERATE_API_INCLUDED__)
#define __TRANS_VIDEO_FRAMERATE_API_INCLUDED__


#include "mctypes.h"
#include "bufstrm.h"

/**
 * @brief Frame Rate Converter instance type
 */
typedef struct v_frc frc_tt;
typedef struct frc_v_settings frc_v_settings_tt;


/**
 * @name General errors
 * @{
 **/
#define FRC_ERROR_NONE     0		/**< @brief No error */
#define FRC_ERROR_FAILED   1		/**< @brief Failed */
/** @} */

/**
 * @name Sample information flags
 * @{
 **/
#define FRC_SI_OK			0		///< Time is set
#define FRC_SI_TIME_NOT_SET 1		///< Time is not set
#define FRC_SI_NO_STOP_TIME 2		///< No stop time
/** @} */


#pragma pack(push,1)

/**
 * @name Frame rate
 * @{
 **/
struct frc_frame_rate
{
	int32_t num;		///< Numerator
	int32_t denum;		///< Denumerator
};
/** @} */


/**
 * @name Sample information
 * @{
 **/
struct frc_sample_info
{
	int64_t rt_start;    ///< Start time                      
    int64_t rt_stop;	 ///< Stop time
	int32_t flag;		///< Time flag. Supported values are @ref FRC_SI_OK .. @ref FRC_SI_NO_STOP_TIME.
};
/** @} */


 /**
 * @name Frame Rate Converter settings
 * @{
 **/
struct frc_v_settings
{

    frc_frame_rate in_frame_rate;		///< Input frame rate
	frc_frame_rate out_frame_rate;		///< Output frame rate

	int32_t in_progressive;				///< Input progressive flag
	int32_t out_progressive;			///< Output progressive flag

	int32_t odd_field_first;			///< Specifies the field that is delivered first
	int32_t fd_switch;					///< Enables/disables the Line Doubler mode
	int32_t preserve_spatial_placement; ///< Enables/disables preserving field’s spatial placement
									
	int32_t width;						///< Number of pixels per row used for streams, normally this is the horizontal size
	int32_t height;						///< Number of lines used for streams, normally this is the vertical size
	int32_t in_stride;					///< Size of one line in byte in source buffers
	int32_t out_stride;					///< Size of one line in byte in destination buffers
	uint32_t fourcc;					/**< @brief Color space format. 
										* @brief The following FourCC are valid:
										* @li FOURCC_YUY2
										* @li FOURCC_YV12
										* @li FOURCC_UYVY
										* @li FOURCC_YVU9
										* @li FOURCC_YVYU
										* @li FOURCC_RGB24
										* @li FOURCC_RGB32
                                        * @li FOURCC_W010
                                        * @li FOURCC_W210
                                        * @li FOURCC_W410
										**/
	uint32_t in_invBMP;					///< Invert BMP flag for input buffer
	uint32_t out_invBMP;				///< Invert BMP flag for output buffer

    uint32_t reserved[16];		
};
/** @} */


#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Call to create a new Video Frame Rate Converter instance.
 * @param[in] get_rc the get_rc callback to provide a callback to get info / error messages.
 * @param[in] frc_set transform settings for every view + common settings.
 * @return pointer to @ref frc_tt object if successful.
 * @return NULL if unsuccessful.
 */
frc_tt * MC_EXPORT_API FRCOutVideoNew(void * (MC_EXPORT_API * get_rc)(const char * name),
                                           const struct frc_v_settings *  frc_set);


/**
 * @brief Call to initialize the Video Frame Rate Converter for a transformation session.
 * @brief Must be called before the PutFrame function.
 * @param[in] instance the Video Frame Rate Converter instance.
 * @param[in] videobs @ref bufstream object for writing data. Must be initialized before.
 * @param[in] options_flags reserved.
 * @param[in] opt_ptr reserved.
 * @return @ref FRC_ERROR_NONE if successful.
 * @return @ref FRC_ERROR_FAILED if an error occurs.
 */
int32_t MC_EXPORT_API FRCOutVideoInit(frc_tt *      instance,
                                       struct bufstream * videobs,
									   uint32_t           options_flags,
									   void **            opt_ptr);

/**
 * @brief Call to transform one video frame.
 * @param[in] instance the Video Frame Rate Converter instance.
 * @param[in] pb_src pointers to the input frame buffers.
 * @param[in] src_size size of input buffer in bytes.
 * @param[in] sample_info input sample information.
 * @param[in] options_flags reserved.
 * @param[in] opt_ptr reserved.
 * @return @ref FRC_ERROR_NONE if successful.
 * @return @ref FRC_ERROR_FAILED if an error occurs.
 */
int32_t MC_EXPORT_API FRCOutVideoPutFrame(frc_tt *   instance,
										 uint8_t *       pb_src,
										 int32_t		  src_size,
										 frc_sample_info *sample_info,
										 uint32_t        opt_flags,
										 void **         ext_info);

/**
 * @brief call to finish transform session. 
 * @param[in] instance the Video Frame Rate Converter instance.
 * @param[in] abort If abort equals 0, finish any leftover video and clean up, else just clean up.
 */
int32_t MC_EXPORT_API FRCOutVideoDone(frc_tt * instance, int32_t abort);

/**
 * @brief Destroys the Video Frame Rate Converter instance.
 * @param[in] instance the Video Frame Rate Converter instance.
 */
void MC_EXPORT_API FRCOutVideoFree(frc_tt * instance);

/**
 * @brief Call to change settings on-the-fly.
 * @brief Call this function to update video settings during transformation.
 * @param[in] instance the Video Frame Rate Converter instance.
 * @param[in] set transform settings for every view + common settings.
 * @return    @ref FRC_ERROR_NONE													 
 */
int32_t MC_EXPORT_API FRCOutVideoUpdateSettings(frc_tt *  instance,
                                                 const struct frc_v_settings * set);


#ifdef __cplusplus
}
#endif


#endif // #if !defined (__TRANS_VIDEO_FRAMERATE_API_INCLUDED__)
