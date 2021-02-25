/**
@file: enc_avc_def.h
@brief AVC/H.264 Encoder API defines

@verbatim
File: enc_avc_def.h
Desc: AVC/H.264 Encoder API defines

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
 **/


#if !defined (__ENC_AVC_DEF_INCLUDED__)
#define __ENC_AVC_DEF_INCLUDED__

/**
@def H264_VIDEO_TYPES
@name Video type defines
@{
**/

#define H264_BASELINE                 0        /**< @brief  Baseline Profile @hideinitializer*/
#define H264_CIF                      1        /**< @brief  similar to MPEG1 VideoCD @hideinitializer*/
#define H264_MAIN                     2        /**< @brief  similar to ISO/IEC 13818-1/2 @hideinitializer*/
#define H264_SVCD                     3        /**< @brief  similar to MPEG2 SuperVCD @hideinitializer */
#define H264_D1                       4        /**< @brief  similar to MPEG2 DVD @hideinitializer */
#define H264_HIGH                     5        /**< @brief  High Profile 1920x1080i @hideinitializer */
#define H264_DVD                      6        /**< @brief  DVD @hideinitializer */
#define H264_HD_DVD                   7        /**< @brief  HD DVD @hideinitializer */
#define H264_BD                       8        /**< @brief  Blu-ray Disc @hideinitializer */
#define H264_BD_HDMV                  9        /**< @brief  Blu-ray Disc (Main Video) @hideinitializer */
#define H264_PSP                     10        /**< @brief  Sony PSP @hideinitializer */
#define H264_HDTV_720p               11        /**< @brief  1280x720p @hideinitializer */
#define H264_HDTV_1080i              12        /**< @brief  1440x1080i @hideinitializer */
#define H264_iPOD                    13        /**< @brief  Apple iPod @hideinitializer */
#define H264_AVCHD                   14        /**< @brief  AVCHD @hideinitializer */
#define H264_iPOD_640x480            15        /**< @brief  Apple iPod 640x480 @hideinitializer */
#define H264_1SEG                    16        /**< @brief  1seg compatible @hideinitializer */
#define H264_INTRA_HIGH_10           17        /**< @brief   @hideinitializer */
#define H264_INTRA_CLASS_50          18        /**< @brief  H.264 intra frame coding (Class 50) @hideinitializer */
#define H264_INTRA_CLASS_100         19        /**< @brief  H.264 intra frame coding (Class 100) @hideinitializer */
#define H264_PSP_480x270             20        /**< @brief  Sony PSP 480x270 @hideinitializer */
#define H264_PSP_640x480             21        /**< @brief  Sony PSP 640x480 @hideinitializer */
#define H264_DIVX                    22        /**< @brief  @hideinitializer */
#define H264_FLASH_LOWRES            23        /**< @brief  Adobe Flash (Low Resolution Web Content) @hideinitializer */
#define H264_FLASH_HIGHRES           24        /**< @brief  Adobe Flash (High Resolution Web Content) @hideinitializer */
#define H264_3GP                     25        /**< @brief  3GP @hideinitializer */
#define H264_SILVERLIGHT             26        /**< @brief  Microsoft Silverlight Profile @hideinitializer */
#define H264_DVB_SD                  27        /**< @brief  ETSI TS 101 154 V1.9.1, paragraph 5.6 @hideinitializer */
#define H264_DVB_HD                  28        /**< @brief  ETSI TS 101 154 V1.9.1, paragraph 5.7 @hideinitializer */
#define H264_IPTV_480i               30        /**< @brief  IPTVFJ STD-0004 [Appendix A] @hideinitializer */
#define H264_IPTV_720p               31        /**< @brief  IPTVFJ STD-0004 [Appendix A] @hideinitializer */
#define H264_IPTV_1080i              32        /**< @brief  IPTVFJ STD-0004 [Appendix A] @hideinitializer */
#define H264_INTRA_CLASS_50_RP2027   33        /**< @brief  RP2027 H.264 intra frame coding (Class 50) @hideinitializer */
#define H264_INTRA_CLASS_100_RP2027  34        /**< @brief  RP2027 H.264 intra frame coding (Class 100) @hideinitializer */
#define H264_iPAD                    35        /**< @brief  Apple iPad/iPad2 @hideinitializer */
#define H264_PIFF                    36        /**< @brief  fragmented MP4 PIFF (as part of ISOFF), not yet supported @hideinitializer */
#define H264_IIS_SMOOTHSTREAMING     37        /**< @brief  fragmented MP4 IIS Smooth Streaming Transport Protocol, treat like H264_SILVERLIGHT for now @hideinitializer */
#define H264_RESERVED                38        /**< @brief   @hideinitializer */
#define H264_DPS_L1                  39        /**< @brief  DivX Plus Streaming Video Level 1:  400kb @hideinitializer */
#define H264_DPS_L2                  40        /**< @brief  DivX Plus Streaming Video Level 2:  600kb @hideinitializer */
#define H264_DPS_L3                  41        /**< @brief  DivX Plus Streaming Video Level 3:  800kb @hideinitializer */
#define H264_DPS_L4                  42        /**< @brief  DivX Plus Streaming Video Level 4: 1200kb @hideinitializer */
#define H264_DPS_L5                  43        /**< @brief  DivX Plus Streaming Video Level 5: 1750kb @hideinitializer */
#define H264_DPS_L6                  44        /**< @brief  DivX Plus Streaming Video Level 6: 2400kb @hideinitializer */
#define H264_DPS_L7                  45        /**< @brief  DivX Plus Streaming Video Level 7: 3500kb @hideinitializer */
#define H264_DPS_L8                  46        /**< @brief  DivX Plus Streaming Video Level 8: 5300kb @hideinitializer */
#define H264_DPS_L9                  47        /**< @brief  DivX Plus Streaming Video Level 9: 8400kb @hideinitializer */
#define H264_DPS_TPT                 48        /**< @brief  DivX Plus Streaming Video TPT @hideinitializer */
#define H264_DPS_HEMT_L1             49        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 1: 125kb @hideinitializer */
#define H264_DPS_HEMT_L2             50        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 2: 275kb @hideinitializer */
#define H264_DPS_HEMT_L3             51        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 3: same as MCPROFILE_DPS_V_L1 @hideinitializer */
#define H264_DPS_HEMT_L4             52        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 4: same as MCPROFILE_DPS_V_L2 @hideinitializer */
#define H264_DPS_HEMT_L5             53        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 5: same as MCPROFILE_DPS_V_L3 @hideinitializer */
#define H264_DPS_HEMT_L6             54        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 6: same as MCPROFILE_DPS_V_L4 @hideinitializer */
#define H264_DPS_HEMT_L7             55        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 7: same as MCPROFILE_DPS_V_L5 @hideinitializer */
#define H264_DPS_HEMT_L8             56        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 8: same as MCPROFILE_DPS_V_L6 @hideinitializer */
#define H264_DPS_HEMT_L9             57        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 9: same as MCPROFILE_DPS_V_L7 @hideinitializer */
#define H264_DPS_HEMT_L10            58        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 10: same as MCPROFILE_DPS_V_L8 @hideinitializer */
#define H264_DPS_HEMT_L11            59        /**< @brief  DivX Plus Streaming Mobile HEMT Video Level 11: same as MCPROFILE_DPS_V_L9 @hideinitializer */
#define H264_DPS_LEM_L1              60        /**< @brief  DivX Plus Streaming Mobile LEM Video Level 1: 175kb @hideinitializer */
#define H264_DPS_LEM_L2              61        /**< @brief  DivX Plus Streaming Mobile LEM Video Level 2: 300kb @hideinitializer */
#define H264_DPS_LEM_L3              62        /**< @brief  DivX Plus Streaming Mobile LEM Video Level 3: 550kb @hideinitializer */
#define H264_HD_DVD_BD_MV            64        /**< @brief  Redundant HD DVD & BD MV (external script needed to convert to target application) @hideinitializer */
#define H264_ULTRAVIOLET_PD_L1       65        /**< @brief  fragmented MP4 CFF (DECE/UV), portable definition 320x180, 768kb @hideinitializer */
#define H264_ULTRAVIOLET_PD_L2       66        /**< @brief  fragmented MP4 CFF (DECE/UV), portable definition 320x240, 768kb @hideinitializer */
#define H264_ULTRAVIOLET_PD_L3       67        /**< @brief  fragmented MP4 CFF (DECE/UV), portable definition 416x240, 768kb @hideinitializer */
#define H264_ULTRAVIOLET_SD_L1       68        /**< @brief  fragmented MP4 CFF (DECE/UV), standard definition 640x480, 10Mb @hideinitializer */
#define H264_ULTRAVIOLET_SD_L2       69        /**< @brief  fragmented MP4 CFF (DECE/UV), standard definition 854x480, 10Mb @hideinitializer */
#define H264_ULTRAVIOLET_HD_L1       70        /**< @brief  fragmented MP4 CFF (DECE/UV), high definition 1280x720, 25Mb @hideinitializer */
#define H264_ULTRAVIOLET_HD_L2       71        /**< @brief  fragmented MP4 CFF (DECE/UV), high definition 1920x1080, 25Mb @hideinitializer */
#define H264_INTRA_CLASS_200_RP2027  72        /**< @brief  RP2027-2012 H.264 intra frame coding (Class 200) @hideinitializer */
#define H264_XAVC_INTRA_CBG          73        /**< @brief  Obsolete, will be remapped to @ref H264_XAVC_HD_INTRA_CLASS_100_CBG @hideinitializer */
#define H264_XAVC_INTRA_VBR          74        /**< @brief  Obsolete, will be remapped to @ref H264_XAVC_4K_INTRA_CLASS_300_VBR @hideinitializer */
#define H264_AVCHD_20                75        /**< @brief  AVCHD 2.0 1080/50p 1080/60p @hideinitializer */
#define H264_DASH264                 76        /**< @brief  DASH264 @hideinitializer */
#define H264_XAVC_LONG_GOP           77        /**< @brief  Obsolete, will be remapped to @ref H264_XAVC_4K @hideinitializer */
#define H264_LONG_GOP_422_CLASS_G50  78        /**< @brief  Panasonic AVC-LongG 4:2:2 Classes G50 (50 Mbps) @hideinitializer */
#define H264_LONG_GOP_422_CLASS_G25  79        /**< @brief  Panasonic AVC-LongG 4:2:2 Classes G25 (25 Mbps) @hideinitializer */
#define H264_LONG_GOP_420_CLASS_G12  80        /**< @brief  Panasonic AVC-LongG 4:2:0 Classes G12 (12 Mbps) @hideinitializer */
#define H264_LONG_GOP_420_CLASS_G6   81        /**< @brief  Panasonic AVC-LongG 4:2:0 Classes G6 (6 Mbps) @hideinitializer */
#define H264_INTRA_CLASS_200         82        /**< @brief  H.264 intra frame coding (Class 200) @hideinitializer */
#define H264_XAVC_4K                      83   /**< @brief  SONY XAVC Long GOP 4K Profile for M4 and XD Style (MP4 and MXF file formats) @hideinitializer */
#define H264_XAVC_HD_MP4                  84   /**< @brief  SONY XAVC Long GOP HD Profile for M4 Style (MP4 file format) @hideinitializer */
#define H264_XAVC_HD_MXF                  85   /**< @brief  SONY XAVC Long GOP HD Profile for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_HD_INTRA_VBR            86   /**< @brief  SONY XAVC HD Intra VBR Profile for M4 Style (MP4 file format) @hideinitializer */
#define H264_XAVC_HD_INTRA_CLASS_50_CBG   87   /**< @brief  SONY XAVC HD Intra CBG Profile Class 50 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_HD_INTRA_CLASS_100_CBG  88   /**< @brief  SONY XAVC HD Intra CBG Profile Class 100 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_HD_INTRA_CLASS_200_CBG  89   /**< @brief  SONY XAVC HD Intra CBG Profile Class 200 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_100_CBG  90   /**< @brief  SONY XAVC 4K Intra CBG Class 100 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_300_CBG  91   /**< @brief  SONY XAVC 4K Intra CBG Class 300 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_480_CBG  92   /**< @brief  SONY XAVC 4K Intra CBG Class 480 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_100_VBR  93   /**< @brief  SONY XAVC 4K Intra VBR Class 100 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_300_VBR  94   /**< @brief  SONY XAVC 4K Intra VBR Class 300 for XD Style (MXF file format) @hideinitializer */
#define H264_XAVC_4K_INTRA_CLASS_480_VBR  95   /**< @brief  SONY XAVC 4K Intra VBR Class 480 for XD Style (MXF file format) @hideinitializer */
#define H264_HLS_CELL_L1                  96   /**< @brief  HTTP Live Streaming over cellular network       Level 1:  200kb @hideinitializer */
#define H264_HLS_CELL_L2                  97   /**< @brief  HTTP Live Streaming over cellular network       Level 2:  400kb @hideinitializer */
#define H264_HLS_CELL_L3                  98   /**< @brief  HTTP Live Streaming over cellular/Wi-Fi network Level 3:  600kb @hideinitializer */
#define H264_HLS_WIFI_L1                  99   /**< @brief  HTTP Live Streaming over Wi-Fi network          Level 1: 1200kb @hideinitializer */
#define H264_HLS_WIFI_L2                 100   /**< @brief  HTTP Live Streaming over Wi-Fi network          Level 2: 3500kb @hideinitializer */
#define H264_HLS_WIFI_L3                 101   /**< @brief  HTTP Live Streaming over Wi-Fi network          Level 3: 5000kb @hideinitializer */
#define H264_HLS_WIFI_L4                 102   /**< @brief  HTTP Live Streaming over Wi-Fi network          Level 4: 6500kb @hideinitializer */
#define H264_HLS_WIFI_L5                 103   /**< @brief  HTTP Live Streaming over Wi-Fi network          Level 5: 8500kb @hideinitializer */

