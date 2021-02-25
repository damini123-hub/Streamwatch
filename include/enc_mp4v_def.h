/**
@file: enc_mp4v_def.h
@brief MPEG-4 Video Encoder API Defines

@verbatim
File: enc_mp4v_def.h
Desc: MPEG-4 Video Encoder API Defines

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
 **/

#if !defined (__ENC_MP4V_DEF_INCLUDED__)
#define __ENC_MP4V_DEF_INCLUDED__

/**
@def MPEG4_VIDEO_TYPES
@name Video type defines
@brief It is not really video types but rather sample presets
@{
**/

#define MPEG4_SP_L0              0      /**< @brief Simple Profile, Level 0             @hideinitializer*/
#define MPEG4_SP_L1              1      /**< @brief Simple Profile, Level 1             @hideinitializer*/
#define MPEG4_SP_L2              2      /**< @brief Simple Profile, Level 2             @hideinitializer*/
#define MPEG4_SP_L3              3      /**< @brief Simple Profile, Level 3             @hideinitializer*/
#define MPEG4_ASP_L0             4      /**< @brief Advanced Simple Profile, Level 0    @hideinitializer*/
#define MPEG4_ASP_L1             5      /**< @brief Advanced Simple Profile, Level 1    @hideinitializer*/
#define MPEG4_ASP_L2             6      /**< @brief Advanced Simple Profile, Level 2    @hideinitializer*/
#define MPEG4_ASP_L3             7      /**< @brief Advanced Simple Profile, Level 3    @hideinitializer*/
#define MPEG4_ASP_L4             8      /**< @brief Advanced Simple Profile, Level 4    @hideinitializer*/
#define MPEG4_ASP_L5             9      /**< @brief Advanced Simple Profile, Level 5    @hideinitializer*/
#define MPEG4_QCIF              10      /**< @brief QCIF                                @hideinitializer*/
#define MPEG4_CIF               11      /**< @brief CIF                                 @hideinitializer*/
#define MPEG4_halfD1            12      /**< @brief Half D1                             @hideinitializer*/
#define MPEG4_D1                13      /**< @brief D1                                  @hideinitializer*/
#define MPEG4_720p              14      /**< @brief 720p                                @hideinitializer*/
#define MPEG4_IPOD              15      /**< @brief Apple iPod                          @hideinitializer*/
#define MPEG4_PSP               16      /**< @brief Sony PSP                            @hideinitializer*/
#define MPEG4_IPOD_640x480      17      /**< @brief Apple iPod 640x480                  @hideinitializer*/
#define MPEG4_1080p             18      /**< @brief 1080p                               @hideinitializer*/
#define MPEG4_SP_L0b            19      /**< @brief Simple Profile, Level 0b            @hideinitializer*/
#define MPEG4_SP_L4a            20      /**< @brief Simple Profile, Level 4a            @hideinitializer*/
#define MPEG4_SP_L5             21      /**< @brief Simple Profile, Level 5             @hideinitializer*/
#define MPEG4_ShortHeader       30      /**< @brief                                     @hideinitializer*/
#define H263_Profile3_Level10   33      /**< @brief ITU-T H.263 Interactive and Streaming Wireless Profile (Profile 3) @hideinitializer*/
#define H263_Profile3_Level20   34      /**< @brief H.263 Profile 3 Level 20            @hideinitializer*/
#define H263_Profile3_Level30   35      /**< @brief H.263 Profile 3 Level 30            @hideinitializer*/
#define H263_Profile3_Level40   36      /**< @brief H.263 Profile 3 Level 40            @hideinitializer*/
#define H263_Profile3_Level45   37      /**< @brief H.263 Profile 3 Level 45            @hideinitializer*/
#define H263_Profile3_Level50   38      /**< @brief H.263 Profile 3 Level 50            @hideinitializer*/
#define H263_Profile3_Level60   39      /**< @brief H.263 Profile 3 Level 60            @hideinitializer*/
#define H263_Profile3_Level70   40      /**< @brief H.263 Profile 3 Level 70            @hideinitializer*/
#define MPEG4_MAIN_L2            41      /**< @brief H.263 Main Profile Level 2          @hideinitializer*/
#define MPEG4_MAIN_L3            42      /**< @brief H.263 Main Profile Level 3          @hideinitializer*/
#define MPEG4_MAIN_L4            43      /**< @brief H.263 Main Profile Level 4          @hideinitializer*/
#define MPEG4_CUSTOM           100      /**< @brief Custom            @hideinitializer*/
/** @}*/

