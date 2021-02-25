/**
 @file	mcprofile.h
 @brief Profile and Preset definitions for default codec settings

 @verbatim
 * File: mcprofile.h
 *
 * Desc: profile/preset defines for default codec settings
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/

#if !defined (__MC_PROFILE_INCLUDED__)
#define __MC_PROFILE_INCLUDED__

/////////////////////////////////////////////////////////////////
//
// returned strings of the default setting functions should contain the profile name
// and some codec specific data terminated by '\0' if the profile is known by the codec
// otherwise return NULL
//
// samples:
// MCPROFILE_VCD:   "VCD, PAL, MPEG-2 Video\0"      for MPEG video encoder
// MCPROFILE_DVD:   "DVD, NTSC, MPEG-2 Video\0"     for MPEG video encoder
//
// MCPROFILE_VCD:   "VCD, MPEG-1 Audio Layer 2\0"   for MPEG audio encoder
// MCPROFILE_DVD:   "DVD, MPEG-1 Audio Layer 2\0"   for MPEG audio encoder
// a.s.o
//
//
/////////////////////////////////////////////////////////////////


#define MCPROFILE_DEFAULT                       0x00000000       /**< @brief default setting of codec */


/////////////////////////////////////////////////////////////////
//
// reserved ranges
//
// 0x0001 - 0x0FFF   reserved for private codec profiles
//
/////////////////////////////////////////////////////////////////


#define MCPROFILE_VCD                           0x00001000       /**< @brief MPEG-1 VideoCD @hideinitializer */
#define MCPROFILE_SVCD                          0x00001001       /**< @brief MPEG-2 SuperVCD @hideinitializer */
#define MCPROFILE_DVD                           0x00001010       /**< @brief MPEG-2 DVD-Video @hideinitializer */
#define MCPROFILE_DVD_MPEG1                     0x00001011       /**< @brief MPEG-1 DVD-Video @hideinitializer */
#define MCPROFILE_DVD_DVR                       0x00001012       /**< @brief MPEG-2 DVD-VR @hideinitializer */
#define MCPROFILE_DVD_DVR_MPEG1                 0x00001013       /**< @brief MPEG-1 DVD-VR @hideinitializer */
#define MCPROFILE_DVD_PVR                       0x00001014       /**< @brief MPEG-2 DVD+VR, not yet implemented! @hideinitializer */
#define MCPROFILE_DVD_PVR_MPEG1                 0x00001015       /**< @brief MPEG-1 DVD+VR, not yet implemented! @hideinitializer */

#define MCPROFILE_DTV                           0x00001019       /**< @brief Networked Digital Television @hideinitializer */
#define MCPROFILE_DVB                           0x00001020       /**< @brief DVB @hideinitializer */
#define MCPROFILE_MMV                           0x00001021       /**< @brief MPEG-2 MICROMV @hideinitializer */
#define MCPROFILE_DVHS                          0x00001022       /**< @brief DVHS @hideinitializer */
#define MCPROFILE_ATSC                          0x00001023       /**< @brief MPEG-2 ATSC Terrestrial @hideinitializer */
#define MCPROFILE_ATSCHI                        0x00001024       /**< @brief MPEG-2 ATSC Terrestrial high data rate @hideinitializer */
#define MCPROFILE_CABLELABS                     0x00001025       /**< @brief CableLabs MPEG-2 MP @ ML (3.18MBit) or MP @ HL (18.1MBit) @hideinitializer */
#define MCPROFILE_ATSC_C                        0x00001026       /**< @brief MPEG-2 ATSC Cable @hideinitializer */

#define MCPROFILE_HDV_HD1                       0x00001030       /**< @brief MPEG-2 HDV HD1 @hideinitializer */
#define MCPROFILE_HDV_HD2                       0x00001031       /**< @brief MPEG-2 HDV HD2 @hideinitializer */


#define MCPROFILE_D10                           0x00001040       /**< @brief D-10 MPEG-2 4:2:2P @ ML (25 MBit) @hideinitializer */
#define MCPROFILE_D10_25                        0x00001040       /**< @brief D-10 MPEG-2 4:2:2P @ ML (25 MBit) @hideinitializer */
#define MCPROFILE_D10_30                        0x00001041       /**< @brief D-10 MPEG-2 4:2:2P @ ML (30 MBit) @hideinitializer */
#define MCPROFILE_D10_40                        0x00001042       /**< @brief D-10 MPEG-2 4:2:2P @ ML (40 MBit) @hideinitializer */
#define MCPROFILE_D10_50                        0x00001043       /**< @brief D-10 MPEG-2 4:2:2P @ ML (50 MBit) @hideinitializer */