#define H264_LAST_H264_TYPE              104   /**< @brief  @hideinitializer */
/** @}*/

/**
@def VSCD_MODES
@name Scene change detection modes
@{
**/
#define VCSD_MODE_OFF        0        /**< @brief  no scene detection @hideinitializer */
#define VCSD_MODE_IDR        1        /**< @brief  set IDR on scene change @hideinitializer */
/** @}*/

/**
@def PULLDOWN_FLAGS
@name Pulldown flags
@{
**/
#define VIDEO_PULLDOWN_NONE  0        /**< @brief  no pulldown @hideinitializer */
#define VIDEO_PULLDOWN_23    1        /**< @brief  23.976/24 played as 29.97/30 @hideinitializer */
#define VIDEO_PULLDOWN_32    2        /**< @brief  23.976/24 played as 29.97/30 @hideinitializer **/
#define VIDEO_PULLDOWN_23_P  4        /**< @brief  23.976/24 played as 59.94/60 @hideinitializer */
#define VIDEO_PULLDOWN_32_P  5        /**< @brief  23.976/24 played as 59.94/60 @hideinitializer */
#define VIDEO_PULLDOWN_AUTO  6        /**< @brief  adaptive mode, use OPT_EXT_PARAM_DISPLAY_MODE to specify display mode for every frame @hideinitializer */
#define VIDEO_PULLDOWN_22    7        /**< @brief  every frame is displayed twice @hideinitializer */
#define VIDEO_PULLDOWN_33    8        /**< @brief  every frame is displayed three times @hideinitializer */
/** @}*/

