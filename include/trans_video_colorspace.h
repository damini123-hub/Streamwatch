/**
 @file  trans_video_colorspace.h
 @brief Standalone Universal Color Converter API

 @verbatim
 File: trans_video_colorspace.h

 Desc: Standalone Universal Color Converter API

 Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/

#if !defined (__TRANS_VIDEO_COLORSPACE_API_INCLUDED__)
#define __TRANS_VIDEO_COLORSPACE_API_INCLUDED__

#include "mcapiext.h"
#include "mcdefs.h"

/**
  * @brief Maximum number of planes which can be used by one of the color spaces.
  * 4 = 3 + 1 where 3 - R,G,B or Y,U,V planes and 1 - A plane (alpha channel)
  */
#define UCC_MAX_NUM_PLANES	4

/// Returned error codes
typedef enum ucc_error_e{
	UCC_ERROR_OK = 0,		///< Success
	UCC_ERROR_OUTOFMEMORY,	///< Not enough memory
	UCC_ERROR_INVALIDARG,	///< One or more arguments are invalid
	UCC_ERROR_UNEXPECTED	///< Unknown error occurred
} ucc_error_t;


/// That defines color coordinates for the red, green, blue and white points in XYZ color space.
typedef enum ucc_color_primaries_e {
	UCC_COLOR_PRIMARIES_BT709_5					= 0,	//R(0.640 0.330) G(0.300 0.600) B(0.150 0.060) W(0.3127 0.3290)
														//HDTV 1125- and 1250-line systems
														//BT.709-5
														//BT.1361
														//SMPTE RP 177
	UCC_COLOR_PRIMARIES_BT470_6_System_M		= 1,	//R(0.670 0.330) G(0.210 0.710) B(0.140 0.080) W(0.3101 0.3161)
														//NTSC 1953 color television transmission standard
														//BT.470-6 System M
	UCC_COLOR_PRIMARIES_BT470_6_System_B_G		= 2,	//R(0.640 0.330) G(0.290 0.600) B(0.150 0.060) W(0.3127 0.3290)
														//BT.470-6 System B, G
														//BT.601 625-line systems
														//BT.1358 625
														//BT.1700 625 PAL and 625 SECAM
	UCC_COLOR_PRIMARIES_SMPTE_170M				= 3,	//R(0.630 0.340) G(0.310 0.595) B(0.155 0.070) W(0.3127 0.3290) BT601 525-line systems
														//BT.601-6 525
														//BT.1358 525
														//BT.1700 NTSC
														//SMPTE 170M
														//SMPTE 240M
	UCC_COLOR_PRIMARIES_Custom					= 4,	//set yours in ucc_frame_t::colorimetry
    UCC_COLOR_PRIMARIES_BT2020                  = 5     //R(0.708 0.292) G(0.170 0.797) B(0.131 0.046) W(0.3127 0.3290)
														//BT.2020
} ucc_color_primaries_t;

/// That defines gamma for conversions of XYZ color space.
typedef enum ucc_srgb_companding_type_e {
	UCC_SRGB_DEFAULT					= 0,	    // x <= 0.04045 ? x / 12.92 : pow(((x + 0.055f) / 1.055f), 2.4)
	UCC_SRGB_SIMPLE						= 1		    // Simple RGB gamma:    x ^ 2.2
} ucc_srgb_companding_type_t;

/// That defines luminance of white color for conversions of XYZ color space.
typedef enum ucc_ref_luminance_e {
	UCC_REF_LUM_48						= 0,		// SMPTE 431-1-2006
	UCC_REF_LUM_Custom					= 1			// Custom reference luminance
} ucc_ref_luminance_t;

