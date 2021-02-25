/********************************************************************
created:    2009/07/20 - 12:00
file name:  sample_common_args.cpp
purpose:    Common command line parsing routines

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 This software is protected by copyright law and international treaties.  Unauthorized 
 reproduction or distribution of any portion is prohibited by law.
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if !defined(__linux__) && !defined(__APPLE__)
#include <conio.h> // _kbhit()
#endif

#include "sample_common_args.h"
#include "sample_common_misc.h"
#include "mcfourcc.h"
#include "mcprofile.h"

#if defined _MSC_VER
#define strcmp_lower_case _stricmp
#else
#define strcmp_lower_case strcasecmp
#endif

// add new command line parameters to this table,
// make sure to keep 2nd and 3rd column commands unique and to not shift the index of IDS_CUSTOM_ARG!
static const arg_item_desc_t g_arg_items[] =
{
    // common args
    { IDS_VIDEO_FILE,       "v",        "video",        ItemTypeString, 0,              "raw input video file name"},
    { IDS_AUDIO_FILE,       "a",        "audio",        ItemTypeString, 0,              "raw input audio file name"},
    { IDS_INPUT_FILE,       "i",        "input",        ItemTypeString, 0,              "input file name"},
    { IDS_OUTPUT_FILE,      "o",        "output",       ItemTypeString, 0,              "output file name"},
    { IDI_BITRATE,          "b",        "bitrate",      ItemTypeInt,    0,              "target bit rate (kbps)"},
    { IDI_BITS_PER_SAMPLE,  "bps",      "",             ItemTypeInt,    0,              "bits per sample"},
    { IDS_CONFIG_FILE,      "c",        "config",       ItemTypeString, 0,              "config file name"},
    { IDS_CUSTOM_ARG,       "",         "",             ItemTypeString, 0,              "custom argument"},             // this one is for check_params()
    { IDS_OUTPUT_FILE_VID,  "ov",       "outputvideo",  ItemTypeString, 0,              "output file name video"},
    { IDS_OUTPUT_FILE_AUD,  "oa",       "outputaudio",  ItemTypeString, 0,              "output file name audio"},
    { IDI_OUTPUT_BITS_PER_SAMPLE, "obps", "",           ItemTypeInt,    0,              "output bits per sample"},
    { IDS_REC_FILE,         "preview",  "",             ItemTypeString, 0,              "output file name for reconstructed stream"},
    { IDS_ORG_FILE,         "original",   "",           ItemTypeString, 0,              "output file name for source stream"},
    { IDS_OPS_FILE,         "opsfile",  "",             ItemTypeString, 0,              "name of operations list file"},
    { IDS_LOG_FILE,         "logfile",  "",             ItemTypeString, 0,              "name of output logging file"},
	{ IDS_SERIAL_NUMBER,    "sn",       "serialnumber", ItemTypeString, 0,              "serial number"},
	{ IDI_NUM_THREADS,      "nth",      "numthreads",   ItemTypeInt,    0,              "number of threads"},
	{ IDN_QUIET,            "q",        "quiet",        ItemTypeNoArg,  1,              "quiet mode"},
    { IDS_STAT_FILE,        "st",       "statistics",   ItemTypeString, 0,              "statistics file name"},

    //{ IDS_EXT_MEM_ALLOC,    "",         "",             ItemTypeNoArg,  0,              "no external memory allocator"},
    { IDS_EXT_MEM_ALLOC,    "malloc",   "memalloc",     ItemTypeNoArg,  1,              "external memory allocator"},
    
    // MC preset
    { IDI_PRESET,           "preset",   "",                 ItemTypeInt,    0,          "MC preset"},

    // video args
    { IDI_V_WIDTH,          "w",        "width",        ItemTypeInt,    0,              "picture width"},
    { IDI_V_HEIGHT,         "h",        "height",       ItemTypeInt,    0,              "picture height"},
    { IDI_V_LINE_SIZE,      "l",        "linesize",     ItemTypeInt,    0,              "picture stride"},
    { IDD_V_FRAMERATE,      "f",        "framerate",    ItemTypeDouble, 0,              "frame rate"},
    { IDI_V_NUM_LAYERS,     "nl",       "numlayers",    ItemTypeInt,    0,              "number of scalability layers"},
    { IDI_V_SR_MODE,        "srm",      "sr_mode",      ItemTypeInt,    0,              "smart render mode"},
    { IDI_V_MULTIPASS_MODE, "mpass",    "multipass",    ItemTypeInt,    1,              "single-/2-pass/multipass mode"},
    { IDI_V_PAL,            "pal",      "",             ItemTypeInt,    0,              "PAL video"},
    { IDI_V_START_FRAME,    "tstart",   "",             ItemTypeInt,    0,              "Start frame for processing"},
    { IDI_V_DURATION,       "tdurat",   "",             ItemTypeInt,    0,              "A number of frames for processing"},
    { IDN_V_SWITCH_I,       "switchi",  "",             ItemTypeNoArg,  1,              "SR <-> non SR switching at I frame"},
    { IDN_V_PROGRESS,       "progress", "",             ItemTypeNoArg,  1,              "Show progress during decoding"},
    { IDN_V_MD5,            "md5",      "",             ItemTypeNoArg,  1,              "calculate MD5 of decoded frames"},
    { IDN_V_MD5L,           "md5l",     "",             ItemTypeNoArg,  1,              "calculate MD5 of decoded frames (lowercase)"},
    { IDN_V_SMP,            "smp",      "",             ItemTypeInt,   -1,              "SMP mode"},

    { IDN_V_FOURCC,         "",         "",             ItemTypeNoArg,  0,              "input fourCC"},                // this one is for check_params()
    { IDN_V_FOURCC,         "iyuv",     "",             ItemTypeNoArg,  FOURCC_IYUV,    "planar 8-bit YUV 4:2:0 (Y, Cb, Cr)"},
    { IDN_V_FOURCC,         "i420",     "",             ItemTypeNoArg,  FOURCC_I420,    "planar 8-bit YUV 4:2:0 (Y, Cb, Cr)"}, // identical to IYUV
    { IDN_V_FOURCC,         "yv12",     "",             ItemTypeNoArg,  FOURCC_YV12,    "planar 8-bit YUV 4:2:0 (Y, Cr, Cb)"},
    { IDN_V_FOURCC,         "yv16",     "",             ItemTypeNoArg,  FOURCC_YV16,    "planar 8-bit YUV 4:2:2 (Y, Cr, Cb)"},
    { IDN_V_FOURCC,         "uyvy",     "",             ItemTypeNoArg,  FOURCC_UYVY,    "packed 8-bit YUV 4:2:2 (Cb, Y, Cr, Y)"},
    { IDN_V_FOURCC,         "yuy2",     "",             ItemTypeNoArg,  FOURCC_YUY2,    "packed 8-bit YUV 4:2:2 (Y, Cb, Y, Cr)"},
    { IDN_V_FOURCC,         "bgr3",     "rgb24",        ItemTypeNoArg,  FOURCC_BGR3,    "packed 24-bit RGB"},
    { IDN_V_FOURCC,         "bgr4",     "rgb32",        ItemTypeNoArg,  FOURCC_BGR4,    "packed 32-bit RGB"},
    { IDN_V_FOURCC,         "bgra",     "argb32",       ItemTypeNoArg,  FOURCC_BGRA,    "packed 32-bit RGB with alpha channel"},
    { IDN_V_FOURCC,         "rgba",     "",             ItemTypeNoArg,  FOURCC_RGBA,    "packed 32-bit RGB with alpha channel"},
    { IDN_V_FOURCC,         "v210",     "",             ItemTypeNoArg,  FOURCC_V210,    "packed 10-bit YUV 4:2:2"},
    { IDN_V_FOURCC,         "v216",     "v216",         ItemTypeNoArg,  FOURCC_V216,    "V216 422 16 bit"},
    { IDN_V_FOURCC,         "y16" ,     "y16" ,         ItemTypeNoArg,  FOURCC_Y16,     "Grayscale 16bit"},
    { IDN_V_FOURCC,         "y416",     "y416",         ItemTypeNoArg,  FOURCC_Y416,    "Y416 444 16 bit"},
    { IDN_V_FOURCC,         "h264",     "",             ItemTypeNoArg,  FOURCC_H264,    "H264 bitstream"},
    { IDN_V_FOURCC,         "xyzc",     "xyz12",        ItemTypeNoArg,  FOURCC_XYZ12,   "D-Cinema XYZ 12bit"},
    { IDN_V_FOURCC,         "dpxa",     "dpx10",        ItemTypeNoArg,  FOURCC_DPX10,   "10bit DPX format, SMPTE 268M"},
    //{ IDN_V_FOURCC,         "rgb3",     "",             ItemTypeNoArg,    FOURCC_RGB3,    ""},
    { IDN_V_FOURCC,         "bgra32f",  "",             ItemTypeNoArg,  FOURCC_BGRA32F, ""},
    { IDN_V_FOURCC,         "b32f",     "",             ItemTypeNoArg,  FOURCC_B32F,    ""},
    { IDN_V_FOURCC,         "argb",     "",             ItemTypeNoArg,  FOURCC_ARGB,    "ARGB Colorspace as common used on Macintosh. May not be available on other platforms"},
    { IDN_V_FOURCC,         "r555",     "rgb555",       ItemTypeNoArg,  FOURCC_R555,    ""},
    { IDN_V_FOURCC,         "r565",     "rgb565",       ItemTypeNoArg,  FOURCC_R565,    ""},
    { IDN_V_FOURCC,         "ayuv",     "",             ItemTypeNoArg,  FOURCC_AYUV,    ""},
    { IDN_V_FOURCC,         "vuya",     "",             ItemTypeNoArg,  FOURCC_VUYA,    ""},
    { IDN_V_FOURCC,         "vuya32f",     "",          ItemTypeNoArg,  FOURCC_VUYA32F, ""},
    { IDN_V_FOURCC,         "y32f",     "",             ItemTypeNoArg,  FOURCC_Y32F,    ""},
    { IDN_V_FOURCC,         "yuyv",     "",             ItemTypeNoArg,  FOURCC_YUYV,    ""},
    { IDN_V_FOURCC,         "yvyu",     "",             ItemTypeNoArg,  FOURCC_YVYU,    ""},
    { IDN_V_FOURCC,         "vyuy",     "",             ItemTypeNoArg,  FOURCC_VYUY,    ""},
    //{ IDN_V_FOURCC,         "extp",     "",             ItemTypeNoArg,    FOURCC_EXTP,    "external, 3-plane colorspace-converter (requires a valid ext_cc_struct)"},
    { IDN_V_FOURCC,         "i422",     "",             ItemTypeNoArg,  FOURCC_I422,    ""},
    { IDN_V_FOURCC,         "i444",     "",             ItemTypeNoArg,  FOURCC_I444,    ""},
    { IDN_V_FOURCC,         "yv24",     "",             ItemTypeNoArg,  FOURCC_YV24,    ""},
    { IDN_V_FOURCC,         "y41p",     "",             ItemTypeNoArg,  FOURCC_Y41P,    ""},
    { IDN_V_FOURCC,         "nv12",     "",             ItemTypeNoArg,  FOURCC_NV12,    ""},
    { IDN_V_FOURCC,         "nv21",     "",             ItemTypeNoArg,  FOURCC_NV21,    ""},
    { IDN_V_FOURCC,         "nv24",     "",             ItemTypeNoArg,  FOURCC_NV24,    ""},
    { IDN_V_FOURCC,         "imc1",     "",             ItemTypeNoArg,  FOURCC_IMC1,    ""},
    { IDN_V_FOURCC,         "imc2",     "",             ItemTypeNoArg,  FOURCC_IMC2,    ""},
    { IDN_V_FOURCC,         "imc3",     "",             ItemTypeNoArg,  FOURCC_IMC3,    ""},
    { IDN_V_FOURCC,         "imc4",     "",             ItemTypeNoArg,  FOURCC_IMC4,    ""},
    { IDN_V_FOURCC,         "yvu9",     "",             ItemTypeNoArg,  FOURCC_YVU9,    ""},
    { IDN_V_FOURCC,         "yuv9",     "",             ItemTypeNoArg,  FOURCC_YUV9,    ""},
    { IDN_V_FOURCC,         "y211",     "",             ItemTypeNoArg,  FOURCC_Y211,    ""},
    { IDN_V_FOURCC,         "cljr",     "",             ItemTypeNoArg,  FOURCC_CLJR,    ""},
    { IDN_V_FOURCC,         "r32c",     "",             ItemTypeNoArg,  FOURCC_R32C,    "16..235 (16:black, 235-white) RGB32"},
    { IDN_V_FOURCC,         "r24c",     "",             ItemTypeNoArg,  FOURCC_R24C,    "16..235 (16:black, 235-white) RGB24"},
    //{ IDN_V_FOURCC,         "yuyv",     "",             ItemTypeNoArg,  FOURCC_YuYv,    "No clamping to ITU601 range"},   // conflict clamping / full-range
    //{ IDN_V_FOURCC,         "uyvy",     "",             ItemTypeNoArg,  FOURCC_UyVy,    "No clamping to ITU601 range"},   // conflict clamping / full-range
    { IDN_V_FOURCC,         "v210",     "",             ItemTypeNoArg,  FOURCC_v210,    "Quicktime v210 10bit format"}, // note: 'v', not 'V'
    { IDN_V_FOURCC,         "w009",     "",             ItemTypeNoArg,  FOURCC_W009,    "like yv12 16-bit component, 9 - significant"},
    { IDN_V_FOURCC,         "w010",     "",             ItemTypeNoArg,  FOURCC_W010,    "4:2:0 10-bit"},
    { IDN_V_FOURCC,         "w011",     "",             ItemTypeNoArg,  FOURCC_W011,    "4:2:0 11-bit"},
    { IDN_V_FOURCC,         "w012",     "",             ItemTypeNoArg,  FOURCC_W012,    "4:2:0 12-bit"},
    { IDN_V_FOURCC,         "w016",     "",             ItemTypeNoArg,  FOURCC_W016,    "4:2:0 16-bit"},
    { IDN_V_FOURCC,         "x009",     "",             ItemTypeNoArg,  FOURCC_X009,    "inverted W009"},
    { IDN_V_FOURCC,         "x010",     "",             ItemTypeNoArg,  FOURCC_X010,    "inverted W010"},
    { IDN_V_FOURCC,         "x011",     "",             ItemTypeNoArg,  FOURCC_X011,    "inverted W011"},
    { IDN_V_FOURCC,         "x012",     "",             ItemTypeNoArg,  FOURCC_X012,    "inverted W012"},
    { IDN_V_FOURCC,         "x014",     "",             ItemTypeNoArg,  FOURCC_X014,    "inverted W014"},
    { IDN_V_FOURCC,         "x016",     "",             ItemTypeNoArg,  FOURCC_X016,    "inverted W016"},
    { IDN_V_FOURCC,         "w209",     "",             ItemTypeNoArg,  FOURCC_W209,    "like yv16 16-bit component, 9 - significant"},
	{ IDN_V_FOURCC,         "w210",     "",             ItemTypeNoArg,  FOURCC_W210,    "planar 10-bit YUV 4:2:2 (Y, Cr, Cb)"},
    { IDN_V_FOURCC,         "w211",     "",             ItemTypeNoArg,  FOURCC_W211,    "4:2:2 11-bit"},
    { IDN_V_FOURCC,         "w212",     "",             ItemTypeNoArg,  FOURCC_W212,    "4:2:2 12-bit"},
    { IDN_V_FOURCC,         "w214",     "",             ItemTypeNoArg,  FOURCC_W214,    "4:2:2 14-bit"},
    { IDN_V_FOURCC,         "w216",     "",             ItemTypeNoArg,  FOURCC_W216,    "4:2:2 16-bit"},
    { IDN_V_FOURCC,         "x209",     "",             ItemTypeNoArg,  FOURCC_X209,    "inverted W209"},
	{ IDN_V_FOURCC,         "x210",     "",             ItemTypeNoArg,  FOURCC_X210,    "inverted W210"},
    { IDN_V_FOURCC,         "x211",     "",             ItemTypeNoArg,  FOURCC_X211,    "inverted W211"},
    { IDN_V_FOURCC,         "x212",     "",             ItemTypeNoArg,  FOURCC_X212,    "inverted W212"},
    { IDN_V_FOURCC,         "x216",     "",             ItemTypeNoArg,  FOURCC_X216,    "inverted W216"},
    { IDN_V_FOURCC,         "w409",     "",             ItemTypeNoArg,  FOURCC_W409,    "like yv24 16-bit component, 9 - significant"},
    { IDN_V_FOURCC,         "w410",     "",             ItemTypeNoArg,  FOURCC_W410,    "4:4:4 10-bit"},
    { IDN_V_FOURCC,         "w411",     "",             ItemTypeNoArg,  FOURCC_W411,    "4:4:4 11-bit"},
    { IDN_V_FOURCC,         "w412",     "",             ItemTypeNoArg,  FOURCC_W412,    "4:4:4 12-bit"},
    { IDN_V_FOURCC,         "w416",     "",             ItemTypeNoArg,  FOURCC_W416,    "4:4:4 16-bit"},
    { IDN_V_FOURCC,         "x409",     "",             ItemTypeNoArg,  FOURCC_X409,    "inverted W409"},
    { IDN_V_FOURCC,         "x410",     "",             ItemTypeNoArg,  FOURCC_X410,    "inverted W410"},
    { IDN_V_FOURCC,         "x411",     "",             ItemTypeNoArg,  FOURCC_X411,    "inverted W411"},
    { IDN_V_FOURCC,         "x412",     "",             ItemTypeNoArg,  FOURCC_X412,    "inverted W412"},
    { IDN_V_FOURCC,         "x416",     "",             ItemTypeNoArg,  FOURCC_X416,    "inverted W416"},
    { IDN_V_FOURCC,         "p010",     "",             ItemTypeNoArg,  FOURCC_P010,    ""},
    { IDN_V_FOURCC,         "p016",     "",             ItemTypeNoArg,  FOURCC_P016,    ""},
    { IDN_V_FOURCC,         "p210",     "",             ItemTypeNoArg,  FOURCC_P210,    ""},
    { IDN_V_FOURCC,         "p216",     "",             ItemTypeNoArg,  FOURCC_P216,    ""},
    { IDN_V_FOURCC,         "y210",     "",             ItemTypeNoArg,  FOURCC_Y210,    ""},
    { IDN_V_FOURCC,         "y216",     "",             ItemTypeNoArg,  FOURCC_Y216,    ""},
    { IDN_V_FOURCC,         "y410",     "",             ItemTypeNoArg,  FOURCC_Y410,    ""},
    { IDN_V_FOURCC,         "a2r10g10b10",     "",      ItemTypeNoArg,  FOURCC_AR10,    "A2R10G10B10"},
    { IDN_V_FOURCC,         "a2b10g10r10",     "",      ItemTypeNoArg,  FOURCC_AB10,    "A2B10G10R10"},
    { IDN_V_FOURCC,         "r16g16b16p",     "",       ItemTypeNoArg,  FOURCC_R48P,    "Planar R16G16B16"},

    { IDI_V_PERFORMANCE,    "perf",     "",             ItemTypeInt,    0,              "performance preset"},
    { IDN_V_COLOR_MATRIX,   "bt601",    "",             ItemTypeNoArg,  601,            "ITU-R BT.601"},
    { IDN_V_COLOR_MATRIX,   "bt709",    "",             ItemTypeNoArg,  709,            "ITU-R BT.709"},

    { IDN_V_INTERLACED,     "",         "",             ItemTypeNoArg,  0,              "interlaced / progressive"},    // this one is for check_params()
    { IDN_V_INTERLACED,     "p",        "progressive",  ItemTypeNoArg,  0,              "progressive"},
    { IDN_V_INTERLACED,     "i",        "interlaced",   ItemTypeNoArg,  1,              "interlaced (field pictures)"},
    { IDN_V_INTERLACED,     "mbaff",    "",             ItemTypeNoArg,  2,              "interlaced (macroblock-adaptive frame-field)"},

    { IDN_V_SNR_SCALABILITY,"",         "",             ItemTypeNoArg,  0,              "SNR scalability type"},    // this one is for check_params()
    { IDN_V_SNR_SCALABILITY,"cgs",      "",             ItemTypeNoArg,  0,              "CGS"},
    { IDN_V_SNR_SCALABILITY,"mgs",      "",             ItemTypeNoArg,  1,              "MGS"},

    // audio args
    { ID_A_SAMPLERATE,      "s",        "samplerate",   ItemTypeInt,    0,              "audio sample rate"},
    { ID_A_CHANNELS,        "ch",       "channels",     ItemTypeInt,    0,              "number of channels"},
    { ID_A_OUTPUT_SAMPLERATE, "os",     "outsamplerate", ItemTypeInt,   0,              "output audio sample rate"},
    { ID_A_OUTPUT_CHANNELS, "och",      "outchannels",  ItemTypeInt,    0,              "output number of channels"},

    // container args
    { IDI_C_TITLE_ID,       "tid",         "titleid",   ItemTypeInt,    0,              "title id"},
    { IDI_C_STREAM_ID,      "sid",         "streamid",  ItemTypeInt,    0,              "stream id"},
    { IDI_C_MPD_PROFILE,    "mpd_profile", "",          ItemTypeString, 0,              "MPD profile"},
	{ IDS_C_DMF_PROFILE,    "",          "dmf_profile", ItemTypeString, 0,              "dmf mux profile name"},

    // network args
    { IDS_NET_IP,           "ip",       "ipaddress",    ItemTypeString, 0,              "network IP address"},
    { IDI_NET_PORT,         "port",     "portnumber",   ItemTypeInt,    0,              "network port number"},

    // mode args
    { IDI_MODE,             "m",        "mode",         ItemTypeInt,    0,              "mode"},
	{ IDI_GUI_MODE,         "gui",      "",             ItemTypeInt,    0,              "gui mode"},
    { IDI_NOMUX,            "nm",       "nomux",        ItemTypeNoArg,  1,              "don't mux encoder output"},

    // 3D
    { IDI_3D_NUM_VIEWS,     "nv",       "numviews",     ItemTypeInt,    0,              "number of views"},
    { IDI_3D_SPLIT_VIEWS,   "",         "",             ItemTypeNoArg,  0,              "do not split output views"},
    { IDI_3D_SPLIT_VIEWS,   "split",    "splitviews",   ItemTypeNoArg,  1,              "split output views"},
    { IDI_TRANSMISSION_FORMAT,   "",    "",             ItemTypeNoArg,  0,              "sequential video"},
    { IDI_TRANSMISSION_FORMAT,   "side","sidebyside",   ItemTypeNoArg,  1,              "side-by-side video"},
    { IDI_TRANSMISSION_FORMAT,   "top", "topandbottom", ItemTypeNoArg,  2,              "top-and-bottom video"},

	//hardware acceleration
	{ IDI_HWACC_MODE,		"",		"",					ItemTypeNoArg, 0,			"hardware acceleration type"},
	{ IDI_HWACC_MODE,		"dxva2",		"",			ItemTypeNoArg, 2,			"use dxva2 hardware acceleration mode"},
	{ IDI_HWACC_MODE,		"nvcuvid",		"",			ItemTypeNoArg, 4,			"use nvcuvid hardware acceleration mode"},


	//decoders supported hardware acceleration
	{ IDI_MC_HWACC_DECODER,	"",		"",					ItemTypeNoArg, 0,			"decoder supported hardware acceleration type"},
	{ IDI_MC_HWACC_DECODER,	"mpeg2","",					ItemTypeNoArg, 1,			"use mpeg2 decoder"},
	{ IDI_MC_HWACC_DECODER,	"vc1",	"",					ItemTypeNoArg, 2,			"use vc1 decoder"},
	{ IDI_MC_HWACC_DECODER,	"avc",	"",					ItemTypeNoArg, 3,			"use avc decoder"},


	{ IDN_LOOPPLAY,         "loop",   "",               ItemTypeNoArg, 1,           "loop play"},
    { IDI_SILENT_MODE,      "silent", "",               ItemTypeNoArg, 1,           "silent mode"},
    { IDI_ERRCONCEAL_MODE,  "err",    "",               ItemTypeNoArg, 1,           "err conceal mode"},
    { IDS_BINDIR_PATH,      "bd",     "bindir",         ItemTypeString, 1,          "path to SDK dir"},
    { IDS_LIC_KEYGEN,       "kf",     "keyfile",        ItemTypeString, 1,          "keys file"},
    { IDS_LIC_FILE,         "lf",     "licfile",        ItemTypeString, 1,          "license file"},
    { IDS_LIC_DBGLEVEL,     "dbg",     "debug",         ItemTypeInt, 1,             "DBG level"},
    { IDN_LIC_MAKEALL,      "makeall", "",              ItemTypeNoArg, 1,        ""},
    
};

static const int32_t g_arg_items_cnt = sizeof(g_arg_items) / sizeof(g_arg_items[0]);


static int32_t invalidate_params(int32_t dstc, const arg_item_t * dst, const arg_item_desc_t *custom_args /* = NULL */, int32_t custom_args_cnt /* = 0 */)
{
    for (int32_t i = 0; i < dstc; i++)
    {
        const arg_item_desc_t * item_desc = NULL;

        // first of all search in custom the custom component-defined argument list if it's available
        if (custom_args != NULL)
        {
            for (int32_t d = 0; d < custom_args_cnt; d++)
            {
                if (dst[i].id == custom_args[d].id)
                {
                    item_desc = custom_args + d;
                    break;
                }
            }
        }

        if (item_desc == NULL)
        {
            for (int32_t d = 0; d < g_arg_items_cnt; d++)
            {
                if (dst[i].id == g_arg_items[d].id)
                {
                    item_desc = g_arg_items + d;
                    break;
                }
            }
        }

        if (item_desc == NULL)
        {
            printf("Error: application expects unknown argument (id = %d)\n", dst[i].id);
            return -1;
        }
        else
        {
            switch (item_desc->type)
            {
            case ItemTypeNoArg:
            case ItemTypeInt:
                *(int32_t *)dst[i].dst = ITEM_NOT_INIT;
                break;

            case ItemTypeDouble:
                *(double *)dst[i].dst = (double)ITEM_NOT_INIT;
                break;

            case ItemTypeString:
                *(char **)dst[i].dst = NULL;
                break;
            }
        }
    }

    return 0;
}


