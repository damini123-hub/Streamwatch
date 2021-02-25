/* ----------------------------------------------------------------------------
 * File: mux_mxf.h
 *
 * Desc: MXF Muxer API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

/*! @file /mux_mxf.h
 */

#if !defined (__MUX_MXF_API_INCLUDED__)
#define __MUX_MXF_API_INCLUDED__

#include "bufstrm.h"
#include "mcprofile.h"
#include "mcapiext.h"
#include "mcdefs.h"


//! @name MXF Muxer object
typedef struct mxf_muxer mxf_muxer_tt;    //!< muxer object

#define MAX_STREAMS					32		        //!< Max number of input streams

//! @name Profile types
//! @brief Specifies the type of MXF format to multiplex
//! Used with \ref  mxf_m_settings.profile field
//! @{
#define MXF_PROF_SONY_XDCAM         0     //!< Sony XDCAM and XAVC type files (frame-wrapped essences)
#define MXF_PROF_PANASONIC_P2       3     //!< Panasonic P2 type files (clip-wrapped essences, each stream in a separate file)
#define MXF_PROF_DCI_2K             6     //!< DCI 2K MXF files (frame-wrapped essences, each stream in a separate file)
#define MXF_PROF_DCI_4K             7     //!< DCI 4K MXF files (frame-wrapped essences, each stream in a separate file)
#define MXF_PROF_DEFAULT            8     //!< Generic MXF files (OP1a, OP1b, OPAtom, all supported essence types)
#define MXF_PROF_DNxHD				      9	    //!< DNxHD profile
//!@}


//! @name Multiplex sub types
//! @brief Specifies the type of MXF stream to multiplex
//! Used with \ref  mxf_m_settings.mplex_type field
//! @{
#define MXF_MUX_TYPE_XDCAM_HD       32		/*!< @brief XDCAM-HD compliant MPEG-2 video and AES-3 382M audio; OP1a SMPTE 378M.
													                  (Matches MPEG_XDCAM_HD in mcdefs.h) */
#define MXF_MUX_TYPE_XDCAM_IMX      28		/*!< @brief XDCAM IMX compliant MPEG D-10 video and AES-3 331M audio; OP1a SMPTE 378M.
													                 (Matches MPEG_XDCAM_IMX in mcdefs.h) */
#define MXF_MUX_TYPE_XDCAM_DV       99		/*!< @brief XDCAM DV compliant DV-DIF video and AES-3 382M audio; OP1a SMPTE 378M. */

#define MXF_MUX_TYPE_XDCAM_SXS_HD   50		/*!< @brief Professional Memory Card (SxS) XDCAM HD File Format */
#define MXF_MUX_TYPE_XDCAM_SXS_IMX  51		/*!< @brief Professional Memory Card (SxS) XDCAM IMX File Format */
#define MXF_MUX_TYPE_XDCAM_SXS_DV   52		/*!< @brief Professional Memory Card (SxS) XDCAM DV File Format */

#define MXF_MUX_TYPE_XAVC_SXS       53		/*!< @brief Professional Memory Card System (SxS) XAVC File Format.
                                               XAVC compliant AVC-Intra and AVC-LongGOP video and up to 16 channels of AES-3 382M audio.  */

#define MXF_MUX_TYPE_P2_DVCPRO      100		/*!< @brief P2 compliant DV/DVCPRO video and AES-3 382M audio; Specialized Operational Pattern Atom SMPTE 390M. */
#define MXF_MUX_TYPE_P2_AVCI        101		/*!< @brief P2 compliant AVC-Intra video and AES-3 382M audio; Specialized Operational Pattern Atom SMPTE 390M. */
#define MXF_MUX_TYPE_P2_AVC_LG      102		/*!< @brief P2 compliant AVC-LongGOP video and AES-3 382M audio; Op1b SMPTE 391M */


#define MXF_MUX_TYPE_J2K_DCI_2K     201		/*!< @brief DCI 2K compliant JPEG2000 video and AES-BWF 382M audio; Specialized Operational Pattern Atom SMPTE 390M. */
#define MXF_MUX_TYPE_J2K_DCI_4K     202		/*!< @brief DCI 4K compliant JPEG2000 video and AES-BWF 382M audio; Specialized Operational Pattern Atom SMPTE 390M. */