///	This defines formula for the gamma correction that wasn't implemented yet in UCC.
typedef enum ucc_transfer_characteristics_e {
	UCC_TRANSFER_CHARACTERISTICS_BT709_5			= 0,	//V = 4.500 * L for 0.018 > L >= 0, V = 1.099 * L^0.45 ? 0.099 for 1 >= Lc >= 0.018
															//ITU-R BT.709-5
															//ITU-R Rec. BT.1361
															//ITU-R Rec. BT.601-6 525 or 625
															//ITU-R Rec. BT.1358 525 or 625
															//ITU-R Rec. BT.1700 NTSC
															//SMPTE 170M
	UCC_TRANSFER_CHARACTERISTICS_BT470_6_System_M	= 1,	//V = L ^ 1/2.2
															//ITU-R BT.470-6 System M
															//BT.1700 (2007 revision) 625 PAL and 625 SECAM
	UCC_TRANSFER_CHARACTERISTICS_BT470_6_System_B_G	= 2,	//V = L ^ 1/2.8
															//ITU-R Rec. BT.470-6 System B, G
	UCC_TRANSFER_CHARACTERISTICS_SMPTE_240M			= 3,	//V = 4.0 * L for 0.0228 > L >= 0, V = 1.1115 * L^0.45 ? 0.1115 for 1 >= L >= 0.0228
															//SMPTE 240M
	UCC_TRANSFER_CHARACTERISTICS_Linear				= 4,	//V = L
    UCC_TRANSFER_CHARACTERISTICS_BT2020             = 5,    //BT.2020, 10 bit
    UCC_TRANSFER_CHARACTERISTICS_BT2020_10BIT       = UCC_TRANSFER_CHARACTERISTICS_BT2020,
    UCC_TRANSFER_CHARACTERISTICS_BT2020_12BIT       = 6     //BT.2020, 12 bit
} ucc_transfer_characteristics_t;

/** @brief This defines coefficients for getting YUV->RGB or inverse conversion.
  *
  * This defines coefficients for getting YUV->RGB or inverse conversion.
  *
  *	Some of code:
  *
  *	YUV->RGB
  * @par
  *     float d = (float)(u-128);<br>
  *		float e = (float)(v-128);<br>
  *		*r = (uint8_t)clip(y + e*c->Krv				+ 0.5f, 0,255);<br>
  *		*g = (uint8_t)clip(y + d*c->Kgu + e*c->Kgv	+ 0.5f, 0,255);<br>
  *		*b = (uint8_t)clip(y + d*c->Kbu				+ 0.5f, 0,255);
  *
  *	RGB->YUV
  * @par
  *		float luma = c->Kyr*r + c->Kyg*g + c->Kyb*b;<br>
  *		*u = (uint8_t)clip((b - luma)*c->Nu + 128.5, 0, 255);<br>
  *		*v = (uint8_t)clip((r - luma)*c->Nv + 128.5, 0, 255);<br>
  *		*y = (uint8_t)clip(luma + 0.5f, 0, 255);
  *
  *	And in theory:
  * <table bgcolor=white>
  *	<tr><td>( Y )</td><td>( Kyr Kyg Kyb )</td><td>( R )</td></tr>
  * <tr><td>( U ) =</td><td>( Kur Kug Kub ) *</td><td>( G )</td></tr>
  *	<tr><td>( V )</td><td>( Kvr Kvg Kvb )</td><td>( B )</td></tr>
  * </table>
  * , where:
  *
  * Kur = -Kyr;<br>
  *	Kug = -Kyg;<br>
  *	Kub = 1 - Kyb;
  *
  *	Kvr = 1 - Kyr;<br>
  *	Kvg = -Kyg;<br>
  *	Kvb = -Kyb;
*/
typedef enum ucc_matrix_coefficients_e {
	UCC_MATRIX_COEFFICIENTS_BT709_5_System_1125		= 0,	//Kyr = 0.2126, Kyb = 0.0722
															//ITU-R Rec. BT.709-5 1125-lines system
															//ITU-R Rec. BT.1361
	UCC_MATRIX_COEFFICIENTS_BT709_5_System_1250		= 1,	//Kyr = 0.2991, Kyb = 0.1145
															//ITU-R Rec. BT.709-5 1250-lines system
															//ITU-R Rec. BT.470-6 System B, G
															//ITU-R Rec. BT.601-6 525 and 625
															//ITU-R Rec. BT.1358 525 and 625
															//ITU-R Rec. BT.1700 625 PAL and 625 SECAM, NTSC
															//SMPTE 170M
	UCC_MATRIX_COEFFICIENTS_SMPTE_240M				= 2,	//Kyr = 0.2124, Kyb = 0.0866
															//SMPTE 240M
	UCC_MATRIX_COEFFICIENTS_Custom					= 3,	//set yours in ucc_frame_t::colorimetry
    UCC_MATRIX_COEFFICIENTS_BT2020                  = 4,    //Kyr = 0.2627, Kyb = 0.0593
                                                            //ITU-R Rec. BT.2020 non-const luma
    UCC_MATRIX_COEFFICIENTS_BT2020_NON_CONST        = UCC_MATRIX_COEFFICIENTS_BT2020,
    UCC_MATRIX_COEFFICIENTS_BT2020_CONST            = 5,    //Kyr = 0.2627, Kyb = 0.0593
                                                            //ITU-R Rec. BT.2020 const luma
    UCC_MATRIX_COEFFICIENTS_FCC                     = 6     //Kyr = 0.30, Kyb = 0.11
                                                            //ITU-T Recomendation H.264 | ISO/IEC 14496-10 AVC
} ucc_matrix_coefficients_t;