#define MCPROFILE_HD_DVD                        0x00002000       /**< @brief HD DVD @hideinitializer */

#define MCPROFILE_AVCHD                         0x00002800       /**< @brief AVCHD @hideinitializer */
#define MCPROFILE_AVCHD_20                      0x00002801       /**< @brief AVCHD 2.0 1080/50p 1080/60p @hideinitializer */
#define MCPROFILE_AVC_INTRA_CLASS_50            0x00002802       /**< @brief P2 AVC Intra (Class 50) @hideinitializer */
#define MCPROFILE_AVC_INTRA_CLASS_100           0x00002803       /**< @brief P2 AVC Intra (Class 100) @hideinitializer */
#define MCPROFILE_AVC_INTRA_CLASS_200           0x00002810       /**< @brief P2 AVC Intra (Class 200) @hideinitializer */


#define MCPROFILE_AVC_INTRA_CLASS_50_RP2027     0x00002804       /**< @brief RP2027 AVC Intra (Class 50) @hideinitializer */
#define MCPROFILE_AVC_INTRA_CLASS_100_RP2027    0x00002805       /**< @brief RP2027 AVC Intra (Class 100) @hideinitializer */
#define MCPROFILE_AVC_INTRA_CLASS_200_RP2027    0x00002806       /**< @brief RP2027 AVC Intra (Class 200) @hideinitializer */

// deprecated
#define MCPROFILE_XAVC_INTRA_CBG                0x00002807       //
#define MCPROFILE_XAVC_INTRA_VBR                0x00002808       //
#define MCPROFILE_XAVC_LONG_GOP                 0x00002809       //

#define MCPROFILE_XAVC_4K                       MCPROFILE_XAVC_LONG_GOP  /**< @brief SONY  XAVC Long GOP 4K Profile, for M4 and XD Style @hideinitializer */
#define MCPROFILE_XAVC_HD_M4                    0x0000280A               /**< @brief SONY  XAVC Long GOP HD Profile, for MP4 Style @hideinitializer */
#define MCPROFILE_XAVC_HD_XD                    0x0000280B               /**< @brief SONY  XAVC Long GOP HD Profile, for XD Style @hideinitializer */

#define MCPROFILE_XAVC_HD_INTRA_CLASS_50_CBG    0x0000280C       		 /**< @brief SONY  XAVC HD Intra CBG Class 50 Profile (RP2027, XD Style) @hideinitializer */
#define MCPROFILE_XAVC_HD_INTRA_CLASS_100_CBG   MCPROFILE_XAVC_INTRA_CBG /**< @brief SONY  XAVC HD Intra CBG Class 100 Profile (RP2027, XD Style) @hideinitializer */
#define MCPROFILE_XAVC_HD_INTRA_CLASS_200_CBG   0x0000280D       		 /**< @brief SONY  XAVC HD Intra CBG Class 200 Profile (RP2027, XD Style) @hideinitializer */
#define MCPROFILE_XAVC_HD_INTRA_VBR             0x0000280E       		 /**< @brief SONY  XAVC HD Intra VBR Profile (M4 Style) @hideinitializer */

#define MCPROFILE_XAVC_4K_INTRA_CLASS_100_CBG   0x00002820       /**< @brief SONY  XAVC 4K Intra CBG Class 100 Profile (XD Style) @hideinitializer */
#define MCPROFILE_XAVC_4K_INTRA_CLASS_300_CBG   0x00002821       /**< @brief SONY  XAVC 4K Intra CBG Class 300 Profile (XD Style) @hideinitializer */
#define MCPROFILE_XAVC_4K_INTRA_CLASS_480_CBG   0x00002822       /**< @brief SONY  XAVC 4K Intra CBG Class 480 Profile (XD Style) @hideinitializer */

