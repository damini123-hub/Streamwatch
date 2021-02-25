/* ----------------------------------------------------------------------------
 * File: drm_encryption.h
 *
 * Desc: DRM protection declaration
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

/*!
    \file drm_encryption.h
    \brief Interface for DRM protection
*/

#if !defined (__DRM_ENCRYPTION_INCLUDED__)
#define __DRM_ENCRYPTION_INCLUDED__

#include "mctypes.h"
#include "mcmediatypes.h"

typedef struct drm_encryption_provider drm_encryption_provider_tt; //!< Protection scheme provider object

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/*!
    Describes protection scheme
*/
typedef enum _tagDrmEncryptionScheme {
    drmSchemeCommonEncryption,                                                               //!< present Common Encryption Scheme, ISO IEC 23001-7, 2012
    drmSchemeUltravioletEncryption                                                           //!< present Scheme described in CFF Media Format 1.0.3
} drmEncryptionScheme;

/*!
    Describes stream track
*/
typedef struct _tagDrmTrackDescriptor{

    mcmediatypes_t stream_type;                                                             //!< stream format
    uint32_t track_id;                                                                      //!< unique track identifier from muxer
} drmTrackDescriptor;

/*!
    Common encryption params, @see drmCENCEncryptParams
*/
typedef struct _tagDrmEncryptParams {
    void* pEncryptContext;                                                                  //! Interpretation depends on encryption scheme
} drmEncryptParams;

/*!
    Common encryption specific parameters
*/
typedef struct _tagDrmCENCEncryptParams
{
    uint32_t IsEncrypted;                                                                   //!< Enables/Disables encrypt(track or sample specific)
    uint8_t IV_size;                                                                        //!< Initializaton Vector size(0, 8, 16 bytes)
    uint8_t IV[16];                                                                         //!< Initialization vector
    uint8_t KID[16];                                                                        //!< Key ID, See CENC specs
    uint8_t Key[16];                                                                        //!< Encryption Key, not stored inside of container
} drmCENCEncryptParams;

/*!
    Defines stream unit type.

    Muxer separates stream on units and passes them for encryption. Unit may contain data or be simple marker.
    AAC stream: { drmUnitNextFrame; drmUnitPayload; drmUnitNextFrame; drmUnitPayload; drmUnitEOS;};
    AVC stream: { drmUnitNextFrame; drmUnitKeyFrame; drmUnitSubsample; drmUnitPayload; drmUnitSubsample; drmUnitPayload; drmUnitSubsample; drmUnitPayload; drmUnitEOS; };
*/
typedef enum _tagDrmStreamUnitType {

    drmUnitPayload,                                                                         //!< Media payload, contains media data buffer
    drmUnitSubsample,                                                                       //!< Subsample marker(AVC NAL unit), not contain data
    drmUnitNextFrame,                                                                       //!< New data frame marker, not contain data
    drmUnitKeyFrame,                                                                        //!< Key frame marker, not contain data
    drmUnitEOS                                                                              //!< End of stream marker, last marker during encryption proccess, not contain data
} drmStreamUnitType;

/*!
   Describes protection scheme
*/
typedef struct _tagdrmEncryptionDescriptor {

    drmEncryptionScheme scheme;                                                             //!< Protection scheme
    uint8_t BaseLocation[256];                                                              //!< Base location server, must be a UTF-8 null-terminated string, see CFF 1.0.4, ch. 2.2.3; should be next format "[<retailersub>.]<retailer>.<decedomain>"
    uint8_t PurchaseLocation[256];                                                          //!< Purchase location server, must be a UTF-8 null-terminated string, see CFF 1.0.4, ch. 2.2.3; should be next format "http://purchase.<basePurlLocation>.<decedomain>/index.html?apid=<APID>", where APID equal APID in AINF atom
    void* pEncryptContext;                                                                  //!< Protection scheme specific data, @see drmCENCEncryptionDescriptor
} drmEncryptionDescriptor;

/*!
   Common encryption scheme specific data, 'pssh' atom contents
*/
typedef struct _tagdrmCENCEncryptionDescriptor{

    uint8_t SystemID[16];                                                                   //!< System ID, ISO IEC 23001-7, 2012, chapter 8.1.2
    uint8_t* pData;                                                                         //!< Encryption specific data
    uint32_t uiDataSize;                                                                    //!< Length of specific data
} drmCENCEncryptionDescriptor;