/// Type of lowpass filter.
typedef enum ucc_lowpass_filter_e {
	UCC_FILTER_CATMULLROM	= 0,
	UCC_FILTER_LANCZOS2		= 1,
	UCC_FILTER_MITCHEL		= 2,
	UCC_FILTER_NOTCH		= 3,
	UCC_FILTER_GAUSSIAN1	= 4,
	UCC_FILTER_GAUSSIAN2	= 5
} ucc_lowpass_filter_t;



///	Type of the deinterlacing
typedef enum ucc_deinterlacing_e {
	UCC_DEINTERLACING_None = 0,
	UCC_DEINTERLACING_INTERFIELD_INTERPOLATION			= 1,///< Deinterlacing is done by blending or averaging consecutive fields.
															///< This is fine for slow motion, when the image hasn't changed between fields.

	UCC_DEINTERLACING_INTRAFIELD_INTERPOLATION_TOP		= 2,///< Deinterlacing is done by replacement of samples of bottom field with samples of top field (top field stretching).
	UCC_DEINTERLACING_INTRAFIELD_INTERPOLATION_BOTTOM	= 3	///< Deinterlacing is done by replacement of samples of top field with samples of bottom field (bottom field stretching).
} ucc_deinterlacing_t;

/*
	This feature was excepted from the trunk due to it's complexity for the SIMD optimization and his doubtful future.
*/
typedef enum ucc_dithering_e {
	UCC_DITHERING_None = 0,
	UCC_DITHERING_FLOYD_STEINBERG = 1,
	UCC_MAX_NUM_DITHERING_METHODS
}ucc_dithering_t;

/*
	This feature was implemented in older version (plain C and SIMD) but don't turned on after code refactoring.
	This is not so suitable for the UCC pipeline (pipeline use line by line conversion) and new implementation or
	turning on this feature requires knowledge of basics (and more) of UCC pipeline.
*/
typedef enum ucc_rotation_e {
	UCC_ROTATION_None= 0,
	UCC_ROTATION_90  = 1,
	UCC_ROTATION_180 = 2,
	UCC_ROTATION_270 = 3
}ucc_rotation_t;

typedef enum {
	UCC_IDX_Y		= 0,
	UCC_IDX_V		= 1,
	UCC_IDX_U		= 2,
	UCC_IDX_R		= 0,
	UCC_IDX_G		= 1,
	UCC_IDX_B		= 2,
	UCC_IDX_A		= 3,
	UCC_MAX_COMP_IDX
}ucc_component_idx_t;

/// Defines for speed/quality switch.
typedef enum {
	UCC_SQ_SWITCH_MAX_QUALITY		= 0,	///< Maximum quality
	UCC_SQ_SWITCH_MIDDLE			= 1,	///< Middle
	UCC_SQ_SWITCH_MAX_SPEED			= 2,	///< Maximum speed
}ucc_speed_quality_switch_t;

