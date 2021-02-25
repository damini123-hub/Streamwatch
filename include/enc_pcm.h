/* ----------------------------------------------------------------------------
 * File: enc_pcm.h
 *
 * Desc: PCM Encoder API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__ENC_PCM_API_INCLUDED__)
#define __ENC_PCM_API_INCLUDED__

#include "bufstrm.h"
#include "mcdefs.h"
#include "mcprofile.h"
#include "mcapiext.h"

typedef struct pcm_a_encoder pcmaenc_tt;  // PCM audio encoder object

// use these with the option parameter in the pcmOutAudioNew function

// if the audio samples being input to the encoder are already
// byte swapped (LPCM) samples instead of normal PCM samples.
#define LPCM_SAMPLES      0x00000001  // for compatiblity
#define OPT_LPCM_SAMPLES  0x00000001


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct pcm_a_settings
{
    int32_t pcm_channels;               // one of the PCM_?CHANNEL defines in mcdefs.h
    int32_t audio_layer;                // to overlay with mpeg audio layer field
    int32_t pcm_emphasis;               // 0 - off, 1 - on, on only allowed for 48 kHz audio, DVD only
    int32_t pcm_mute_flag;              // 0 - off, 1 - on, DVD only
    int32_t pcm_quantization;           // one of the PCM_??BITS defines in mcdefs.h
    int32_t pcm_dynamic_range_control;  // 8 bit value, 0x80 (the default) = no range control (gain)
                                        // X = bits 7..5, Y = bits 4..0 where
                                        // gain (dB) = 24.082 - 6.0206*X - 0.2007*Y, DVD only

    int64_t pts_offset;                 // pts of the start of the stream
    int32_t pts_units;                  // the units of the pts_offset field
                                        // 10000000 = 10MHz, 27000000 = 27Mhz, etc
                                        // a value <= 0 will be treated as 27MHz

    int32_t reserved[1015];
};
#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// audio routines in enc_pcm.dll
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// call to fill a pcm_a_settings-structure with defaults values
// based on one of the MPEG preset values
//
//  inputs:
//    set - pointer to a pcm_a_settings structure
//    profileID - one of the MCPROFILE_* or MPEG_* constants
// output:
//    modified pcm_a_settings structure
//  return:
//    profile description if valid or NULL else

char * MC_EXPORT_API pcmOutAudioDefaults(struct pcm_a_settings *set, int32_t profileID);


// call to create a pcm audio encoder object
//
//  inputs:
//    get_rc - pointer to a get resource function
//    set - pointer to a filled in pcm_a_settings structure
//    options - one or more of the OPT_ defines above
//    frameRateCode - one of FRAMERATE defines in mcdefs.h, required for
//                    AES3 audio types, not used for other PCM types
//    sampleRate - sample rate of the input audio, must be 48000 or 96000
// outputs:
//    none
//  return:
//    pointer to a pcmaenc_tt object if succesful
//    NULL if unsuccesful

pcmaenc_tt * MC_EXPORT_API pcmOutAudioNew(void                        *(MC_EXPORT_API *get_rc)(const char* name),
                                          const struct pcm_a_settings *set,
                                          int32_t                      options,
                                          int32_t                      frameRateCode,
                                          int32_t                      sampleRate);


// call to free a pcm audio encoder object
//
//  inputs:
//    instance - audio encoder object to free
// outputs:
//    none
//  return:
//    none

void MC_EXPORT_API pcmOutAudioFree(pcmaenc_tt *instance);


// call to initialize the audio encoder for an encoding session, this
// function must be called before the PutBytes function is called
//
//  inputs:
//    instance - pointer to a pcm audio encoder object
//    audiobs - pointer to a bufstream_tt object for the compressed data
// outputs:
//    none
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t MC_EXPORT_API pcmOutAudioInit(pcmaenc_tt *instance, bufstream_tt *audiobs);


// call to process some audio samples
//
//  inputs:
//    instance - pointer to a pcm audio encode object
//    audioBuffer - pointer to a buffer of PCM audio samples
//    numAudioBytes - number of bytes of data in the audio buffer
// outputs:
//    encoded samples to the audiobs object
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t MC_EXPORT_API pcmOutAudioPutBytes(pcmaenc_tt *instance, uint8_t *audioBuffer, uint32_t numAudioBytes);


// call to process some audio samples with a timestamp
//
//  inputs:
//    instance - pointer to a pcm audio encode object
//    audioBuffer - pointer to a buffer of PCM audio samples
//    numAudioBytes - number of bytes of data in the audio buffer
//    timestamp - the timestamp (PTS) of the first sample in the buffer
//    ts_units - the units of the timestamp field
//               10000000 = 10MHz, 27000000 = 27Mhz, etc
//               a value <= 0 will be treated as 27MHz
// outputs:
//    encoded samples to the audiobs object
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t MC_EXPORT_API pcmOutAudioPutBytesEx(pcmaenc_tt *instance, uint8_t *audioBuffer, uint32_t numAudioBytes, uint64_t timestamp, int32_t ts_units);


// call to finish encoding session, set abort non-zero if encoding is being
// aborted.
//
//  inputs:
//    instance - pointer to a pcm audio encoder object
//    abort - set to 0 to finish any leftover audio and clean up,
//            else just clean up
// outputs:
//    encoded samples to the audiobs object if needed
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t MC_EXPORT_API pcmOutAudioDone(pcmaenc_tt *instance, int32_t abort);


// call to get the setting errors/warnings in a pcm_a_settings structure
// use the get_rc callback to provide an err_printf callback to get error
// messages that can be localized
//
//  inputs:
//    get_rc - pointer to a get_rc function
//    set - pointer to a pcm_a_settings structure
//    mpeg_type - one of the MCPROFILE_* or MPEG_* constants to check for format
//                compliance, use 0 for no format compliance checks
//    sample_rate - sampling rate to check
//    options - check options, one or more of the CHECK_* defines in mcdefs.h
//    app - reserved
// outputs:
//    none
//  return:
//    mpegOutErrNone if no errors found
//    one of the INV_* error codes if an error is found

int32_t MC_EXPORT_API pcmOutAudioChkSettings(void                        *(MC_EXPORT_API *get_rc)(const char* name),
                                             const struct pcm_a_settings *set,
                                             int32_t                      mpeg_type,
                                             int32_t                      sample_rate,
                                             uint32_t                     options,
                                             void                        *app);


// call to get extended API function
//
//  inputs:
//    func - function ID
//  return:
//    function pointer or NULL

APIEXTFUNC MC_EXPORT_API pcmOutAudioGetAPIExt(uint32_t func);


#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------

#endif // #if !defined (__ENC_PCM_API_INCLUDED__)