/**
@def H264_PROFILE_TYPES
@name Profile type defines
@{
**/
#define H264PROFILE_BASELINE                                0   /**< @brief  Baseline Profile @hideinitializer */
#define H264PROFILE_MAIN                                    1   /**< @brief  Main Profile @hideinitializer */
#define H264PROFILE_EXTENDED                                2   /**< @brief  Extended Profile (not supported by now) @hideinitializer */
#define H264PROFILE_HIGH                                    3   /**< @brief  High Profile @hideinitializer */
#define H264PROFILE_HIGH_10                                 4   /**< @brief  High 10 Profile @hideinitializer */
#define H264PROFILE_HIGH_422                                5   /**< @brief  High 4:2:2 Profile @hideinitializer */
#define H264PROFILE_HIGH_444                                6   /**< @brief  NOT USED FOR NOW @hideinitializer */
/** @}*/

/**
@def CHROMA_FORMATS
@name Chroma format defines
@{
**/
#define H264_CHROMA_400                                     1   /**< @brief   currently not supported @hideinitializer */
#define H264_CHROMA_420                                     2   /**< @brief   4:2:0 colour sampling @hideinitializer */
#define H264_CHROMA_422                                     3   /**< @brief   4:2:2 colour sampling @hideinitializer */
#define H264_CHROMA_444                                     4   /**< @brief   4:4:4 colour sampling @hideinitializer */
/** @}*/