/** @brief Defines for pixel format.
  *
  *	FORMAT NAME CONSTUCTOR (instead of FOURCC usage):
  * <table bgcolor=white>
  *	<tr><td>prefix:</td><td>"PL_" for planar<br><nothing> for packed</td></tr>
  * <tr><td>sampling:</td><td>"444" or "422" or "420" or "411" or "410" or "211" or smth else<br>
  *							note that rgb formats are always 4:4:4 and sampling is not used</td></tr>
  *	<tr><td>break:</td><td>"_"</td></tr>
  *	<tr><td>samples order<br>and bitdepth for sample:</td><td>"Y" or "U" or "V" or "R" or "G" or "B" or for XYZ ("X" or "Y" or "Z")
  *							 or "X" for zeros<br>bitdepth after sample sign: <=8 (in bytes) or
  *								>8 (in words)</td></tr>
  *	<tr><td>break:</td><td>"_"</td></tr>
  *	<tr><td>bits per pixel:</td><td>(if integer)</td></tr>
  * </table>
*/
typedef enum ucc_pixel_format_e {
	UCC_PIXEL_FORMAT_444_A8Y8U8V8_32 = 0,
	UCC_PIXEL_FORMAT_AYUV = UCC_PIXEL_FORMAT_444_A8Y8U8V8_32,
	UCC_PIXEL_FORMAT_444_V8U8Y8A8_32 = 1,
	UCC_PIXEL_FORMAT_VUYA = UCC_PIXEL_FORMAT_444_V8U8Y8A8_32,
	UCC_PIXEL_FORMAT_Y410 = 2,
	UCC_PIXEL_FORMAT_Y416 = 3,
	UCC_PIXEL_FORMAT_UYVY = 4,
	UCC_PIXEL_FORMAT_YUYV = 5,
	UCC_PIXEL_FORMAT_YUY2 = UCC_PIXEL_FORMAT_YUYV,
	UCC_PIXEL_FORMAT_YVYU = 6,
	UCC_PIXEL_FORMAT_VYUY = 7,
	UCC_PIXEL_FORMAT_Y210 = 8,
	UCC_PIXEL_FORMAT_Y216 = 9,
	UCC_PIXEL_FORMAT_v210 = 10,
	UCC_PIXEL_FORMAT_Y41P = 11,
	UCC_PIXEL_FORMAT_CLJR = 12,
	UCC_PIXEL_FORMAT_Y211 = 13,
	UCC_PIXEL_FORMAT_PL_444_Y8V8U8_24 = 14,//YV24
	UCC_PIXEL_FORMAT_PL_444_Y09V09U09_48 = 15,//W409
	UCC_PIXEL_FORMAT_PL_444_Y10V10U10_48 = 16,//W410
	UCC_PIXEL_FORMAT_PL_444_Y11V11U11_48 = 17,//W411
	UCC_PIXEL_FORMAT_PL_444_Y12V12U12_48 = 18,//W412
	UCC_PIXEL_FORMAT_PL_444_Y16V16U16_48 = 19,//W416
	UCC_PIXEL_FORMAT_YV16 = 20,
	UCC_PIXEL_FORMAT_PL_422_Y09V09U09_32 = 21,//W209
	UCC_PIXEL_FORMAT_PL_422_Y10V10U10_32 = 22,//W210
	UCC_PIXEL_FORMAT_PL_422_Y11V11U11_32 = 23,//W211
	UCC_PIXEL_FORMAT_PL_422_Y12V12U12_32 = 24,//W212
	UCC_PIXEL_FORMAT_PL_422_Y16V16U16_32 = 25,//W216
	UCC_PIXEL_FORMAT_P210 = 26,
	UCC_PIXEL_FORMAT_P216 = 27,
	UCC_PIXEL_FORMAT_YV12 = 28,
	UCC_PIXEL_FORMAT_NV12 = 29,
	UCC_PIXEL_FORMAT_NV21 = 30,
	UCC_PIXEL_FORMAT_NV24 = 31,
	UCC_PIXEL_FORMAT_PL_420_Y09V09U09_24 = 32,//W009
	UCC_PIXEL_FORMAT_PL_420_Y10V10U10_24 = 33,//W010
	UCC_PIXEL_FORMAT_PL_420_Y11V11U11_24 = 34,//W011
	UCC_PIXEL_FORMAT_PL_420_Y12V12U12_24 = 35,//W012
	UCC_PIXEL_FORMAT_PL_420_Y16V16U16_24 = 36,//W016
	UCC_PIXEL_FORMAT_P010 = 37,
	UCC_PIXEL_FORMAT_P016 = 38,
	UCC_PIXEL_FORMAT_PL_410_Y8V8U8_10 = 39,//yvu410
	UCC_PIXEL_FORMAT_PL_411_Y8V8U8_12 = 40,//yvu411
	UCC_PIXEL_FORMAT_PL_211_Y8V8U8_8 = 41,//yvu211
	UCC_PIXEL_FORMAT_YVU9 = 42,
	UCC_PIXEL_FORMAT_B8G8R8A8_32 = 43,
	UCC_PIXEL_FORMAT_A8R8G8B8_32 = 44,
	UCC_PIXEL_FORMAT_B5G5R5X1_16 = 45,
	UCC_PIXEL_FORMAT_B5G6R5_16 = 46,
	UCC_PIXEL_FORMAT_R8G8B8_24 = 47,
	UCC_PIXEL_FORMAT_B8G8R8_24 = 48,
	UCC_PIXEL_FORMAT_B8G8R8X8_32 = 49,
	UCC_PIXEL_FORMAT_PL_R8G8B8_24 = 50,
	UCC_PIXEL_FORMAT_PL_R16G16B16_48 = 51,
	UCC_PIXEL_FORMAT_PL_BE_R16G16B16_48 = 52, //That format is not supported yet
	UCC_PIXEL_FORMAT_444_VUYA_FP = 53,
	UCC_PIXEL_FORMAT_BGRA_FP = 54,
	UCC_PIXEL_FORMAT_X8Y8Z8_24 = 55,
	UCC_PIXEL_FORMAT_X12Y12Z12_48 = 56,
	UCC_PIXEL_FORMAT_PL_X8Y8Z8_24 = 57,
	UCC_PIXEL_FORMAT_PL_X10Y10Z10_48 = 58,
	UCC_PIXEL_FORMAT_PL_X12Y12Z12_48 = 59,
	UCC_PIXEL_FORMAT_PL_X16Y16Z16_48 = 60,
	UCC_PIXEL_FORMAT_PL_444_R09G09B09_48 = 61,
	UCC_PIXEL_FORMAT_PL_444_R10G10B10_48 = 62,
	UCC_PIXEL_FORMAT_PL_444_R11G11B11_48 = 63,
	UCC_PIXEL_FORMAT_PL_444_R12G12B12_48 = 64,
	UCC_PIXEL_FORMAT_A2R10G10B10_32 = 65,
	UCC_PIXEL_FORMAT_A2B10G10R10_32 = 66,
	UCC_PIXEL_FORMAT_R8G8B8A8_32 = 67,
	UCC_PIXEL_FORMAT_PL_X09Y09Z09_48 = 68,
	UCC_PIXEL_FORMAT_PL_X11Y11Z11_48 = 69,
    UCC_PIXEL_FORMAT_B10G10R10X2_32 = 70,       // DPX10
    UCC_PIXEL_FORMAT_A16R16G16B16_64_BE = 71,   // ARGB 16bit, big endian. Used as b64a in Apple devices.
    UCC_PIXEL_FORMAT_B16G16R16A16_64 = 72,      // b64a, BGRA 16bit, little endian
    UCC_PIXEL_FORMAT_PL_444_R14G14B14_48 = 73,
    UCC_PIXEL_FORMAT_PL_444_Y14V14U14_48 = 74,  //W414
    UCC_PIXEL_FORMAT_PL_X14Y14Z14_48 = 75,
    UCC_PIXEL_FORMAT_PL_420_Y14V14U14_24 = 76,  //W014
    UCC_PIXEL_FORMAT_PL_422_Y14V14U14_32 =77,   //W214

	UCC_PIXEL_FORMAT_UNKNOWN,
		UCC_MAX_NUM_PIXEL_FORMATS = UCC_PIXEL_FORMAT_UNKNOWN,
		UCC_FORMAT_UNKNOWN = UCC_PIXEL_FORMAT_UNKNOWN
} ucc_pixel_format_t;