#define MXF_MUX_TYPE_DEFAULT        300		/*!< @brief Generic MXF files; all supported operational patterns, all supported essence types */
#define MXF_MUX_TYPE_DNxHD			    900		//!< DNxHD multiplex type
//! @}


//! @name Flags
//! @brief flags used with \ref  mxf_m_settings.flags field
//! @{
#define MXF_MUX_FLAG_DROP_FRAME       0x00000001	//!< @brief Use drop frame timecode.
													                        /*!< @details If the DV essence had timecode embeded
													                          this will be overwritten by the DF flag in the DIF frames timecode.*/

#define MXF_MUX_FLAG_XDCAM_11_COMP    0x00000002	//!< @brief force old XDCAM 1.1 spec. format.
													                        /*!< @details Only use if the output streams compatibilty with old applications that do not
													                          support 2008 1.2 specs w. XDCAM meta data correction and changed XDCAM HD index structure is needed.*/

//!< @brief Video flags for XDCAM FAM mode, which can not rewrite header metadata at end of muxing
//!< @details Use for calculating EditUnitByteCount into Index Table
#define MXF_MUX_FLAG_XDCAM_PAL			0x00000004	//!< @brief PAL video (XDCAM IMX 625/50, DVCAM 50i)
#define MXF_MUX_FLAG_XDCAM_NTSC			0x00000008	//!< @brief NTSC video (XDCAM IMX 525/60, DVCAM 60i)
#define MXF_MUX_FLAG_XDCAM_IMX30		0x00000010	//!< @brief IMX30
#define MXF_MUX_FLAG_XDCAM_IMX40		0x00000020	//!< @brief IMX40
#define MXF_MUX_FLAG_XDCAM_IMX50		0x00000040	//!< @brief IMX50

#define MXF_MUX_FLAG_AES3_SPLIT			0x00000080	//!< @brief Flag to split channels for input aes3 stream

#define MXF_MUX_FLAG_AES3_OUT			  0x00000100	//!< @brief Flag to specify AES audio essence instead of wave for PCM in generic profile

#define MXF_MUX_FLAG_AVC_ST381_3		0x00000200	//!< @brief Flag to specify descriptor style for AVC ind generic profiles.
                                                /*!< @details Use CDCIDescriptor/AVCSubdescriptor according to SMPTE 381-3
                                                      rather than to old style SMPTE RP 2008 w. MPEG Descriptor.*/

//!< @brief Video flags for XAVC Intra 4K
/*!< @details Used to decide index table style and for calculating Index EditUnitByteCount and max bitrate for AVC sub Descriptor.
For absolutely XAVC Specification conform output these flags should be set.
However not set these flags if the stream parameters are not clearly known.
A valid, though not fully XAVC Specification conform, file can be created w.o. that information.
*/
#define MXF_MUX_FLAG_AVC_INTRA_4K_CBG  0x00001000 //!< @brief Flag to indicate that the XAVC Intra 4K video stream has constant bitrate.
                                                    /*!< @details Used to decide index table style.
                                                      There is no bitrate information in XAVC Intra video streams.
                                                      If constant bitrate is indicated a simple index will be used.
                                                      Must be Used in combination with Intra class flags below */

#define MXF_MUX_FLAG_XAVC_INTRA_4K_100  0x00002000 //!< @brief Flag to indicate the XAVC Intra 4K video stream bitrate class 100 for VBR and CBG.
#define MXF_MUX_FLAG_XAVC_INTRA_4K_300  0x00004000 //!< @brief Flag to indicate the XAVC Intra 4K video stream bitrate class 300 for VBR and CBG.
#define MXF_MUX_FLAG_XAVC_INTRA_4K_480  0x00008000 //!< @brief Flag to indicate the XAVC Intra 4K video stream bitrate class 400 for VBR and CBG.



// !@}


//! @name Operational patterns
//! @brief Used with \ref  mxf_m_settings.operational_pattern field
//! @{
#define OPERATIONAL_PATTERN_OPATOM  1	//!< @brief Specialized Operational Pattern "Atom" (spec. SMPTE 390M)
                                        /*!< @details Use this pattern for the multiplexing of one essence track in one output stream
											                    Thus, video, audio tracks wrap into separate output streams
											                    Panasonic P2 and DCI profiles use this operational pattern */

#define OPERATIONAL_PATTERN_OP1A    2	//!< @brief Generalized Operational Pattern 1A (spec. SMPTE 378M)