static int32_t check_params(int32_t dstc, arg_item_t * dst, const arg_item_desc_t *custom_args /* = NULL */, int32_t custom_args_cnt /* = 0 */)
{
    int32_t i, d;
    int32_t uninit_params = 0;

    for (i = 0; i < dstc; i++)
    {
        if (dst[i].mandatory)
        {
            const arg_item_desc_t * item_desc = NULL;

            // first of all search in custom the custom component-defined argument list if it's available
            if (custom_args != NULL)
            {
                for (int32_t d = 0; d < custom_args_cnt; d++)
                {
                    if (dst[i].id == custom_args[d].id)
                    {
                        item_desc = custom_args + d;
                        break;
                    }
                }
            }

            if (item_desc == NULL)
            {
                for (d = 0; d < g_arg_items_cnt; d++)
                {
                    if (dst[i].id == g_arg_items[d].id)
                    {
                        item_desc = g_arg_items + d;
                        break;
                    }
                }
            }

            if (item_desc == NULL)
            {
                printf("Error: application expects unknown argument (id = %d)\n", dst[i].id);
                return -1;
            }
            else
            {
                int32_t err = 0;

                switch (item_desc->type)
                {
                case ItemTypeNoArg:
                case ItemTypeInt:
                    err = (*(int32_t *)dst[i].dst == ITEM_NOT_INIT);
                    break;

                case ItemTypeDouble:
                    err = ((int32_t)*(double *)dst[i].dst == ITEM_NOT_INIT);
                    break;

                case ItemTypeString:
                    err = (*(char **)dst[i].dst == NULL);
                    break;
                }

                if (err)
                {
                    uninit_params++;

                    if (item_desc->type == ItemTypeNoArg)
                    {
                        uninit_params++;

                        printf("Error: %s not set. Available values:\n", item_desc->desc);

                        while (++d < g_arg_items_cnt && item_desc->id == g_arg_items[d].id)
                        {
                            printf("%s%s\t%s\n", ITEM_PREFIX, g_arg_items[d].name[0], g_arg_items[d].desc);
                        }
                    }
                    else
                    {
                        printf("Error: %s not set (%s%s)\n", item_desc->desc, ITEM_PREFIX, item_desc->name[0]);
                    }
                }
            }
        }
    }

    return (uninit_params > 0) ? -1 : 0;
}