/// Frame description.
typedef struct ucc_frame_s {
	/**
	* @name Layout and geometry
	* @{
	**/
	ucc_pixel_format_t	pixel_format;					///< Pixel format (color space, layout, bit depth, sampling). One of @ref ucc_pixel_format_t.

	uint32_t    		width;							///< Frame width.
	uint32_t    		height;							///< Frame height.
	int32_t    			stride[UCC_MAX_NUM_PLANES];		///< Number of bytes from one row of pixels in memory to the next row of pixels in memory.
	uint8_t*    		plane[UCC_MAX_NUM_PLANES];		///< Pointer to frame samples.
	/** @} */

	/**
	* @name Pixels that we can use outside of frame size.
	* This feature was implemented in old version and was not turned on. Complexity is for inserting in pipeline is pretty good.
	* @{
	**/
	uint32_t			num_available_pixels_left;		///< Pixels left.
	uint32_t			num_available_pixels_right;		///< Pixels right.
	uint32_t			num_available_lines_top;		///< Pixels top.
	uint32_t			num_available_lines_bottom;		///< Pixels bottom.
	/** @} */

	/// Colorimetry
	struct colorimetry_t {
		ucc_color_primaries_t	color_primaries;	///< Chrominance coordinates of the source primaries. One of @ref ucc_color_primaries_t.

		/**
		  * @name Custom coordinates
		  * If @ref color_primaries is equal to @ref UCC_COLOR_PRIMARIES_Custom.
		  * @{
		**/
		float    coordinate_red_x;					///< X coordinate of red color.
		float    coordinate_red_y;					///< Y coordinate of red color.
		float    coordinate_green_x;				///< X coordinate of green color.
		float    coordinate_green_y;				///< Y coordinate of green color.
		float    coordinate_blue_x;					///< X coordinate of blue color.
		float    coordinate_blue_y;					///< Y coordinate of blue color.
		float    coordinate_reference_white_x;		///< X coordinate of reference white color.
		float    coordinate_reference_white_y;		///< Y coordinate of reference white color.
		/** @} */

		ucc_transfer_characteristics_t	transfer_characteristics;	///< Gamma correction function. One of @ref ucc_transfer_characteristics_t.

		ucc_matrix_coefficients_t	matrix_coefficients;	/**@brief Matrix coefficients used in deriving luminance and chrominance signals from
															  * the green, blue, and red primaries. One of @ref ucc_matrix_coefficients_t.*/

		/**
		  * @name Custom matrix
		  * If @ref matrix_coefficients is equal to @ref UCC_MATRIX_COEFFICIENTS_Custom.
		  * @{
		**/
		float		matrix_coeff_Kyr;	///< Coefficient of R signal.
		float		matrix_coeff_Kyb;	///< Coefficient of B signal.
		/** @} */

	} colorimetry;		///< Colorimetry structure

	float phase_luma[2][2];			///< Phase shift for luma.
	float phase_chroma[2][2];		///< Phase shift for chroma.

	union {
		struct {
			uint32_t	progressive_frame_flag		:1; ///< Indicates that frame is progressive frame (two fields were captured in the same time).
			uint32_t	top_field_first_flag		:1; ///< Indicates that top field was captured before bottom field. Ignored now.
			uint32_t	video_full_range_flag		:1; ///< Indicates that signals take full range.
			uint32_t    non_cachable_memory_flag	:1; /**@brief Indicates that frame is stored in non-cachable memory. Ignored now: no need,
														  * see _ntd & _td sse2 functions and their usage. */
		};
		uint32_t properties_flags;		///< Not used.
	};

	/**
	  * @name Reserved
	  * @{
	**/
	int32_t	reserved_int[512];
	void*	reserved_ptr[64];
	/** @} */

}ucc_frame_t;