#define MCPROFILE_XAVC_4K_INTRA_CLASS_100_VBR   0x00002830       /**< @brief SONY  XAVC 4K Intra VBR Class 100 Profile (XD Style) @hideinitializer */
#define MCPROFILE_XAVC_4K_INTRA_CLASS_300_VBR   0x00002831       /**< @brief SONY  XAVC 4K Intra VBR Class 300 Profile (XD Style) @hideinitializer */
#define MCPROFILE_XAVC_4K_INTRA_CLASS_480_VBR   0x00002832       /**< @brief SONY  XAVC 4K Intra VBR Class 480 Profile (XD Style) @hideinitializer */

#define MCPROFILE_LONG_GOP_422_CLASS_G50        0x00002900       /**< @brief Panasonic AVC-LongG 4:2:2 Class G50 @hideinitializer */
#define MCPROFILE_LONG_GOP_422_CLASS_G25        0x00002901       /**< @brief Panasonic AVC-LongG 4:2:2 Class G25 @hideinitializer */
#define MCPROFILE_LONG_GOP_420_CLASS_G12        0x00002902       /**< @brief Panasonic AVC-LongG 4:2:0 Class G12 @hideinitializer */
#define MCPROFILE_LONG_GOP_420_CLASS_G6         0x00002903       /**< @brief Panasonic AVC-LongG 4:2:0 Class G6 @hideinitializer */

#define MCPROFILE_BD                            0x00003000       /**< @brief Blu Ray Disc @hideinitializer */
#define MCPROFILE_BD_HDMV                       0x00003001       /**< @brief Blu Ray Disc (Main Video) @hideinitializer */

#define MCPROFILE_PSP                           0x00004000       /**< @brief Play Station Portable by Sony @hideinitializer */
#define MCPROFILE_PSP_480x270                   0x00004001       /**< @brief Play Station Portable 480x270 by Sony @hideinitializer */
#define MCPROFILE_PSP_640x480                   0x00004002       /**< @brief Play Station Portable 640x480 by Sony @hideinitializer */

#define MCPROFILE_IPOD                          0x00005000       /**< @brief iPod by Apple @hideinitializer */
#define MCPROFILE_IPOD_640x480                  0x00005001       /**< @brief iPod 640x480 by Apple @hideinitializer */
#define MCPROFILE_IPAD                          0x00005010       /**< @brief iPad/iPad2 by Apple @hideinitializer */

#define MCPROFILE_3GPP                          0x00006000       /**< @brief 3GPP  @hideinitializer */
#define MCPROFILE_3GPP2                         0x00006100       /**< @brief 3GPP2 @hideinitializer */
#define MCPROFILE_ISMA                          0x00006200       /**< @brief ISMA @hideinitializer */

#define MCPROFILE_PIFF                          0x00006300       /**< @brief fragmented MP4 PIFF (as part of ISOFF) @hideinitializer */
#define MCPROFILE_IIS_SMOOTHSTREAMING           0x00006301       /**< @brief fragmented MP4 IIS Smooth Streaming Transport Protocol @hideinitializer */

#define MCPROFILE_ULTRAVIOLET                   0x00006302       /**< @brief fragmented MP4 CFF (DECE/UV) applies to multiplexer only @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_A_SD              0x00006380       /**< @brief fragmented MP4 CFF (DECE/UV) SD @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_A_HD              0x00006381       /**< @brief fragmented MP4 CFF (DECE/UV) HD @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_A_SD_51C          0x00006382       /**< @brief fragmented MP4 CFF (DECE/UV) SD 5.1 Channel @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_A_HD_51C          0x00006383       /**< @brief fragmented MP4 CFF (DECE/UV) HD 5.1 Channel @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_A_HD_71C          0x00006386       /**< @brief fragmented MP4 CFF (DECE/UV) HD 7.1 Channel @hideinitializer */

#define MCPROFILE_ULTRAVIOLET_V_PD_L1           0x00006390       /**< @brief fragmented MP4 CFF (DECE/UV) PD (Portable Devices) 320x180, 768kb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_PD_L2           0x00006391       /**< @brief fragmented MP4 CFF (DECE/UV) PD (Portable Devices) 320x240, 768kb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_PD_L3           0x00006392       /**< @brief fragmented MP4 CFF (DECE/UV) PD (Portable Devices) 416x240, 768kb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_SD_L1           0x00006393       /**< @brief fragmented MP4 CFF (DECE/UV) SD 640x480, 10mb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_SD_L2           0x00006394       /**< @brief fragmented MP4 CFF (DECE/UV) SD 854x480, 10mb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_HD_L1           0x00006395       /**< @brief fragmented MP4 CFF (DECE/UV) HD 1280x720, 25mb @hideinitializer */
#define MCPROFILE_ULTRAVIOLET_V_HD_L2           0x00006396       /**< @brief fragmented MP4 CFF (DECE/UV) HD 1920x1080, 25mb @hideinitializer */