/**
@def STREAM_TYPES
@name Stream types defines
@{
**/
#define H264_STREAM_TYPE_I                                  0   /**< @brief  VCL NALUs + filler data @hideinitializer */
#define H264_STREAM_TYPE_I_SEI                              1   /**< @brief  VCL NALUs + filler data + SEI messages @hideinitializer */
#define H264_STREAM_TYPE_II                                 2   /**< @brief  all NALU types @hideinitializer */
#define H264_STREAM_TYPE_II_NO_SEI                          3   /**< @brief  all NALU types except SEI @hideinitializer */
/** @}*/

/**
@def VUI_CONFIG
@name VUI configuration flags
@{
**/
#define H264_VUI_PRESENTATION_AUTO                 0x00000000   /**< @brief 0x000 - Configures the video usability information appearing in the SPS header automatically (depending on the other encoder settings)  @hideinitializer */
#define H264_VUI_PRESENTATION_CUSTOMIZE            0x00000001   /**< @brief 0x001 - Enable combination of the following flags for configure the video usability information appearing in the SPS header@hideinitializer */
#define H264_VUI_ASPECT_RATIO_INFO_PRESENT_FLAG    0x00000002   /**< @brief 0x002 - aspect_ratio_info_present_flag @hideinitializer */
#define H264_VUI_OVERSCAN_INFO_PRESENT_FLAG        0x00000004   /**< @brief 0x004 - overscan_info_present_flag @hideinitializer */
#define H264_VUI_VIDEO_SIGNAL_TYPE_PRESENT_FLAG    0x00000008   /**< @brief 0x008 - video_signal_type_present_flag @hideinitializer */
#define H264_VUI_COLOUR_DESCRIPTION_PRESENT_FLAG   0x00000010   /**< @brief 0x010 - colour_description_present_flag (video_signal_type_present_flag should be enabled as well) @hideinitializer */
#define H264_VUI_CHROMA_LOC_INFO_PRESENT_FLAG      0x00000020   /**< @brief 0x020 - chroma_loc_info_present_flag (not supported) @hideinitializer */
#define H264_VUI_TIMING_INFO_PRESENT_FLAG          0x00000040   /**< @brief 0x040 - timing_info_present_flag @hideinitializer */
#define H264_VUI_NAL_HRD_PARAMETERS_PRESENT_FLAG   0x00000080   /**< @brief 0x080 - nal_hrd_parameters_present_flag @hideinitializer */
#define H264_VUI_VCL_HRD_PARAMETERS_PRESENT_FLAG   0x00000100   /**< @brief 0x100 - vcl_hrd_parameters_present_flag @hideinitializer */
#define H264_VUI_PIC_STRUCT_PRESENT_FLAG           0x00000200   /**< @brief 0x200 - pic_struct_present_flag @hideinitializer */
#define H264_VUI_BISTREAM_RESTRICTION_FLAG         0x00000400   /**< @brief 0x400 - bitstream_restriction_flag @hideinitializer */
/** @}*/

/**
@def VIDEO_STANDARDS
@name Video standard defines
@{
**/
#define H264_NTSC                                           0   /**< @brief   NTSC format @hideinitializer */
#define H264_PAL                                            1   /**< @brief   PAL format @hideinitializer */
/** @}*/

/**
@def VIDEO_FORMATS
@name Video format defines
@{
**/
#define H264_VF_AUTO                                        -1   /**< @brief   @hideinitializer */
#define H264_VF_COMPONENT                                   0   /**< @brief   @hideinitializer */
#define H264_VF_PAL                                         1   /**< @brief   @hideinitializer */
#define H264_VF_NTSC                                        2   /**< @brief   @hideinitializer */
#define H264_VF_SECAM                                       3   /**< @brief   @hideinitializer */
#define H264_VF_MAC                                         4   /**< @brief   @hideinitializer */
#define H264_VF_UNSPECIFIED                                 5   /**< @brief   @hideinitializer */
/** @}*/

/**
@def INTERLACED_MODES
@name Interlaced mode defines
@{
**/
#define H264_PROGRESSIVE                                    0   /**< @brief  progressive */
#define H264_INTERLACED                                     1   /**< @brief  interlaced - encode every frame as 2 field pictures @hideinitializer */
#define H264_MBAFF                                          2   /**< @brief  interlaced - macroblock-adaptive frame-field coding @hideinitializer */
/** @}*/

/**
@def FRAME_MBS
@name frame_mbs_only_flag defines
@{
**/
#define H264_FRAME_MBS_ON                                   0   /**< @brief  for progressive encodes (frame_mbs_only_flag = 1) @hideinitializer */
#define H264_FRAME_MBS_OFF                                  1   /**< @brief  for interlaced encodes (frame_mbs_only_flag = 0) @hideinitializer */
/** @}*/

/**
@def FIELD_ORDER
@name Field order defines
**/
#define H264_TOPFIELD_FIRST                                 0   /**< @brief   @hideinitializer */
#define H264_BOTTOMFIELD_FIRST                              1   /**< @brief   @hideinitializer */
/** @}*/

/**
@def RATE_CONTROL_MODES
@name Bit rate control mode defines
**/
#define H264_CBR                                            0   /**< @brief  Constant bit rate @hideinitializer */
#define H264_CQT                                            1   /**< @brief  Constant quantization @hideinitializer */
#define H264_VBR                                            2   /**< @brief  Variable bit rate @hideinitializer */
#define H264_TQM                                            3   /**< @brief  Target quality mode @hideinitializer */
#define H264_INTRARC                                        4   /**< @brief  Special mode for Intra-only presets @hideinitializer */
/** @}*/