/// Converter configuration
typedef struct ucc_config_s {
	union {
		struct {
			uint32_t	floating_point_arithmetic_flag		:1; ///< Indicates that calculations must be performed by floating point arithmetic (more accuracy, less speed).
			uint32_t	reordering_flag				:1; ///< Indicates that top field must be exchanged by bottom field and vice versa.
			uint32_t	disable_sse2_flag			:1; ///< Indicates that commands from SSE2 instructions set must not be used.
			uint32_t	disable_sse4_flag			:1; ///< Indicates that commands from SSE4 instructions set must not be used. Ignored now.
			uint32_t	disable_multithreading			:1; ///< Indicates that conversion must be done simultaneously.
			uint32_t	enable_phase_shift_flag			:1; ///< Ignored now.
			uint32_t	enable_user_defined_phases		:1; ///< If 0 the best phases for getting symmetrical filters used and @ref phase_luma, @ref phase_chroma is ignored.
			uint32_t	disable_neon_flag			:1; ///< Indicates that commands from NEON instructions set must not be used.
		};
		uint32_t	config_flags;		///< Not used.
	};

	ucc_lowpass_filter_t		resampling_filter;				///< Type of interpolation window. One of @ref ucc_lowpass_filter_t.
	ucc_deinterlacing_t			deinterlacing_method_idc;		///< Deinterlacing mode. One of @ref ucc_deinterlacing_t.

    int32_t			    		lumi_change_k;					///< Brightness change coefficient. The range is -100 .. 100. The value 0 means no changes will be made.

	ucc_rotation_t				rotation_idc;					///< Rotation mode. Not supported currently.

	ucc_speed_quality_switch_t	speed_quality_switch;			///< Speed/quality switch. One of @ref ucc_speed_quality_switch_t.

	int32_t flip;												///< Vertical flip mode for the output picture

	int32_t	threads_count;										///< Number of threads.

	int32_t						sharpness_coef;					///< Sharpness change coefficient. The range is -100 .. 300. The value 0 means no changes will be made.

    ucc_srgb_companding_type_t	srgb_companding_type;			///< sRGB companding index.

    ucc_ref_luminance_t         ref_lum;                        ///< Luminace of 100% white
    float                       ref_lum_custom;                 ///< 48.0(default)

    uint32_t                    black_stripes_left;             ///< Black stripes at the left
    uint32_t                    black_stripes_top;              ///< Black stripes at the top
    uint32_t                    black_stripes_right;            ///< Black stripes at the right
    uint32_t                    black_stripes_bottom;           ///< Black stripes at the bottom

	/**
	  * @name Reserved
	  * @{
	**/
	int32_t	reserved_int[504];

    void *  thread_pool;
	void*	reserved_ptr[63];
	/** @} */
}ucc_config_t;