#define OPERATIONAL_PATTERN_OP1B    3	//!< @brief Operational Pattern 1B (spec. SMPTE 391M)
										                  //!< @details Use for multiple ganged input streams
// !@}


//! @name Metadata types
//! @brief used with \ref  mxf_reserv_meta_info.metadata_type and mxf_dm_meta_data.dm_type field
//! @{
#define MXF_METADATA_XML_DARK         1  //!< @brief XML-based dark metadata specified in SMPTE 377M
#define MXF_METADATA_SONY_XML_DARK    2  //!< @brief Sony NRT metadata

#define MXF_METADATA_DMS1             3  //!< descriptive metadata scheme1 specified SMPTE 380M. do not use, for future abilities

#define MXF_METADATA_DMS_P2_XML       4  //!< P2 AVC-LongGOP decriptive metadata, XML meta data
#define MXF_METADATA_DMS_P2_BMP       5  //!< P2 AVC-LongGOP decriptive metadata, BMP Thumbnail
//! @}

//! @name Descritpive Metadata track types
//! @brief used with \ref  mxf_dm_meta_data.dm_track_type field
//! @{
#define MXF_DM_TRACK_TYPE_STATIC    0x00000001
#define MXF_DM_TRACK_TYPE_EVENT     0x00000002
#define MXF_DM_TRACK_TYPE_TIMELINE  0x00000003  // currently not supported
//! @}


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif


// Meta Data



//! @brief Used to specify Panasonic P2 AVC-LongGOP Descriptive Metadata in \ref mxf_dm_meta_data
//! @details Panasonic P2 DM segment (to be used with METADATA_DMS_P2_XML, METADATA_DMS_P2_BMP)
struct mxf_dm_segment_info_p2
{
  uint32_t  segment_id;                 //!< @brief User defined unique segment ID, must be identical for reserve and update calls
  //
  uint32_t  size_data;                  //!< @brief Size of segment data, space to reserve or size of data
  uint8_t   *data;                      //!< @brief Pointer to data buffer, Thumbnail or XML, or NULL in case of space reservation
  //
  uint32_t  duration;                   //!< @brief Duration in number of frames (default 1 for P2 descriptive meta data)
  //
  uint32_t  event_start_position;       //!< @brief Event start position (Offset of the thumbnail position in number of frames)
  //
  uint32_t  thumbnail_format;           //!< @brief Thumbnail Format (default 0 for P2)
  int32_t   thumbnail_width;            //!< @brief Thumbnail Width
  int32_t   thumbnail_height;           //!< @brief Thumbnail Height
  //
  int32_t reserved[10];
};


/*! @brief Used to specify Descriptive Metadata details as extra data in \ref mxf_reserv_meta_info and \ref mxf_update_meta_info */
struct mxf_dm_meta_data
{
  int32_t dm_type;                  //!< @brief Decsriptive Meta Data type, one of MXF_METADATA_DMS_xx
  int32_t dm_track_type;            //!< @brief Track Type, one of DM_TRACK_TYPE_xx  - automatically set for P2 DM
  uint8_t dm_scheme_ul[16];         //!< @brief DM Scheme Label - automatically set for P2 DM
  uint8_t dm_framework_key[16];     //!< @brief DM Framework key - automatically set for P2 DM
  //
  int32_t num_dm_segments;          //!< @brief Number of dm_segments
  int32_t size_segment_struct;      //!< @brief Size of a dm_segment_info struct, according to dm_type, e.g. sizeof mxf_dm_segment_info_p2 for P2 DM
  uint8_t *dm_segments;             //!< @brief DM segments buffer, array of mxf_dm_segment_info_xxx structures
  //
  int32_t reserved[10];
};


//! @brief Used to allocate space for metadata in the end of header partition
//! @details This structure is necessary for \ref mxfMuxReserveMetadata function
struct mxf_reserv_meta_info
{
  int32_t ID;                   //!< @brief user-defined unique identifier for metadata

  int32_t output_stream_id;     //!< Identifier for output stream in which it's necessary to allocate space.
								                //!< Do not use, currently not implemented.

  int32_t metadata_type;        //!< @brief One of MXF_METADATA_xxx values

  char *klv_key;                //!< @brief Pointer to a 16-byte KLV key for packet of dark metadata
                                //!< @details If metadata_type equal MXF_METADATA_DARK this value is mandatory. Automatically set for the other supported types.