int32_t parse_args(int32_t argc, char * argv[], int32_t dstc, arg_item_t * dst, const arg_item_desc_t *custom_args /* = NULL */, int32_t custom_args_cnt /* = 0 */)
{
    int32_t i = 0;
    int32_t arg_idx = 0;

    if (invalidate_params(dstc, dst, custom_args, custom_args_cnt) < 0)
        return -1;

    while (arg_idx < argc)
    {
        if (strstr(argv[arg_idx], ITEM_PREFIX) != argv[arg_idx])
        {
            printf("Wrong argument (prefix \"%s\" expected): %s\n", ITEM_PREFIX, argv[arg_idx++]);
            continue;
        }

        const char * arg_name = argv[arg_idx] + strlen(ITEM_PREFIX);
        const arg_item_desc_t * item_desc = NULL;

        // first of all search in custom the custom component-defined argument list if it's available
        if (custom_args != NULL)
        {
            for (int32_t i = 0; i < custom_args_cnt; i++)
            {
                if (strcmp_lower_case(custom_args[i].name[0], arg_name) == 0 ||
                    strcmp_lower_case(custom_args[i].name[1], arg_name) == 0)
                {
                    item_desc = custom_args + i;
                    break;
                }
            }
        }

        if (item_desc == NULL)
        {
            for (i = 0; i < g_arg_items_cnt; i++)
            {
                if (strcmp_lower_case(g_arg_items[i].name[0], arg_name) == 0 ||
                    strcmp_lower_case(g_arg_items[i].name[1], arg_name) == 0)
                {
                    item_desc = g_arg_items + i;
                    break;
                }
            }
        }

        if (item_desc == NULL)
        {
            for (i = 0; i < dstc; i++)
            {
                if (dst[i].id == IDS_CUSTOM_ARG)
                    item_desc = g_arg_items + IDS_CUSTOM_ARG;
                // this allows to handle unknown arguments privately in the application
            }
        }

        if (item_desc == NULL)
        {
            printf("Unknown argument: %s\n", argv[arg_idx++]);
            continue;
        }

        arg_item_t * item = NULL;
        int32_t excessive_arg = 0;

        for (i = 0; i < dstc; i++)
        {
            if (dst[i].id == item_desc->id)
            {
                int32_t empty = 1;

                switch (item_desc->type)
                {
                case ItemTypeNoArg:
                case ItemTypeInt:
                    empty = (*(int32_t *)dst[i].dst == ITEM_NOT_INIT);
                    break;

                case ItemTypeDouble:
                    empty = ((int32_t)*(double *)dst[i].dst == ITEM_NOT_INIT);
                    break;

                case ItemTypeString:
                    empty = (*(char **)dst[i].dst == NULL);
                    break;
                }

                if (empty)
                {
                    item = dst + i;
                    break;
                }
                else
                {
                    excessive_arg = 1;
                }
            }
        }

        if (item == NULL && !excessive_arg)
        {
            printf("Unexpected argument: %s (%s)\n", argv[arg_idx], item_desc->desc);
            return -1;
        }

        if (item != NULL)
        {
            switch (item_desc->type)
            {
            case ItemTypeNoArg:
                *(int32_t *)item->dst = item_desc->value;
                break;

            case ItemTypeInt:
                if (++arg_idx < argc)
                    *(int32_t *)item->dst = atoi(argv[arg_idx]);
                break;

            case ItemTypeDouble:
                if (++arg_idx < argc)
                    *(double *)item->dst = atof(argv[arg_idx]);
                break;

            case ItemTypeString:
                if( item->id == IDS_CUSTOM_ARG )
                {
                    if (arg_idx < argc)
                        *(char **)item->dst = argv[arg_idx];
                }
                else
                {
                    if (++arg_idx < argc)
                        *(char **)item->dst = argv[arg_idx];
                }
                break;
            }

            if (arg_idx >= argc)
            {
                printf("Incomplete argument: \"%s\" (%s) requires a value\n", argv[argc - 1], item_desc->desc);
                return -1;
            }
        }
        else
        {
            switch (item_desc->type)
            {
            case ItemTypeNoArg:
                printf("Warning: excessive argument \"%s\" will be ignored\n", argv[arg_idx]);
                break;

            case ItemTypeInt:
            case ItemTypeDouble:
            case ItemTypeString:
                if (++arg_idx >= argc)
                    printf("Warning: incomplete excessive argument \"%s\" (%s) will be ignored\n", argv[argc - 1], item_desc->desc);
                else
                    printf("Warning: excessive argument \"%s %s\" will be ignored\n", argv[arg_idx - 1], argv[arg_idx]);
                break;
            }
        }

        arg_idx++;
    }

    return check_params(dstc, dst, custom_args, custom_args_cnt);
}