#define MCPROFILE_JP2K_LOSSLESS                 0x00007000       /**< @brief JPEG 2000 lossless @hideinitializer */
#define MCPROFILE_JP2K_LOSSY                    0x00007001       /**< @brief JPEG 2000 lossy @hideinitializer */
#define MCPROFILE_JP2K_DCI_2K                   0x00007100       /**< @brief JPEG 2000 DCI 2K @hideinitializer */
#define MCPROFILE_JP2K_DCI_4K                   0x00007101       /**< @brief JPEG 2000 DCI 4K @hideinitializer */
#define MCPROFILE_MJ2                           0x00007102       /**< @brief JPEG 2000 file format @hideinitializer */

#define MCPROFILE_DIVX_QMOBILE                  0x00008000       /**< @brief QMOBILE DivX Profile @hideinitializer */
#define MCPROFILE_DIVX_MOBILE                   0x00008010       /**< @brief MOBILE DivX Profile @hideinitializer */
#define MCPROFILE_DIVX_HOME_THEATER             0x00008020       /**< @brief HOME_THEATER DivX Profile @hideinitializer */
#define MCPROFILE_DIVX_HDTV                     0x00008030       /**< @brief HDTV DivX Profile @hideinitializer */
#define MCPROFILE_DIVX_1080                     0x00008040       /**< @brief 1080 DivX Profile @hideinitializer */
#define MCPROFILE_DIVX_UNCONSTRAINED            0x00008100       /**< @brief Unconstrained DivX Profile @hideinitializer */

#define MCPROFILE_DIVXPLUS                      0x00008500       /**< @brief DivX-Plus @hideinitializer */


// DPS profiles
#define MCPROFILE_DPS                           0x00008600       /**< @brief DivX Plus Streaming @hideinitializer */

// DPS Video Levels for AVC and HEVC                                                                       AVC     HEVC
#define MCPROFILE_DPS_V_L1                      0x00008601       // DivX Plus Streaming Video Level  1:  400kb    400kb
#define MCPROFILE_DPS_V_L2                      0x00008602       // DivX Plus Streaming Video Level  2:  600kb    600kb
#define MCPROFILE_DPS_V_L3                      0x00008603       // DivX Plus Streaming Video Level  3:  800kb    900kb
#define MCPROFILE_DPS_V_L4                      0x00008604       // DivX Plus Streaming Video Level  4: 1200kb   1400kb
#define MCPROFILE_DPS_V_L5                      0x00008605       // DivX Plus Streaming Video Level  5: 1750kb   2100kb
#define MCPROFILE_DPS_V_L6                      0x00008606       // DivX Plus Streaming Video Level  6: 2400kb   3100kb
#define MCPROFILE_DPS_V_L7                      0x00008607       // DivX Plus Streaming Video Level  7: 3500kb   5000kb
#define MCPROFILE_DPS_V_L8                      0x00008608       // DivX Plus Streaming Video Level  8: 5300kb   7500kb
#define MCPROFILE_DPS_V_L9                      0x00008609       // DivX Plus Streaming Video Level  9: 8400kb  10000kb
#define MCPROFILE_DPS_V_L10                     0x0000860A       // DivX Plus Streaming Video Level 10:   ---   15000kb

#define MCPROFILE_DPS_V_TPT                     0x00008640       /**< @brief DivX Plus Streaming Trick-play Track @hideinitializer */