/**
@def HRD_BUFFER_UNITS
@name HRD bufer units defines
@{
**/
#define H264_HRD_UNIT_BYTE_PERCENT                          0   /**< @brief   @hideinitializer */
#define H264_HRD_UNIT_BIT                                   1   /**< @brief   @hideinitializer */
#define H264_HRD_UNIT_BIT_90K                               2   /**< @brief   @hideinitializer */
/** @}*/

/**
@def ADAPTIVE_QUANTIZATION_MODES
@name Adaptive quantization mode defines
@{
**/
#define H264_AQUANT_MODE_BRIGHTNESS                         0   /**< @brief   @hideinitializer */
#define H264_AQUANT_MODE_CONTRAST                           1   /**< @brief   @hideinitializer */
#define H264_AQUANT_MODE_COMPLEXITY                         2   /**< @brief   @hideinitializer */
/** @}*/

/**
@def VBV_UNITS
@name VBV units defines
@{
**/
#define H264_VBV_Units_Percents                             0   /**< @brief   @hideinitializer */
#define H264_VBV_Units_Bits                                 1   /**< @brief   @hideinitializer */
#define H264_VBV_Units_Time                                 2   /**< @brief   @hideinitializer */
/** @}*/

/**
@def ENTROPY_CODING
@name Entropy coding mode defines
@{
**/
#define H264_CAVLC                                          0 /**< @brief  Contex-adaptive variable-length coding @hideinitializer */
#define H264_CABAC                                          1 /**< @brief  Contex-adaptive binary arithmetic coding @hideinitializer */
/** @}*/

/**
@def CPU_TYPE
@name CPU type defines
@{
**/
#define H264_CPU_OPT_AUTO                                   0   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_UNKNOWN                                1   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_MMX                                    2   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_MMX_EXT                                3   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_SSE                                    4   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_SSE2                                   5   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_SSE3                                   6   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_SSE4                                   7   /**< @brief   @hideinitializer */
#define H264_CPU_OPT_AVX                                    8   /**< @brief   @hideinitializer */
/** @}*/

/**
@def INTER_SEARCH
@name Inter search defines
@{
**/
#define H264_INTERSEARCH_16x16  0   /**< @brief   @hideinitializer */
#define H264_INTERSEARCH_8x8    1   /**< @brief   @hideinitializer */
/** @}*/

/**
@def SUBPEL
@name Subpixel mode defines
@{
**/
#define H264_FULL_PEL           0                             /**< @brief  only full pixel position will be examined @hideinitializer */
#define H264_HALF_PEL           1                             /**< @brief  half-pixels positions will be added to the search @hideinitializer */
#define H264_QUARTER_PEL        2                             /**< @brief  both half and quarter pixel positions will be added @hideinitializer */
/** @}*/

/**
@def TIME_STAMP
@name Time stamp
@{
**/
#define H264_TIMESTAMP_100NS    0                             /**< @brief  DirectShow style, 100 ns units (10 Mhz clock) @hideinitializer */
#define H264_TIMESTAMP_27MHZ    1                             /**< @brief  MPEG style, 27 Mhz clock @hideinitializer */
/** @}*/

/**
@def DISPLAY_MODE
@name Display mode defines
**/
#define H264_DISPLAY_FRAME      0                             /**< @brief  display as progressive frame @hideinitializer */
#define H264_DISPLAY_TF         1                             /**< @brief  top field @hideinitializer */
#define H264_DISPLAY_BF         2                             /**< @brief  bottom field @hideinitializer */
#define H264_DISPLAY_TF_BF      3                             /**< @brief  top field, bottom field @hideinitializer */
#define H264_DISPLAY_BF_TF      4                             /**< @brief  bottom field, top field @hideinitializer */
#define H264_DISPLAY_TF_BF_TF   5                             /**< @brief  top field, bottom field, top field @hideinitializer */
#define H264_DISPLAY_BF_TF_BF   6                             /**< @brief  bottom field, top field, bottom field @hideinitializer */
#define H264_DISPLAY_DOUBLE     7                             /**< @brief  display frame twice @hideinitializer */
#define H264_DISPLAY_TRIPLE     8                             /**< @brief  display frame three times @hideinitializer */
/** @}*/

/**
@def USER_DATA_TYPES
@name User data type defines (h264_user_data_tt's type)
@{
**/
#define H264_UD_REGISTERED      1                             /**< @brief  user data registered by ITU-T Rec. T.35 SEI message @hideinitializer */
#define H264_UD_UNREGISTERED    2                             /**< @brief  user data unregistered SEI message @hideinitializer */
/** @}*/

/**
@def AIR_MODES
@name Adaptive intra refresh modes
@{
**/
#define H264_AIR_OFF            0                             /**< @brief  do not use AIR (default) @hideinitializer */
#define H264_AIR_SLOW           1                             /**< @brief  1 (SD) or 2 (HD) rows of intra macroblocks in every P frame @hideinitializer */
#define H264_AIR_MEDIUM         2                             /**< @brief  2 (SD) or 4 (HD) rows of intra macroblocks in every P frame @hideinitializer */
#define H264_AIR_FAST           3                             /**< @brief  3 (SD) or 6 (HD) rows of intra macroblocks in every P frame @hideinitializer */
#define H264_AIR_SPLIT          4                             /**< @brief Adjust intra_line_offset for this mode  */
/** @}*/

////////// defines for h264 conformance check