#ifdef __cplusplus
extern "C" {
#endif
	/** @brief Call this function to create a color converter instance.
	 *
	 * Call this function to create a color converter instance.
	 * @param[in] get_rc pointer to a get resource function
	 * @return a handle to the created color converter instance if successful
     * @return NULL if unsuccessful
	 */
	void*			MC_EXPORT_API UCC_Create(void *(MC_EXPORT_API *get_rc)(const char* name));

	/** @brief Call this function to destroy the color converter instance.
	 *
	 * Call this function to destroy the color converter instance.
	 * @param[in] cc handle to the color converter
	 */
	void			MC_EXPORT_API UCC_Destroy(void *cc);

	/** @brief Call this function to perform frame conversion.
	 *
	 * Call this function to perform frame conversion.
	 * @param[in] cc handle to the color converter
	 * @param[in] frame_des pointer to source frame description
	 * @param[in] frame_src pointer to destination frame description
	 * @param[in] config common converter's settings
	 * @return error code from @ref ucc_error_t enumeration
	 */
	ucc_error_t		MC_EXPORT_API UCC_Transform(void *cc, ucc_frame_t *frame_des, ucc_frame_t *frame_src, ucc_config_t *config);

    APIEXTFUNC      MC_EXPORT_API UCC_GetAPIExt(uint32_t func);

	/** @brief Call this function to help to initialize a frame description structure.
	 *
	 * Call this function to help to initialize a frame description structure.
	 * @param[in] src pointer to @ref frame_tt structure
	 * @param[out] dst pointer to @ref ucc_frame_t structure
	 * @return error code from @ref ucc_error_t enumeration
	 */
	ucc_error_t		MC_EXPORT_API UCC_InitFrame(ucc_frame_t *dst, frame_tt *src);
#ifdef __cplusplus
}
#endif

#endif //#if !defined (__TRANS_VIDEO_COLORSPACE_API_INCLUDED__)