void waitForAnyKey( const char* text )
{
#if !defined(__linux__) && !defined(__APPLE__)
    printf_s( text ? text : "\nPress any key to continue ...\n" );
    while( 1 )
    {
        if( _kbhit() )
        {
			char ch = _getch();
			if ( ch == 0 || ch == -32 ) {
				_getch(); // read second part of functional keys
			}
            break;
        }
    }
#endif
}



int map_param_name_to_index(char *s, mapparam_t* param_map, size_t param_num) 
{
	size_t i = 0;
	while (i < param_num && param_map[i].name != NULL)
		if (0==strcmp(param_map[i].name, s))
			return i;
		else
			i++;
	return -1;
}

#define MAX_PARAM_NUM 100
int parse_config_params(char *buf, int len, mapparam_t* param_map, size_t param_num) 
{
	size_t i;
	int  init_flag[MAX_PARAM_NUM*3];
	char *param[MAX_PARAM_NUM*3];
	size_t params = 0;
	char *bufend = &buf[len];
	char *p = buf;
	int	inparam;
	
	memset(init_flag, 0, sizeof(init_flag));
	
	while (p < bufend)
	{
		switch (*p)
		{
		case '#':                 // Found comment
			*p = '\0';              // Replace '#' with '\0' in case of comment immediately following integer or string
			while (*p != '\n' && p < bufend)  // Skip till EOL or EOF, whichever comes first
				p++;
			inparam = 0;
			break;
        case 10:
		case 13:
			inparam = 0;
			*p++='\0';
			break;
		case ' ':
		case '\t':              // Skip whitespace, leave state unchanged
			*p++ = '\0';
			inparam = 0;
			break;
		case '"':               // Begin/End of String
			*p++ = '\0';
			break;
		default:
			if (!inparam)
			{
				param[params++] = p;
				inparam = ~inparam;
			}
			p++;
		}
	}

	for( i = 0; i<params; i++ )
	{
		int index = map_param_name_to_index(param[i], param_map, param_num);
		if( index >= 0 ) 
		{
			if( param_map[index].type == ItemTypeString ) 
			{
                if( !strcmp((char* )param_map[index].name,"FourCC") ) {
                    int32_t stride, img_start;
                    *(int* )param_map[index].place = MAKEFOURCC(param[i+2][0],param[i+2][1],param[i+2][2],param[i+2][3]);
                    if(!get_video_frame_size(128, 128, *(int* )param_map[index].place, &stride, &img_start))
                        *(int* )param_map[index].place = (int)param_map[index].def_val;

                }else
                {
				    strncpy((char* )param_map[index].place,param[i+2], CONFIG_STRING_TYPE_MAX_LEN);
                }
			}
			else if( param_map[index].type == ItemTypeInt ) 
			{
				int v = atoi(param[i+2]);
				if( param_map[index].param_limit ) 
				{
					if( v > param_map[index].max_limit ||  v < param_map[index].min_limit ) 
					{
						printf("\nout of range value of %s. set default %d", param[i], (int)param_map[index].def_val );
						v = (int)param_map[index].def_val;
					}
				}
				*(int* )param_map[index].place = v;
			} 
			else 
			{ // double
				double v = atof(param[i+2]);
				if( param_map[index].param_limit ) 
				{
					if( v > param_map[index].max_limit ||  v < param_map[index].min_limit ) 
					{
						printf("\nout of range value of %s. set default %f",param[i],param_map[index].def_val );
						v = param_map[index].def_val;
					}
				}
				*(double* )param_map[index].place = v;
			}
			init_flag[index] = 1;
            i+=2;
		}
		else 
		{
			printf("\nunknow parameter: %s",param[i]);
		}
	}

	// init unseted params
	for( i=0; i<param_num; i++ ) 
	{
		if( !init_flag[i] ) 
		{
			if( param_map[i].type == ItemTypeString ) 
			{
				*(char*)param_map[i].place = '\0';
			}
			else if(param_map[i].type == ItemTypeInt) 
			{
				*(int* )param_map[i].place = (int)(param_map[i].def_val);
			}
			else  //double
			{
				*(double* )param_map[i].place = param_map[i].def_val;
			}
		}
	}
	return 1;
}