/**
@def CHK_OPTIONS
@name Options for the XXXChkSettings functions
@{
**/
#define H264_CHECK_AND_ADJUST   0x00000001   /**< @brief   @hideinitializer */
#define H264_CHECK_FOR_LEVEL    0x00000002   /**< @brief   @hideinitializer */
#define H264_CHECK_GET_MEMSIZE	0x00000004
/** @}*/

/**
* @name Deinterlacing modes
* Possible values of h264_v_settings::deinterlacing_mode field
* @{
*/
#define H264_DEINTERLACE_NONE            0 /**<@brief Disable deinterlacing @hideinitializer*/
#define H264_DEINTERLACE_ON              1 /**<@brief Enable deinterlacing @hideinitializer*/
#define H264_DEINTERLACE_AUTO            1 /**<@brief Enable deinterlacing use first field interpolate the other @hideinitializer*/
#define H264_DEINTERLACE_USETOPFIELD     2 /**<@brief Enable deinterlacing use top field @hideinitializer*/
#define H264_DEINTERLACE_USEBOTTOMFIELD  3 /**<@brief Enable deinterlacing use bottom field @hideinitializer*/
#define H264_DEINTERLACE_USEBOTHFIELDS   4 /**<@brief Enable deinterlacing blend fields @hideinitializer*/
/**@}*/

/**
@def PERFORMANCE_OPTIONS
@name Options for the h264OutVideoPerformance function
@{
**/
#define H264_PERF_FASTEST       0                             /**< @brief  fastest @hideinitializer */
#define H264_PERF_BALANCED      9                             /**< @brief  recommended default value @hideinitializer */
#define H264_PERF_BEST_Q        15                            /**< @brief  best quality @hideinitializer */
/** @}*/

// options for the h264CudaOutVideoPerformance function
#define H264_GPU_PERF_FASTEST   0
#define H264_GPU_PERF_FAST      4
#define H264_GPU_PERF_QUALITY   8
#define H264_GPU_PERF_BEST_Q    12

/**
@def FPAM_MODES
@name Defines for Frame packing arrangement mode definition
@{
**/
#define H264_FPAM_DONT_WRITE            0                   /**< @brief Don`t write frame packing arrangement SEI messages */
#define H264_FPAM_CHECKBOARD            1                   /**< @brief Checkboard */
#define H264_FPAM_COLUMN_INTERLEAVING   2                   /**< @brief Column based interleaving */
#define H264_FPAM_ROW_INTERLEAVING      3                   /**< @brief Row based interleaving */
#define H264_FPAM_SIDE_BY_SIDE          4                   /**< @brief Side-by-Side packing arrangement */
#define H264_FPAM_TOP_BOTTOM            5                   /**< @brief Top-Bottom packing arrangement */
/** @}*/

/**
@def PRODUCT_INFO
@name Defines for Stream creation product info
**/
#define H264_NO_MESSAGE_IN_SEI			0
#define H264_LL_MESSAGE_IN_SEI			1
#define H264_DS_MESSAGE_IN_SEI			2

/** @}*/

/**
@def ERRORS
@name ERROR_defines
@{
**/
////////// general
#define H264ERROR_NONE                                  0   /**< @brief   @hideinitializer No error*/
#define H264ERROR_FAILED                                1   /**< @brief   @hideinitializer General error*/
#define H264ERROR_NOT_SUPPORTED                         2   /**< @brief   @hideinitializer Attempt to use an unsupported function*/
#define H264ERROR_PARAM_UNKNOWN                         3   /**< @brief   @hideinitializer Unknown parameter*/
#define H264ERROR_PARAM_ADJUSTED                        4   /**< @brief   @hideinitializer Parameter adjusted by validator*/
#define H264ERROR_AVC_INTRA_SKIPPED_FRAME               5   /**< @brief   @hideinitializer */