  uint32_t size;                //!< @brief Packet size of dark metadata, or size of extra data in case of descriptive meta data (sizeof(mxf_mux_dm_meta_data_t))

  uint8_t* data;                /*!< @brief pointer to the dark meta data buffer (can be NULL), or pointer to extra data as mxf_mux_dm_meta_data_t struct in case of Descriptive Metadata.*/
};



//! @brief Used to set XML-based dark metadata specified (SMPTE 377M) in \ref mxf_update_meta_info
struct mxf_metadata_xml_dark
{
  char *xml_data;				  //!< @brief XML data
  int32_t xml_size;				//!< @brief size of XML data. Note: this mus be equal or less than the reserved size, otherwise the data will be truncated
};

//! @brief Used to update/write metadata into the allocated space
//! @details This structure is necessary for \ref mxfMuxUpdateMetadata function
struct mxf_update_meta_info
{
  int32_t ID;                   //!< @brief User-defined unique identifier for updated metadata
                                //!< @details Must correspond to value from mxf_reserv_metadata_info structure

  int32_t output_stream_id;     //!< Identifier for output stream in which it's necessary to update metadata
								                    //!< Do not use, currently not implemented

  void *data;                   /*!< @brief Pointer to one of mxf_metadata_xxx structures in case of Dark Metadata,
                                      or mxf_dm_meta_data_xxx in case of Descriptive Metadata */

  int32_t size;                 //!< @brief Size of structure
};







//! @brief Allows to set product info
/*!	@details This structure uses for passing to multiplexer info about product info.
	This info will be setup into Identification pack in mxf file.
	Used with \ref mxf_m_settings.product_info field.*/
struct mxf_product_info
{
  int32_t size;                 //!< @brief size of this structure
  char company_name[32];        //!< @brief string containing company name
  char product_name[32];        //!< @brief string containing product name
  char product_version[16];     //!< @brief string containing version number
  uint8_t productUID[16];       //!< @brief a unique identification for the product
};


//! @brief Used to define the \ref mxf_muxer_tt object
//! @details The multiplexer uses the mxf_m_settings structure to hold its settings
struct mxf_m_settings
{
	int32_t  profile;					        //!< @brief muxer profile - one of MXF_PROF_XX
	uint32_t video_sequence_length;		//!< depreciated, don't use

	int32_t	 reserved1[5];				    //!< Reserved for future use

	uint32_t video_framerate;			    //!< @brief input video frame rate (e.g. for J2K, VC3 video streams) - one of FRAMERATE_XX (mcdefs.h)

	mxf_product_info *product_info;		//!< @brief pointer to a mxf_product_info structure

	uint32_t operational_pattern;		  //!< @brief operational pattern for MXF wrapping - one of OPERATIONAL_PATTERN_XXX

	uint32_t dci_2k_48;					      //!< 0 or 1 - use 48fps for DCI 2K
										                //!< depreciated, use video_framerate instead.
										                //!< With SMPTE 428-11 now framerates 24, 25, 30, 50, 60 fps are possible for 2K and 24, 25, 30 for 4K

	int32_t	 mplex_type;				      //!< @brief Multiplex sub type - one of MXF_MUX_TYPE_XXX

	int32_t  reserved2[5];				    //!< Reserved for future use

	uint32_t max_gop;					        //!< @brief GOP length (MPEG-2, AVC LongGOP)

	uint32_t user_duration;				    //!< @brief Preset the duration in number of video frames
										                /*!< @details Mandatory for Sony XDCAM SxS and Sony XAVC formats.
										                  Also required for Phase 3 XDCAM FAM mode, which can not rewrite duration in headers at end of muxing.
										                  Note that if the final duration does not match the preset one, file may become invalid on Phase 3 XDCAM devices. */

	int32_t  flags;						        //!< @brief Or'ed combination of  MXF_MUX_FLAG_xxx

	uint32_t start_timecode;			    //!< @brief Start time code in frames
										                /*!< @details - Time code Component:Start Time Code,
										                  in case of spanned clips represents the start TC of first clip */

	uint32_t start_v_duration;			  //!< @brief Video duration in frames of all previous linked clips
										                /*!< @details Zero, if first of spanned clips.
										                    Used for P2 only actualy. */

	uint64_t start_a_duration;			  //!< @brief Audio duration in frames of all previous linked clips
										                /*!< @details Zero, if first of spanned clips.
										                    Used for P2 only actualy. */