// DPS for High End Mobile and Tablet (HEMT) devices (H.264 High)
#define MCPROFILE_DPS_V_HEMT_L1                 0x00008661       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 1: 125kb @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L2                 0x00008662       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 2: 275kb @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L3                 0x00008663       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 3: same as MCPROFILE_DPS_V_L1 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L4                 0x00008664       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 4: same as MCPROFILE_DPS_V_L2 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L5                 0x00008665       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 5: same as MCPROFILE_DPS_V_L3 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L6                 0x00008666       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 6: same as MCPROFILE_DPS_V_L4 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L7                 0x00008667       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 7: same as MCPROFILE_DPS_V_L5 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L8                 0x00008668       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 8: same as MCPROFILE_DPS_V_L6 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L9                 0x00008669       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 9: same as MCPROFILE_DPS_V_L7 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L10                0x0000866A       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 10: same as MCPROFILE_DPS_V_L8 @hideinitializer */
#define MCPROFILE_DPS_V_HEMT_L11                0x0000866B       /**< @brief DivX Plus Streaming Mobile HEMT Video Level 11: same as MCPROFILE_DPS_V_L9 @hideinitializer */

// DPS for Low End Mobile (LEM) devices (H.264 Baseline)
#define MCPROFILE_DPS_V_LEM_L1                  0x00008671       /**< @brief DivX Plus Streaming Mobile LEM Video Level 1: 175kb @hideinitializer */
#define MCPROFILE_DPS_V_LEM_L2                  0x00008672       /**< @brief DivX Plus Streaming Mobile LEM Video Level 2: 300kb @hideinitializer */
#define MCPROFILE_DPS_V_LEM_L3                  0x00008673       /**< @brief DivX Plus Streaming Mobile LEM Video Level 3: 550kb @hideinitializer */

// DPS Audio Levels
#define MCPROFILE_DPS_A_2C                      0x00008680       /**< @brief DivX Plus Streaming Audio 2 Channels @hideinitializer */
#define MCPROFILE_DPS_A_51C                     0x00008681       /**< @brief DivX Plus Streaming Audio 5.1 Channels @hideinitializer */
#define MCPROFILE_DPS_A_71C                     0x00008682       /**< @brief DivX Plus Streaming Audio 7.1  Channels @hideinitializer */

// DPS Mobile Audio
#define MCPROFILE_DPS_A_2CM                     0x00008690       /**< @brief DivX Plus Streaming Mobile Audio 2 Channels @hideinitializer */


/**
 *@ingroup HEVC_VIDEO_TYPES
 *@{
**/
// DivX HEVC
//#define MCPROFILE_DIVXHEVC                    0x00008700       // reserved for future DIVXHEVC audio or muxer constrains
#define MCPROFILE_DIVXHEVC_720P                 0x00008701       /**< @brief  Divx HEVC 720p Profile @hideinitializer*/
#define MCPROFILE_DIVXHEVC_1080P                0x00008702       /**< @brief  DivX HEVC 1080p Profile @hideinitializer*/
#define MCPROFILE_DIVXHEVC_4K                   0x00008703       /**< @brief  DivX HEVC 4K Profile @hideinitializer*/

#define MCPROFILE_DIVXHEVC_TS_720P              0x00008704       /**< @brief  Divx HEVC Temporal Scalability 720p Profile @hideinitializer*/
#define MCPROFILE_DIVXHEVC_TS_1080P             0x00008705       /**< @brief  DivX HEVC Temporal Scalability 1080p Profile @hideinitializer*/
#define MCPROFILE_DIVXHEVC_TS_4K                0x00008706       /**< @brief  DivX HEVC Temporal Scalability 4K Profile @hideinitializer*/
/** @}*/


// DPS Live profiles
#define MCPROFILE_DPSLIVE                       0x00008800       /**< @brief DivX Plus Live Streaming @hideinitializer */

// DPS Live Video Levels for HEVC                                                                               HEVC
#define MCPROFILE_DPSLIVE_V_L1                  0x00008801       // DivX Plus Live Streaming Video Level  1:   400kb
#define MCPROFILE_DPSLIVE_V_L2                  0x00008802       // DivX Plus Live Streaming Video Level  2:   600kb
#define MCPROFILE_DPSLIVE_V_L3                  0x00008803       // DivX Plus Live Streaming Video Level  3:   900kb
#define MCPROFILE_DPSLIVE_V_L4                  0x00008804       // DivX Plus Live Streaming Video Level  4:  1400kb
#define MCPROFILE_DPSLIVE_V_L5                  0x00008805       // DivX Plus Live Streaming Video Level  5:  2100kb
#define MCPROFILE_DPSLIVE_V_L6                  0x00008806       // DivX Plus Live Streaming Video Level  6:  3100kb
#define MCPROFILE_DPSLIVE_V_L7                  0x00008807       // DivX Plus Live Streaming Video Level  7:  5000kb
#define MCPROFILE_DPSLIVE_V_L8                  0x00008808       // DivX Plus Live Streaming Video Level  8:  7500kb
#define MCPROFILE_DPSLIVE_V_L9                  0x00008809       // DivX Plus Live Streaming Video Level  9: 10000kb
#define MCPROFILE_DPSLIVE_V_L10                 0x0000880A       // DivX Plus Live Streaming Video Level 10: 15000kb