////////// parameter set
#define H264ERROR_PARAM_SET                             10000   								/**< @brief @hideinitializer */
#define H264ERROR_PROFILE_ID                            H264ERROR_PARAM_SET + 1                 /**< @brief @hideinitializer */
#define H264ERROR_LEVEL_ID                              H264ERROR_PARAM_SET + 2                 /**< @brief @hideinitializer */
#define H264ERROR_IDR_INTERVAL                          H264ERROR_PARAM_SET + 3                 /**< @brief @hideinitializer */
#define H264ERROR_REORDERING_DELAY                      H264ERROR_PARAM_SET + 4                 /**< @brief @hideinitializer */
#define H264ERROR_USE_B_SLICES                          H264ERROR_PARAM_SET + 5                 /**< @brief @hideinitializer */
#define H264ERROR_INTERLACE_MODE                        H264ERROR_PARAM_SET + 6                 /**< @brief @hideinitializer */
#define H264ERROR_DEF_HORIZONTAL_SIZE                   H264ERROR_PARAM_SET + 7                 /**< @brief @hideinitializer */
#define H264ERROR_DEF_VERTICAL_SIZE                     H264ERROR_PARAM_SET + 8                 /**< @brief @hideinitializer */
#define H264ERROR_FRAME_RATE                            H264ERROR_PARAM_SET + 9                 /**< @brief @hideinitializer */
#define H264ERROR_NUM_REFERENCE_FRAMES                  H264ERROR_PARAM_SET + 10                 /**< @brief @hideinitializer */
#define H264ERROR_SEARCH_RANGE                          H264ERROR_PARAM_SET + 11                 /**< @brief @hideinitializer */
#define H264ERROR_RD_OPTIMIZATION                       H264ERROR_PARAM_SET + 12                 /**< @brief @hideinitializer */
#define H264ERROR_MAX_L0_ACTIVE                         H264ERROR_PARAM_SET + 13                 /**< @brief @hideinitializer */
#define H264ERROR_MAX_L1_ACTIVE                         H264ERROR_PARAM_SET + 14                 /**< @brief @hideinitializer */
#define H264ERROR_QUANT_PI                              H264ERROR_PARAM_SET + 15                 /**< @brief @hideinitializer */
#define H264ERROR_QUANT_PP                              H264ERROR_PARAM_SET + 16                 /**< @brief @hideinitializer */
#define H264ERROR_QUANT_PB                              H264ERROR_PARAM_SET + 17                 /**< @brief @hideinitializer */
#define H264ERROR_BIT_RATE_MODE                         H264ERROR_PARAM_SET + 18                 /**< @brief @hideinitializer */
#define H264ERROR_BIT_RATE_BUFFER_SIZE                  H264ERROR_PARAM_SET + 19                 /**< @brief @hideinitializer */
#define H264ERROR_BIT_RATE                              H264ERROR_PARAM_SET + 20                 /**< @brief @hideinitializer */
#define H264ERROR_MAX_BIT_RATE                          H264ERROR_PARAM_SET + 21                 /**< @brief @hideinitializer */
#define H264ERROR_INTER_SEARCH_SHAPE                    H264ERROR_PARAM_SET + 22                 /**< @brief @hideinitializer */
#define H264ERROR_ENTROPY_CODING_MODE                   H264ERROR_PARAM_SET + 23                 /**< @brief @hideinitializer */
#define H264ERROR_USE_HADAMARD_TRANSFORM                H264ERROR_PARAM_SET + 24                 /**< @brief @hideinitializer */
#define H264ERROR_SAR_WIDTH                             H264ERROR_PARAM_SET + 25                 /**< @brief @hideinitializer */
#define H264ERROR_SAR_HEIGHT                            H264ERROR_PARAM_SET + 26                 /**< @brief @hideinitializer */
#define H264ERROR_VIDEO_FORMAT                          H264ERROR_PARAM_SET + 27                 /**< @brief @hideinitializer */
#define H264ERROR_VIDEO_FULL_RANGE                      H264ERROR_PARAM_SET + 28                 /**< @brief @hideinitializer */
#define H264ERROR_NUM_UNITS_IN_TICK                     H264ERROR_PARAM_SET + 29                 /**< @brief @hideinitializer */
#define H264ERROR_TIME_SCALE                            H264ERROR_PARAM_SET + 30                 /**< @brief @hideinitializer */
#define H264ERROR_VBV_FULLNESS                          H264ERROR_PARAM_SET + 31                 /**< @brief @hideinitializer */
#define H264ERROR_VBV_FULLNESS_TRG                      H264ERROR_PARAM_SET + 32                 /**< @brief @hideinitializer */
#define H264ERROR_VBV_UNITS                             H264ERROR_PARAM_SET + 33                 /**< @brief @hideinitializer */
#define H264ERROR_PIXEL_RANGE                           H264ERROR_PARAM_SET + 34                 /**< @brief @hideinitializer */
#define H264ERROR_DEINTERLACING_MODE                    H264ERROR_PARAM_SET + 35                 /**< @brief @hideinitializer */
/////// reserved for 30 parameters
#define H264ERROR_USE_DEBLOCKING_FILTER                 H264ERROR_PARAM_SET + 64                 /**< @brief @hideinitializer */
#define H264ERROR_DEBLOCKING_ALPHAC0_OFFSET             H264ERROR_PARAM_SET + 65                 /**< @brief @hideinitializer */
#define H264ERROR_DEBLOCKING_BETA_OFFSET                H264ERROR_PARAM_SET + 66                 /**< @brief @hideinitializer */
/////// reserved for 384 parameters
#define H264ERROR_VIDEO_TYPE                            H264ERROR_PARAM_SET + 451                /**< @brief @hideinitializer */
#define H264ERROR_VIDEO_PULLDOWN                        H264ERROR_PARAM_SET + 452                /**< @brief @hideinitializer */
/////// reserved for 100 parameters
#define H264ERROR_STREAM_TYPE                           H264ERROR_PARAM_SET + 553                /**< @brief @hideinitializer */
#define H264ERROR_FRAME_MBS_MODE                        H264ERROR_PARAM_SET + 554                /**< @brief @hideinitializer */
/////// reserved for 6 parameters
#define H264ERROR_BIT_DEPTH_LUMA                        H264ERROR_PARAM_SET + 561                /**< @brief @hideinitializer */
#define H264ERROR_BIT_DEPTH_CHROMA                      H264ERROR_PARAM_SET + 562                /**< @brief @hideinitializer */
#define H264ERROR_CHROMA_FORMAT                         H264ERROR_PARAM_SET + 563                /**< @brief @hideinitializer */
/////// reserved for 14 parameters
#define H264ERROR_VUI_PRESENTATION                      H264ERROR_PARAM_SET + 578                /**< @brief @hideinitializer */
#define H264ERROR_WRITE_AU_DELIMITERS                   H264ERROR_PARAM_SET + 579                /**< @brief @hideinitializer */
#define H264ERROR_WRITE_SEQ_END_CODE                    H264ERROR_PARAM_SET + 580                /**< @brief @hideinitializer */
#define H264ERROR_WRITE_TIMESTAMPS                      H264ERROR_PARAM_SET + 581                /**< @brief @hideinitializer */
#define H264ERROR_TIMESTAMP_OFFSET                      H264ERROR_PARAM_SET + 582                /**< @brief @hideinitializer */
/////// reserved for 126 parameters
#define H264ERROR_VCSD_MODE                             H264ERROR_PARAM_SET + 582                /**< @brief @hideinitializer */
#define H264ERROR_VCSD_SENSIBILITY                      H264ERROR_PARAM_SET + 583                /**< @brief @hideinitializer */
#define H264ERROR_SLICE_MODE                            H264ERROR_PARAM_SET + 584                /**< @brief @hideinitializer */
#define H264ERROR_SLICE_ARG                             H264ERROR_PARAM_SET + 585                /**< @brief @hideinitializer */
#define H264ERROR_BSLICE_REF                            H264ERROR_PARAM_SET + 586                /**< @brief @hideinitializer */
#define H264ERROR_BSLICE_PYRAMID                        H264ERROR_PARAM_SET + 587                /**< @brief @hideinitializer */
#define H264ERROR_CB_OFFSET                             H264ERROR_PARAM_SET + 588                /**< @brief @hideinitializer */
#define H264ERROR_CR_OFFSET                             H264ERROR_PARAM_SET + 589                /**< @brief @hideinitializer */
#define H264ERROR_SUBPEL_MODE                           H264ERROR_PARAM_SET + 590                /**< @brief @hideinitializer */
#define H264ERROR_WEIGHTED_PMODE                        H264ERROR_PARAM_SET + 591                /**< @brief @hideinitializer */
#define H264ERROR_WEIGHTED_BMODE                        H264ERROR_PARAM_SET + 592                /**< @brief @hideinitializer */
#define H264ERROR_FAST_INTRA                            H264ERROR_PARAM_SET + 593                /**< @brief @hideinitializer */
#define H264ERROR_FAST_INTER                            H264ERROR_PARAM_SET + 594                /**< @brief @hideinitializer */
#define H264ERROR_PIC_AR_X                              H264ERROR_PARAM_SET + 595                /**< @brief @hideinitializer */
#define H264ERROR_PIC_AR_Y                              H264ERROR_PARAM_SET + 596                /**< @brief @hideinitializer */
#define H264ERROR_CALC_QUALITY                          H264ERROR_PARAM_SET + 597                /**< @brief @hideinitializer */
#define H264ERROR_CPU_OPT                               H264ERROR_PARAM_SET + 598                /**< @brief @hideinitializer */
#define H264ERROR_NUM_THREADS                           H264ERROR_PARAM_SET + 599                /**< @brief @hideinitializer */
#define H264ERROR_BUFFERING                             H264ERROR_PARAM_SET + 600                /**< @brief @hideinitializer */
#define H264ERROR_ENCODING_BUFFERING                    H264ERROR_PARAM_SET + 601                /**< @brief @hideinitializer */
#define H264ERROR_MIN_QUANT                             H264ERROR_PARAM_SET + 602                /**< @brief @hideinitializer */
#define H264ERROR_MAX_QUANT                             H264ERROR_PARAM_SET + 603                /**< @brief @hideinitializer */
#define H264ERROR_FRAME_PACKING_ARRANGEMENT             H264ERROR_PARAM_SET + 604                /**< @brief @hideinitializer */
/////// reserved for 293 parameters
#define H264ERROR_MIN_IDR_INTERVAL                      H264ERROR_PARAM_SET + 898                /**< @brief @hideinitializer */
#define H264ERROR_ADAPTIVE_B_FRAMES                     H264ERROR_PARAM_SET + 899                /**< @brief @hideinitializer */
#define H264ERROR_IDR_FREQUENCY                         H264ERROR_PARAM_SET + 900                /**< @brief @hideinitializer */
#define H264ERROR_FIELD_ORDER                           H264ERROR_PARAM_SET + 901                /**< @brief @hideinitializer */
#define H264ERROR_FIXED_I_POSITION                      H264ERROR_PARAM_SET + 902                /**< @brief @hideinitializer */
#define H264ERROR_ISOLATED_GOPS                         H264ERROR_PARAM_SET + 903                /**< @brief @hideinitializer */
#define H264ERROR_HIERAR_P_FRAMES                       H264ERROR_PARAM_SET + 904                /**< @brief not supported in enc_avc/trunk @hideinitializer */
/////// reserved for 48 parameters
#define H264ERROR_FAST_MULTI_REF_ME                     H264ERROR_PARAM_SET + 952                /**< @brief @hideinitializer */
#define H264ERROR_SUB_BLOCK_ME                          H264ERROR_PARAM_SET + 953                /**< @brief @hideinitializer */
/////// reserved for 98 parameters
#define H264ERROR_ENABLE_INTRA_BIG                      H264ERROR_PARAM_SET + 1052                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTRA_8x8                      H264ERROR_PARAM_SET + 1053                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTRA_4x4                      H264ERROR_PARAM_SET + 1054                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTRA_PCM                      H264ERROR_PARAM_SET + 1055                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTER_BIG                      H264ERROR_PARAM_SET + 1056                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTER_8x8                      H264ERROR_PARAM_SET + 1057                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTER_4x4                      H264ERROR_PARAM_SET + 1058                /**< @brief @hideinitializer */
#define H264ERROR_ENABLE_INTER_PCM                      H264ERROR_PARAM_SET + 1059                /**< @brief @hideinitializer */
/////// reserved for 92 parameters
#define H264ERROR_FAST_RD_OPTIMIZATION                  H264ERROR_PARAM_SET + 1152                /**< @brief @hideinitializer */
#define H264ERROR_QUANT_MODE                            H264ERROR_PARAM_SET + 1153                /**< @brief @hideinitializer */
/////// reserved for 798 parameters
/** @}*/
#define H264PARAM_DISABLED                              -10000000                /**< @brief @hideinitializer */

#endif // #if !defined (__ENC_AVC_DEF_INCLUDED__)