	uint8_t  *material_id;				    //!< @brief Pointer to a 32 byte array containing UMID for P2 or 0.
										                /*!< @details If 0 UMID is generated internaly.
										                  In case of spanned clips UMID must be passed from external and be same for all clips */

	char     *p2_user_clip_name;		  //!< @brief String containing external P2 clip name or 0.
										                /*!< @details 8 characters for P2 AVC LongGOP, 6 characters for P2 AVC-Intra and P2 DVCPRO, following P2 specification.
										                  If 0, the name is generated internal from clip no and UMID in case of P2 AVC-Intra and P2 DVCPRO.
                                      For AVC LongGOP it is mandatory to pass a correct name from external. */

	int32_t  file_number;				      //!< @brief File number for P2 0..999 used as first four digits for internal clip name generation
};

#pragma pack(pop)

//! @brief Rules for P2 AVC-Intra or P2 DVCPRO spanned clips:
/*!< @details
To link spanned clips all clips must have the same Material ID. The Material ID has to be created externally and to be passed through the material_id parameter for every clip.
The video and audio start durations for every must be set to the total duration of all previous clips. In number of frames for start_v_duration and number of samples for start_a_duration.
The start timecode for each clip however shall be set to the initial start timecode of the whole sequence, e.g. 0 for every clip.
Additionally the external XML Clip metadata files in /CLIP must contain the Relation element with it's child elements. See P2 Content specification
*/

/////////////////////////////////////////////////////////////////////////////
// muxing routines in mcmxfmux.dll
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif


//! Call this function to fill a \ref mxf_m_settings structure with defaults values
//! based on one of the MCPROFILE_xxx preset values
//!
//! @param[in,out] set				pointer to a \ref mxf_m_settings structure
//! @param[in]     profileID		one of the MCPROFILE_* or MPEG_* constants
//!
//! @return pointer to a profile string or NULL

char* MC_EXPORT_API mxfMuxDefaults(struct mxf_m_settings *set, int32_t profileID);


//! Call to create a mxf muxer object
//!
//! @param[in]      get_rc     pointer to a get resource function
//! @param[in]      set        pointer to a filled in \ref mxf_m_settings structure
//! @return  pointer to a \ref mxf_muxer_tt object if succesful,\n
//!    NULL if unsuccesful

mxf_muxer_tt* MC_EXPORT_API mxfMuxNew(void *(MC_EXPORT_API *get_rc)(const char* name), const struct mxf_m_settings *set);


//! Call to add a video, audio or subpic stream to the muxer in streaming mode
//!
//! @param[in]      muxer		pointer to a mxf muxer object
//! @param[in]      reserved	reserved for future use, set to NULL
//! @param[in]      input		pointer to a \ref bufstream_tt object for the input data
//! @return 0 if successful

int32_t MC_EXPORT_API mxfMuxAddInputStream(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *input);


//! Call to add an output mxf stream to the muxer in streaming mode
//!
//! @param[in]      muxer		pointer to a mxf muxer object
//! @param[in]      reserved	reserved for future use, set to NULL
//! @param[in]      output		pointer to a \ref bufstream_tt object for the output muxed data
//! @return 0 if successful

int32_t MC_EXPORT_API mxfMuxAddOutputStream(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *output);


//! Call to add a video, audio or subpic stream to the muxer in file mode
//!
//! @param[in]      muxer		pointer to a mxf muxer object
//! @param[in]      reserved	reserved for future use, set to NULL
//! @param[in]      input		pointer to an input filename or \ref bufstream_tt object (BS version) for the input data
//! @return 0 if successful
//!@{

int32_t MC_EXPORT_API mxfMuxAddInputFile(mxf_muxer_tt *muxer, void *reserved, char *input);
int32_t MC_EXPORT_API mxfMuxAddInputFileBS(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *input);

//!@}


//! Call to add an output mxf stream to the muxer in file mode
//!
//! @param[in]      muxer		pointer to a mxf muxer object
//! @param[in]      reserved	reserved for future use, set to NULL
//! @param[in]      output		pointer to an output filename or \ref bufstream_tt object (BS version) for the output data
//! @return 0 if successful
//!@{

int32_t MC_EXPORT_API mxfMuxAddOutputFile(mxf_muxer_tt *muxer, void *reserved, char *output);
int32_t MC_EXPORT_API mxfMuxAddOutputFileBS(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *output);

//!@}