// DPS Live Audio Levels
#define MCPROFILE_DPSLIVE_A_2C                  MCPROFILE_DPS_A_2C   /**< @brief DivX Plus Live Streaming Audio is the same as DPS @hideinitializer */
#define MCPROFILE_DPSLIVE_A_51C                 MCPROFILE_DPS_A_51C  /**< @brief DivX Plus Live Streaming Audio is the same as DPS @hideinitializer */

// DPS Live Mobile Audio
#define MCPROFILE_DPSLIVE_A_2CM                 MCPROFILE_DPS_A_2CM  /**< @brief DivX Plus Live Streaming Audio is the same as DPS @hideinitializer */



// XDCAM Profiles
#define MCPROFILE_XDCAM_IMX                     0x00010000       /**< @brief Sony XDCAM IMX @hideinitializer */
#define MCPROFILE_XDCAM_IMX_25                  0x00010000       /**< @brief Sony XDCAM IMX MPEG-2 4:2:2P @ ML (25 MBit) @hideinitializer */
#define MCPROFILE_XDCAM_IMX_30                  0x00010001       /**< @brief Sony XDCAM IMX MPEG-2 4:2:2P @ ML (30 MBit) @hideinitializer */
#define MCPROFILE_XDCAM_IMX_40                  0x00010002       /**< @brief Sony XDCAM IMX MPEG-2 4:2:2P @ ML (40 MBit) @hideinitializer */
#define MCPROFILE_XDCAM_IMX_50                  0x00010003       /**< @brief Sony XDCAM IMX MPEG-2 4:2:2P @ ML (50 MBit) @hideinitializer */

#define MCPROFILE_XDCAM_DV                      0x00010050       /**< @brief Sony XDCAM DV25 @hideinitializer */

#define MCPROFILE_XDCAM_HD                      0x00010100       /**< @brief Sony XDCAM HD @hideinitializer */
#define MCPROFILE_XDCAM_1080_CBR_25             0x00010100       /**< @brief Sony XDCAM MP/H-14 (25 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_1080_VBR_17_5           0x00010101       /**< @brief Sony XDCAM MP/HL (17.5 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_1080_VBR_35             0x00010102       /**< @brief Sony XDCAM MP/HL (35 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_540_CBR_12_5            0x00010103       /**< @brief Sony XDCAM MP/H-14 (12.5 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_540_VBR_8_75            0x00010104       /**< @brief Sony XDCAM MP/HL (8.75 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_540_VBR_17_5            0x00010105       /**< @brief Sony XDCAM MP/HL (17.5 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_EX_1920                 0x00010110       /**< @brief Sony XDCAM EX MP/HL   4:2:0 1920x1080 (35 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_EX_1440                 0x00010111       /**< @brief Sony XDCAM EX MP/H-14 4:2:0 1440x1080 (25 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_EX_1280                 0x00010112       /**< @brief Sony XDCAM EX MP/HL   4:2:0 1280x720  (35 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_420_1440             0x00010113       /**< @brief Sony XDCAM HD MP/H-14 4:2:0 1440x1080 (25 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_420_1280             0x00010114       /**< @brief Sony XDCAM HD MP/HL   4:2:0 1280x720  (35 Mbit VBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_422_1920             0x00010115       /**< @brief Sony XDCAM HD 422P/HL 4:2:2 1920x1080 (50 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_422_1280             0x00010116       /**< @brief Sony XDCAM HD 422P/HL 4:2:2 1280x720  (50 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_420_1280_CBR_25      0x00010117       /**< @brief Sony XDCAM HD MP/HL   4:2:0 1280x720  (25 Mbit CBR) @hideinitializer */
#define MCPROFILE_XDCAM_HD_422_1920_540         0x00010118       /**< @brief Sony XDCAM HD 422P/HL 4:2:2 1920x540  (25 Mbit CBR) @hideinitializer */

