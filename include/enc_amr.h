/**
 @file  enc_amr.h
 @brief AMR Encoder API

 @verbatim
 File: enc_amr.h

 Desc: AMR Encoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/


#if !defined (__ENC_AMR_API_INCLUDED__)
#define __ENC_AMR_API_INCLUDED__

#include "mctypes.h"
#include "bufstrm.h"
#include "mcapiext.h"

/**
 * @name AMR output return codes
 * @{
 **/
#define amrOutErrNone   0       ///< no error
#define amrOutCancel    1       ///< user canceled settings
/** @} */

/**
 * @name AMR-NB bitrates
 * @{
 **/
#define AMR_475 0		///< 4.75 kbits/sec
#define AMR_515 1		///< 5.15 kbits/sec
#define AMR_59  2		///< 5.9 kbits/sec
#define AMR_67  3		///< 6.7 kbits/sec
#define AMR_74  4		///< 7.4 kbits/sec
#define AMR_795 5		///< 7.95 kbits/sec
#define AMR_102 6		///< 10.2 kbits/sec
#define AMR_122 7		///< 12.2 kbits/sec
#define AMR_DTX 8		///< DTX
/** @} */

/**
 * @name AMR-WB bitrates
 * @{
 **/
#define AMRWB_660  0		///< 6.6 kbits/sec
#define AMRWB_885  1		///< 8.85 kbits/sec
#define AMRWB_1265 2		///< 12.65 kbits/sec
#define AMRWB_1425 3		///< 14.25 kbits/sec
#define AMRWB_1585 4		///< 15.85 kbits/sec
#define AMRWB_1825 5		///< 18.25 kbits/sec
#define AMRWB_1985 6		///< 19.85 kbits/sec
#define AMRWB_2305 7		///< 23.05 kbits/sec
#define AMRWB_2385 8 		///< 23.85 kbits/sec
/** @} */

/////////////////////////////////////////////////////////////////////////////
// API structures
/////////////////////////////////////////////////////////////////////////////


/// @brief encoder instance type
typedef struct amr_enc amr_enc_tt;


/// @brief encoder settings type
typedef struct amr_a_settings amr_a_settings_tt;

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/**
 * @brief These parameters are used to configure the encoder.
 */
struct amr_a_settings
{
  int bitrate_mode;   		///<  AMR bitrate mode
  int dtx;            		///<  DTX mode
  int write_file_header;	///<  Enables/disables writing a file header to the output stream
  int interface_format;		///<  Output bitstream mode
  uint32_t reserved[1020];
};

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// API functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

	/** @brief Call this function to fill an @ref amr_a_settings structure with
	 * default values.
	 *
	 * @param[in] set a pointer to a @ref amr_a_settings structure to be initialized
	 * @param[in] bitrate_mode AMR bitrate mode.
	 *				Supported values:
	 *				- for sample rate = 8 kHz are @ref AMR_475 .. @ref AMR_DTX.\n
	 *				- for sample rate = 16 kHz are @ref AMRWB_660 .. @ref AMRWB_2385.
	 *
	 * @param[out] set modified amr_a_settings structure
	 */
    void MC_EXPORT_API amrOutAudioDefaults(struct amr_a_settings *set, int bitrate_mode);

	/** @brief Call this function to fill an @ref amr_a_settings structure with
	 * default values based on a profile type.
	 *
	 * @param[in] set a pointer to a @ref amr_a_settings structure to be initialized
	 * @param[in] profile one of the preset values for profile. This field specifies
	 * which default values will be used. It should be one of the following defines:
	 * <table>
	 * <tr><th>value</th><th>meaning</th></tr>
	 * <tr><td>@ref MCPROFILE_DEFAULT</td><td>default settings</td></tr>
	 * <tr><td>@ref MCPROFILE_3GPP</td><td>settings compatible with 3GPP specification</td></tr>
	 * <tr><td>@ref MCPROFILE_3GPP2</td><td>settings compatible with 3GPP2 specification</td></tr>
	 * </table>
	 *
	 * @param[out] set modified amr_a_settings structure
	 * @return short description if the ID is valid
	 * @return NULL if not
	 */
	char* amrOutAudioDefaultsEx( struct amr_a_settings *set, int profile, int reserved1, int reserved2)	;


	/** @brief Call this function to get the setting errors/warnings in an @ref amr_a_settings structure
	 * use with the get_rc parameter to supply an err_printf callback to get error messages that can be localized.
	 *
	 * @param[in] get_rc a pointer to a get_rc function
	 * @param[in] set a pointer to a filled in @ref amr_a_settings structure
	 * @param[in] sample_rate sampling rate to check
	 * @param[in] app reserved, set to NULL
	 * @return @ref amrOutErrNone if settings are valid
	 */
    int MC_EXPORT_API amrOutAudioChkSettings(void *(MC_EXPORT_API * get_rc)(const char* name), const struct amr_a_settings *set, int sample_rate, void *app);

	/** @brief Call this function to create an AMR audio encoder object.
	 *
	 * @param[in] get_rc pointer to a get resource function
	 * @param[in] set pointer to a filled in @ref amr_a_settings structure
	 * @param[in] options reserved, set to 0
	 * @param[in] CPU set to 0xFFFFFFFF for autodetect (reserved for CPU-flags)
	 * @param[in] sample rate of the input audio, must be 8 or 16 kHz
	 * @return pointer to a @ref amr_enc_tt object if succesful
	 * @return NULL if unsuccesful
	 */
    amr_enc_tt * MC_EXPORT_API amrOutAudioNew(void *(MC_EXPORT_API * get_rc)(const char* name),const struct amr_a_settings *set,int options,int CPU,int sample_rate);

	/** @brief Call this function to free an AMR audio encoder object
	 *
	 * @param[in] instance a pointer to a @ref amr_enc_tt object
	 */
    void MC_EXPORT_API amrOutAudioFree(amr_enc_tt* instance);


	/** @brief Call this function to initialize the AMR encoder for an encoding session.
	 *
	 * Function must be called before the PutBytes function is called.
	 *
	 * @param[in] instance a pointer to a @ref amr_enc_tt object
	 * @param[in] audiobs a pointer to a @ref bufstream_tt object for the compressed
	 * data output stream
	 * @return @ref amrOutErrNone if successful
	 * @return @ref amrOutCancel if not
	 */
    int MC_EXPORT_API amrOutAudioInit(amr_enc_tt *instance, bufstream_tt *audiobs);


	/** @brief Call this function to encode some audio samples.
	 *
	 * @param[in] instance pointer to a @ref amr_enc_tt object
	 * @param[in] audioBuffer pointer to a buffer of PCM audio samples
	 * @param[in] numAudioBytes number of bytes of data in the audio buffer
	 * @return @ref amrOutErrNone if successful
	 * @return @ref amrOutCancel if not
	 */
    int MC_EXPORT_API amrOutAudioPutBytes(amr_enc_tt *instance, unsigned char *audioBuffer, unsigned long numAudioBytes);


	/** @brief Call to finish an AMR audio encoding session and write encoded samples to the audiobs
	 * object if needed, set the abort flag to a non-zero if audio encoding is being aborted.
	 *
	 * @param[in] instance a pointer to a @ref amr_enc_tt object
	 * @param[in] abort flag, set to a non-zero value to abort encoding
	 * @return @ref amrOutErrNone if successful
	 * @return @ref amrOutCancel if not
	 */
    int MC_EXPORT_API amrOutAudioDone(amr_enc_tt *instance, int abort);


    APIEXTFUNC MC_EXPORT_API amrOutAudioGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif // #if !defined (__ENC_AMR_API_INCLUDED__)