/*!
   Error codes for DRM-related functions.
*/
typedef enum _tagDrmError {

    drmOK,                                                                                  //!< Return DRM success.
    drmError,                                                                               //!< Return DRM failure.
} drmErrorCode;

/*!
   Subsample information.
*/
typedef struct _tagdrmSubSampleUnit {

    uint32_t clearSize;                                                                     //!< Number of unencrypted bytes.
    uint32_t encryptSize;                                                                   //!< Number of encrypted bytes.
}drmSubSampleUnit;

/*!
   Encrypted sample information.
*/
typedef struct _tagdrmEncryptSample {

    uint8_t* pData;                                                                         //!< Actual encrypted data
    uint32_t uiLen;                                                                         //!< Encrypted data size.

    uint32_t subsample_count;                                                               //!< Number of subsamples.
    drmSubSampleUnit* subsamples;                                                           //!< Subsamples array.

    drmEncryptParams sample_specific_encrypt_data;                                          //!< Scheme-specific encryption parameters
} drmEncryptSample;

typedef struct drm_encryption_applier_s drm_encryption_applier_tt;

/*!
    @brief Encryption applier structure.

    @details This structure is used to provide custom encryption applier to the system.
    It should be passed into OnTrackEncryptRequest callback as pApplier parameter (@see OnTrackEncryptRequest).

    @note If application uses external applier OnStreamUnitEncrypt callback will not be called
    (and can be set to NULL in drmEncryptionSettings structure).
    In this case application gets all the necessary information through encrypt function.
 */
struct drm_encryption_applier_s
{
    /*!
     Encrypt specified data.
     */
    drmErrorCode ( MC_EXPORT_API * encrypt)( drm_encryption_applier_tt* applier, const uint8_t* const pData, uint32_t uiLen, drmStreamUnitType type, drmEncryptSample* pEncBlock );
    /*!
     Release additional data associated with encrypted data block.
     */
    drmErrorCode ( MC_EXPORT_API * release_encrypted_sample)( drm_encryption_applier_tt* applier, drmEncryptSample* pEncBlock );
    /*!
     Return default encryption parameters associated with current track.
     */
    drmErrorCode ( MC_EXPORT_API * get_default_encrypt_params)( drm_encryption_applier_tt* applier, drmEncryptParams* pTrackDefaultEncryptData );
    /*!
     Release specified custom encryption applier.
     */
    void ( MC_EXPORT_API * release)( drm_encryption_applier_tt* pApplier );
};

/*!
    Protection process parameters
*/
typedef struct _tagDrmEncryptionSettings {

    drmEncryptionDescriptor scheme_descriptor;                                              //!< Encryption descriptor
    void* application_context;

    /*!
        Called on track adding. Application should define encrypt params for each track

        @param[in] encrypt_provider  pointer to protection provider
        @param[in] pTrack declares stream track
        @param[out] pTrackDefault encryption config(track specific)

        @return  0 if successful
    */
    drmErrorCode  (MC_EXPORT_API * OnTrackEncryptRequest)( drm_encryption_provider_tt* encrypt_provider, const drmTrackDescriptor* pTrack, drmEncryptParams* pEncryptParams, drm_encryption_applier_tt** pApplier, void* pApplicationContext );

    /*!
        Called on each stream unit receiving. Application may decide to change encryption params depends on unit type

        @param[in] pTrack declares stream track
        @param[in] unit stream unit type
        @param[out] pNewConfig new encryption config(sample or unit specific)

        @return  0 if successful
    */
    drmErrorCode  (MC_EXPORT_API * OnStreamUnitEncrypt)( drmTrackDescriptor* pTrack, drmStreamUnitType unit, drmEncryptParams* pNewConfig, void* pApplicationContext );

} drmEncryptionSettings;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

    /*!
        Call this method to create an protection provider object

        @param[in] pSettings  pointer to protection provider settings

        @return pointer to the object that represents protection protection instance, NULL if creation was failed
    */
    drm_encryption_provider_tt* MC_EXPORT_API drmCreateEncryptionProvider( const drmEncryptionSettings* const pSettings, void *(MC_EXPORT_API *get_rc)(const char* name) );

    /*!
        Call this method to destroy an protection provider object and release resource

        @param[in] pProvider  pointer to protection provider

        @return none
    */
    void MC_EXPORT_API drmFreeEncryptionProvider( drm_encryption_provider_tt* pProvider );

#ifdef __cplusplus
}
#endif


#endif