#define MCPROFILE_XDCAM_SXS_HD                  0x00010300       /**< @brief Sony XDCAM HD for SxS memory cards @hideinitializer */
#define MCPROFILE_XDCAM_SXS_IMX                 0x00010330       /**< @brief Sony XDCAM IMX for SxS memory cards @hideinitializer */
#define MCPROFILE_XDCAM_SXS_DV                  0x00010360       /**< @brief Sony XDCAM DV for SxS memory cards @hideinitializer */

#define MCPROFILE_XAVC_SXS                      0x00010390       /**< @brief Sony XAVC for SxS memory cards @hideinitializer */

#define MCPROFILE_PANASONIC_P2                  0x00011000       /**< @brief outdated, do not use  @hideinitializer */
#define MCPROFILE_PANASONIC_P2_DVCPRO           0x00011001       /**< @brief Panasonic P2 DVCPRO compatible MXF multiplex (Op-Atom, clipwrapped, audio SMPTE 382M) @hideinitializer */
#define MCPROFILE_PANASONIC_P2_AVCI             0x00011002       /**< @brief Panasonic P2 AVC-Intra compatible MXF multiplex (Op-Atom, clipwrapped, audio SMPTE 382M) @hideinitializer */
#define MCPROFILE_PANASONIC_P2_AVC_LG           0x00011003       /**< @brief Panasonic P2 AVC LongGOP compatible MXF multiplex (Op-1b, framewrapped, audio SMPTE 382M) @hideinitializer */

#define MCPROFILE_FLASH_LOWRES                  0x00011010       /**< @brief Adobe Flash (Low Resolution Web Content) @hideinitializer */
#define MCPROFILE_FLASH_HIGHRES                 0x00011011       /**< @brief Adobe Flash (High Resolution Web Content) @hideinitializer */

#define MCPROFILE_SILVERLIGHT                   0x00012000       /**< @brief Microsoft Silverlight, Smooth Streaming @hideinitializer */

#define MCPROFILE_DASH264                       0x00013000       /**< @brief Profile for DASH-264 compliance, VOD and live profiles. @hideinitializer */

#define MCPROFILE_HLS                           0x00014000       /**< @brief Profile for HLS compliance @hideinitializer */
#define MCPROFILE_HLS_CELL_L1                   0x00014001       /**< @brief HLS over cellular network Level 1:        Video bit rate 200kb; Audio bit rate 64 @hideinitializer */
#define MCPROFILE_HLS_CELL_L2                   0x00014002       /**< @brief HLS over cellular network Level 2:        Video bit rate 400kb; Audio bit rate 64 @hideinitializer */
#define MCPROFILE_HLS_CELL_L3                   0x00014003       /**< @brief HLS over cellular/Wi-Fi network Level 3:  Video bit rate 600kb; Audio bit rate 64 @hideinitializer */
#define MCPROFILE_HLS_WIFI_L1                   0x00014010       /**< @brief HLS over Wi-Fi network Level 1:           Video bit rate 1200kb; Audio bit rate 96 @hideinitializer */
#define MCPROFILE_HLS_WIFI_L2                   0x00014011       /**< @brief HLS over Wi-Fi network Level 2:           Video bit rate 3500kb; Audio bit rate 128 @hideinitializer */
#define MCPROFILE_HLS_WIFI_L3                   0x00014012       /**< @brief HLS over Wi-Fi network Level 3:           Video bit rate 5000kb; Audio bit rate 128 @hideinitializer */
#define MCPROFILE_HLS_WIFI_L4                   0x00014013       /**< @brief HLS over Wi-Fi network Level 4:           Video bit rate 6500kb; Audio bit rate 128 @hideinitializer */
#define MCPROFILE_HLS_WIFI_L5                   0x00014014       /**< @brief HLS over Wi-Fi network Level 5:           Video bit rate 8500kb; Audio bit rate 128 @hideinitializer */

/////////////////////////////////////////////////////////////////


#endif // #if !defined (__MC_PROFILE_INCLUDED__)