/**
@def ERRORS_DEFINES
@name Error defines
@{
**/

#define M4ERROR_NONE             0      /**<@brief NO ERRORS         @hideinitializer*/
#define M4ERROR_FAILED           1      /**<@brief FAILED            @hideinitializer*/
#define M4ERROR_NOT_SUPPORTED    2      /**<@brief NOT SUPPORTED     @hideinitializer*/
#define M4ERROR_PARAM_UNKNOWN    3      /**<@brief UNKNOWN PARAMETER @hideinitializer*/
/** @}*/

/**
@def PROFILES
@name Profile defines
@{
**/
#define MPEG4_PROFILE_SIMPLE     0      /**<@brief Simple profile          @hideinitializer*/
#define MPEG4_PROFILE_SIMPLE_ADV 1      /**<@brief Advanced simple profile @hideinitializer*/
#define MPEG4_PROFILE_MAIN       2      /**<@brief Advanced simple profile @hideinitializer*/

#define H263_PROFILE_0           0      /**<@brief Profile 0               @hideinitializer*/
#define H263_PROFILE_3           3      /**<@brief Profile 3               @hideinitializer*/
/** @}*/

/**
@def LEVELS
@name Level defines
@{
**/
#define MPEG4_LEVEL_0            0      /**<@brief Level 0 */
#define MPEG4_LEVEL_0b          10      /**<@brief Level 0b */
#define MPEG4_LEVEL_1            1      /**<@brief Level 1 */
#define MPEG4_LEVEL_2            2      /**<@brief Level 2 */
#define MPEG4_LEVEL_3            3      /**<@brief Level 3 */
#define MPEG4_LEVEL_4a           4      /**<@brief Level 4a */
#define MPEG4_LEVEL_4            4      /**<@brief Level 4 */
#define MPEG4_LEVEL_5            5      /**<@brief Level 5 */

#define H263_LEVEL_10           10      /**<@brief Level 10 */
#define H263_LEVEL_20           20      /**<@brief Level 20 */
#define H263_LEVEL_30           30      /**<@brief Level 30 */
#define H263_LEVEL_40           40      /**<@brief Level 40 */
#define H263_LEVEL_45           45      /**<@brief Level 45 */
#define H263_LEVEL_50           50      /**<@brief Level 50 */
#define H263_LEVEL_60           60      /**<@brief Level 60 */
#define H263_LEVEL_70           70      /**<@brief Level 70 */
/** @}*/

/**
@def PERFORMANCE
@name Performance defines
@brief Options for the m4OutVideoPerformance function
@{
**/
#define MPEG4_PERF_FASTEST       0          /**<@brief Fastest */
#define MPEG4_PERF_BALANCED      9          /**<@brief Balanced. Recommended default value */
#define MPEG4_PERF_BEST_Q        15         /**<@brief Best quality. */
/**@}*/

/**
@def RC_MODES
@name Rate control modes
@{
**/
#define MPEG4_CBR                0          /**<@brief Constant Bit rate  @hideinitializer*/
#define MPEG4_VBR                1          /**<@brief Variable Bit rate  @hideinitializer*/
#define MPEG4_Q_QUALITY          2          /**<@brief Constant Quality   @hideinitializer*/
#define MPEG4_Q_QUANTIZER        3          /**<@brief Constant Quantizer @hideinitializer*/
/**@}*/

/**
@def QUALITY_RANGE
@name Quality range
@{
**/
#define QUALITY_INDEX_RANGE 15
/**@}*/
#endif // #if !defined (__ENC_MP4V_DEF_INCLUDED__)