//! Call to do the multiplexing in file mode
//!
//! @param[in]  muxer   pointer to a mxf muxer object
//! @return 0 if successful

uint32_t MC_EXPORT_API mxfMux(mxf_muxer_tt *muxer);


//! Call to finish a muxing session, set abort non-zero if muxing is being aborted.
//!
//! @param[in]    muxer  pointer to a mxf muxer object
//! @param[in]    abort  set to 0 to finish any leftover muxing and clean up,
//!            else just clean up
//!
//! @return 0 if successful

int32_t MC_EXPORT_API mxfMuxDone(mxf_muxer_tt *muxer, int32_t abort);


//! Call to free a mxf muxer object
//!
//! @param[in]    muxer  muxer object to free
//!
//! @return    none

void MC_EXPORT_API mxfMuxFree(mxf_muxer_tt *muxer);


//! Call to get the setting errors/warnings in an mxf_m_settings structure
//! use with the get_rc callback to provide an err_printf callback to get
//! error messages that can be localized
//!
//! @param[in]	get_rc	pointer to a get_rc function
//! @param[in]	set		pointer to a mxf_m_settings structure
//! @param[in]	options	check options, one or more of the CHECK_* defines in mcdefs.h
//! @param[in]	app		reserved
//! @return 0 if successful

int32_t MC_EXPORT_API mxfMuxChkSettings(void *(MC_EXPORT_API *get_rc)(const char* name), const struct mxf_m_settings *set, uint32_t options, void *app);


//! Call to allocate space for metadata in the end of header partition
//!
//! @param[in]	muxer	pointer to an mxf muxer object
//! @param[in]	info	pointer to a mxf_reserv_meta_info structure
//! @return 0 if successful
//!
//! use this call immediately after calling mxfMuxNew and before any data processing starts
int32_t MC_EXPORT_API mxfMuxReserveMetadata(mxf_muxer_tt *muxer, struct mxf_reserv_meta_info *info);


//! Call to update/write metadata into the allocated space
//!
//! @param[in]	muxer	pointer to an mxf muxer object
//! @param[in]	info	pointer to a mxf_update_meta_info structure
//!
//! @return 0 if successful
//!
//! use this call at any time after a mxfMuxReserveMetadata call for the same item, but before calling mxfMuxDone
int32_t MC_EXPORT_API mxfMuxUpdateMetadata(mxf_muxer_tt *muxer, struct mxf_update_meta_info *info);


//! Call to get extend API functions
//!
//! @param[in]	func	ID	for function a pointer to get
//!
//! @return function pointer or NULL

APIEXTFUNC MC_EXPORT_API mxfMuxGetAPIExt(uint32_t func);


//! Call to get the setting errors/warnings of supported media-types in frame of selected mux settings
//! use with the get_rc callback to provide an err_printf callback to get
//! error messages that can be localized
//!
//! @param[in]	get_rc	pointer to a get_rc function
//! @param[in]	set		pointer to a mxf_m_settings structure
//! @param[in]	format	pointer to a mc_stream_format structure
//! @param[in]	app		reserved
//! @return 0 if successful
int32_t MC_EXPORT_API mxfMuxChkMediatypeSupport(void *(MC_EXPORT_API *get_rc)(const char* name), const struct mxf_m_settings *set, const struct mc_stream_format *format, void *app);


#ifdef __cplusplus
}
#endif

//! @name Depreciated Items
//! @brief Outdated - do not use - for compatibility only
//! @{
#define MXF_PROF_JPEG2000_DEFAULT   5	//!< JPEG2000 profile, use default profile instead

#define MXF_MUX_TYPE_J2K            200	//!< JPEG2000 multiplex type, use default multiplex type instead

#define MXF_PROF_SONY_MPEG_HD		0	//!< XDCAM profile
#define MXF_PROF_PANASONIC_DVCPRO	3	//!< P2 DVCPRO profile

#define MXF_MUX_TYPE_DVPRO			100 //!< DV PRO mplex type

#ifndef MPEG_XDCAM_IMX
#define MPEG_XDCAM_IMX				28	//!< XDCAM IMX mplex type, already also defined in mcdefs.h
#endif
#ifndef MPEG_XDCAM_HD
#define MPEG_XDCAM_HD				32	//!< XDCAM HD mplex type, already also defined in mcdefs.h
#endif
//! @}


#endif // #if !defined (__MUX_MXF_API_INCLUDED__)

