/**
@file http.h
@brief Declaration HTTP header parser interface
@verbatim
File: http.h

Desc: Declaration HTTP header parser interface

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
 */

#if !defined (__MC_HTTP_H__)
#define __MC_HTTP_H__

#include "net_common.h"

/**
* namespace MainConcept
* @brief Global SDK namespace
**/
namespace MainConcept {

	/**
    * namespace NetworkStreaming
    * @brief Network streaming specific namespace
    **/
	namespace NetworkStreaming {

		/**
		 * @brief HTTP methods
		**/
		typedef enum _tagHttpMethod {

			HttpNotify = 0,					/**< UPnP NOTIFY request */
			HttpMSearch = 1,				/**< UPnP M-SEARCH request */
			HttpGet = 2,					/**< HTTP GET request */
			HttpPost = 3,					/**< HTTP POST request */
			HttpReply = 4,					/**< HTTP response */
			HttpOption = 5,					/**< HTTP OPTIONS request */
			HttpHead = 6,					/**< HTTP HEAD request */
			HttpDelete = 7,					/**< HTTP DELETE request */
			HttpTrace = 8,					/**< HTTP TRACE request */
			HttpSubscribe = 9,				/**< UPnP SUBSCRIBE request */
			HttpUnSubscribe = 10,			/**< UPnP UNSUBSCRIBE request */
			HttpExt = 11,					/**< Reserved */
			HttpUndefinedMethod = 0xff		/**< Unknown method */
		} HttpMethod;

		/**
		* @brief HTTP response codes
		**/
		typedef enum _tagHttpErrCodes {

			EHttpContinue = 100,				/**< Continue */
			EHttpSwitchProto = 101,				/**< Switching Protocols */
			EHttpOk = 200,						/**< OK */
			EHttpCreated  = 201,				/**< Created */
			EHttpAccepted = 202,				/**< Accepted */
			EHttpNoContent = 204,				/**< No Content */
			EHttpPartialContent = 206,			/**< Partial Content */
			EHttpMultipleChoices = 300,			/**< Multiple Choices */
			EHttpMovedPermanently = 301,		/**< Moved Permanently */
			EHttpFound = 302,					/**< Found */
			EHttpSeeOther = 303,				/**< See Other */
			EHttpNotModified = 304,				/**< Not Modified */
			EHttpTemporalRedirect = 307,		/**< Temporary Redirect */
			EHttpBadReq = 400,					/**< Bad Request */
			EHttpNonAuthorized = 401,			/**< Unauthorized */
			EHttpForbidden = 403,				/**< Forbidden */
			EHttpNotFound = 404,				/**< Not Found */
			EHttpMethodNotAllowed = 405,		/**< Method Not Allowed */
			EHttpNotAcceptable = 406,			/**< Not Acceptable */
			EHttpProxyNonAuthorized = 407,		/**< Proxy Authentication Required */
			EHttpLenghtRequired = 411,			/**< Length Required */
			EHttpPreconditionFailed = 412,		/**< Precondition Failed */
			EHttpUnsupportedMediaType = 415,	/**< Unsupported Media Type */
			EHttpInternalError = 500,			/**< Internal Server Error */
			EHttpNotImplemented = 501,			/**< Not Implemented */
			EHttpVerNotSupported = 505,			/**< HTTP Version Not Supported */

			EHttpNoSuchObject = 701,			/**< UPnP: No such object */
			EHttpInvalidSortCriteria = 709,		/**< UPnP: Unsupported or invalid sort criteria */
			EHttpCannotProcessRequest = 720,	/**< UPnP: Cannot process the request */

			EHttpUnknownCode = 0x7fffffff		/**< Unknown response code */
		} HttpErrCodes;

		/**
		 * @brief HTTP protocol version
		**/
		typedef enum _tagHttpVersion {

			Http1 = 0,							/**< HTTP/1.0 */
			Http1_1 = 1,						/**< HTTP/1.1 */
			Icy = 2,							/**< ICY */
			HttpUnsupportedVersion = 0xff		/**< Unknown protocol version */
		} HttpVersion;

		/**
		 * @brief HTTP content transfer encoding
		**/
		typedef enum _tagHttpTransferEncoding {
			HttpTransferEncodingNone = 0,		/**< Not specified */
			HttpTransferEncodingChunked = 1,	/**< chunked */
			HttpTransferEncodingIdentity = 2,	/**< identity */
			HttpTransferEncodingGzip = 3,		/**< gzip */
			HttpTransferEncodingCompress = 4,	/**< compress */
			HttpTransferEncodingDeflate = 5		/**< deflate */
		} HttpTransferEncoding;

		/**
		 * @brief HTTP connection type
		**/
		typedef enum _tagHttpConnectionType {
			HttpConnectionClose = 0,			/**< Close */
			HttpConnectionKeepAlive = 1			/**< Keep-Alive */
		} HttpConnectionType;

		/**
        * @brief HTTP request and response header parser
        **/
		class IHttp : public INetRefCounted
		{
		public:

			/**
            * @brief Parses request/response buffer and fills internal fields
            *
            * @param[in]	pData		Request buffer
			* @param[in]	iDataLen	Length of request buffer
			* @param[in]	bIsPartOfMultiPartStream	Has to be set to true when pData contains data for multipart stream without HTTP header
			*
            * @return		0 on success, -1 on parse error
            **/
			virtual int32_t Parse( const int8_t *pData, int32_t iDataLen, bool bIsPartOfMultiPartStream = false ) = 0;

			/**
            * @brief Get HTTP request type
            *
            * @return		Request type or @ref HttpReply if HTTP response detected
            **/
			virtual HttpMethod GetMethod() const = 0;

			/**
            * @brief Get HTTP protocol version
            *
            * @return		HTTP protocol version or @ref Icy if SHOUTcast streaming detected
            **/
			virtual HttpVersion GetVersion() const = 0;

			/**
            * @brief Get HTTP headers length
            *
            * @return		Headers length including trailing line endings
            **/
			virtual int32_t GetHeadersLength() const = 0;

			/**
            * @brief Get full HTTP request URI. Request URI is returned as is, including all slashes and query string.
            *
            * @return		Request URI or NULL if HTTP response found
            **/
			virtual const int8_t * GetRequestedURI() const = 0;

			/**
            * @brief Get requested path from decoded URI without query string and leading slashes. Any occurrences of . and .. are also removed.
            *
            * @return		Decoded URI or NULL if HTTP response found
            **/
			virtual const int8_t * GetRequestedPath() const = 0;

			/**
            * @brief Get query string, fetched from request URI
            *
            * @return		Query string or NULL if not present in request URI
            **/
			virtual const int8_t * GetRequestedQueryString() const = 0;

			/**
            * @brief Get HTTP response code
            *
            * @return		HTTP response code if response detected, EHttpOk if HTTP request found
            **/
			virtual HttpErrCodes GetErrCode() const = 0;

			/**
            * @brief Get HTTP response string
            *
            * @return		HTTP response string if response detected, NULL if HTTP request found
            **/
			virtual const int8_t * GetErrCodeString() const = 0;

			/**
            * @brief Get User Agent string
            *
            * @return		User Agent string or NULL if not present
            **/
			virtual const int8_t * GetUserAgent() const = 0;

			/**
            * @brief Get content type
            *
            * @return		Content type string or NULL if not present
            **/
			virtual const int8_t * GetContentType() const = 0;

			/**
            * @brief Get content length. For response, if Content-Length header not found, length can also be fetched from Range header.
            *
            * @return		Content type string or NULL if not present
            **/
			virtual int64_t GetContentLength() const = 0;

			/**
            * @brief Get start of Range
            *
            * @return		Range start or -1 if Range header is not found or Range start is not specified
            **/
			virtual int64_t GetRangeStart() const = 0;

			/**
            * @brief Get end of Range
            *
            * @return		Range end or -1 if Range header is not found or Range end is not specified
            **/
			virtual int64_t GetRangeEnd() const = 0;

			/**
            * @brief Get start of TimeSeekRange.dlna.org or availableSeekRange.dlna.org
            *
            * @return		TimeSeekRange start or -1.0 if TimeSeekRange header is not found or TimeSeekRange start is not specified
            **/
			virtual double GetTimeSeekRangeStart() const = 0;

			/**
            * @brief Get end of TimeSeekRange.dlna.org or availableSeekRange.dlna.org
            *
            * @return		TimeSeekRange end or -1.0 if TimeSeekRange header is not found or TimeSeekRange end is not specified
            **/
			virtual double GetTimeSeekRangeEnd() const = 0;

			/**
            * @brief Get length of TimeSeekRange.dlna.org or availableSeekRange.dlna.org
            *
            * @return		TimeSeekRange length or -1.0 if TimeSeekRange header is not found
            **/
			virtual double GetDuration() const = 0;

			/**
            * @brief Get content transfer encoding
            *
            * @return		Content transfer encoding type or @ref HttpTransferEncodingNone if not specified
            **/
			virtual HttpTransferEncoding GetTransferEncoding() const = 0;

			/**
            * @brief Get UPnP Callback URI
            *
            * @return		Callback URI or NULL if not present
            **/
			virtual const int8_t * GetCallback() const = 0;

			/**
            * @brief Get UPnP SID string
            *
            * @return		SID string or NULL if not present
            **/
			virtual const int8_t * GetSID() const = 0;

			/**
            * @brief Get UPnP NT string
            *
            * @return		NT string or NULL if not present
            **/
			virtual const int8_t * GetNT() const = 0;

			/**
            * @brief Get timeout value in seconds
            *
            * @return		Timeout value in seconds or 0 if not present
            **/
			virtual int32_t GetTimeout() const = 0;

			/**
            * @brief Get UPnP SOAP action string
            *
            * @return		SOAP action string or NULL if not present
            **/
			virtual const int8_t * GetSoapAction() const = 0;

			/**
            * @brief Get HTTP connection type
            *
            * @return		HTTP Connection type
            **/
			virtual HttpConnectionType GetConnectionType() const = 0;

			/**
            * @brief Get (redirect) Location URI
            *
            * @return		Location URI or NULL if not present
            **/
			virtual const int8_t *GetLocation() const = 0;

			/**
            * @brief Get boundary value for multipart content type
            *
            * @return		null terminated string with boundary value when present, otherwise NULL pointer
            **/
			virtual const int8_t * GetMultiPartBoundary() const = 0;

			/**
            * @brief Reset all internal fields and prepare to parse another buffer
            *
            * @return		none
            **/
			virtual void Reset() = 0;
		};

#if defined (__cplusplus)
		extern "C" {
#endif

			/**
            * @brief Returns response string for response code
            *
            * @param[in]	aCode		HTTP response code
			*
            * @return		Response string or NULL if no predefined string found for specified response code
            **/
			const char *NET_API_CALL HttpGetCodeString(HttpErrCodes aCode);

			/**
            * @brief Removes any leading slashes, extra slashes and . from path
            *
            * @param[in]	pEncodedString		Path to filter
			* @param[out]	pOutDecodedString	Resulting path, should be at least the size of pEncodedString including trailing zero byte
			*
            * @return		None
            **/
			void NET_API_CALL HttpMakeRelativePath( const int8_t *pEncodedString, int8_t *pOutDecodedString );

			/**
            * @brief Removes forbidden symbols (like ? and *) from path
            *
            * @param[in]	pEncodedString		Path to filter
			* @param[out]	pOutDecodedString	Resulting path, should be at least the size of pEncodedString including trailing zero byte
			*
            * @return		None
            **/
			void NET_API_CALL HttpRemoveNonPathSymbols( const int8_t *pEncodedString, int8_t *pOutDecodedString );

			/**
            * @brief Decode URL %XX sequences and + characters
            *
            * @param[in]	pEncodedString		Path to filter
			* @param[out]	pOutDecodedString	Resulting path, should be at least the size of pEncodedString including trailing zero byte
			*
            * @return		None
            **/
			void NET_API_CALL HttpUrlDecode( const int8_t *pEncodedString, int8_t *pOutDecodedString );

			/**
            * @brief Searches buffer for HTTP headers length
            *
            * @param[in]	pData		Request buffer
			* @param[in]	iDataLen	Length of request buffer
			* @param[in]	bIsMultiPart	In case of multipart (content-type: multipart/foo-bar;boundary=--foo) headers length have to be calculated in a different way
			*
            * @return		Headers length including trailing line endings or 0 if HTTP header end sequence not found
            **/
			int32_t NET_API_CALL HttpCalculateHeadersLen( const int8_t *pData, int32_t iDataLen, bool bIsMultiPart = false );

			/**
            * @brief Searches buffer for HTTP request type
            *
            * @param[in]	pData		Request buffer
			* @param[in]	iDataLen	Length of request buffer
			*
            * @return		HTTP request type or @ref HttpReply if response detected or HttpUndefinedMethod on error
            **/
			HttpMethod NET_API_CALL HttpFetchMethod( const int8_t *pData, int32_t iDataLen );

			/**
            * @brief Searches buffer for HTTP request URI
            *
            * @param[in]	pData		Request buffer
			* @param[in]	iDataLen	Length of request buffer
			* @param[out]	pUriLen		Pointer to int that will receive URI length in bytes, can be NULL
			*
            * @return		Pointer of request URI in pData buffer on success and NULL on error
            **/
			const int8_t * NET_API_CALL HttpFetchURI( const int8_t *pData, int32_t iDataLen, int32_t *pUriLen );

			/**
            * @brief Checks that buffer contains valid HTTP headers
            *
            * @param[in]	pData		Request buffer
			* @param[in]	iDataLen	Length of request buffer
			*
            * @return		true if valid headers found, false if buffer is not a valid HTTP request or response
            **/
			bool NET_API_CALL HttpCheckHeaders( const int8_t *pData, int32_t iDataLen );

			/**
            * @brief Retrieves new HTTP headers parser instance
            *
            * @param[in] get_rc            Pointer to get_rc memory manager implementation or NULL to use standard allocators
			*
            * @return                      valid pointer to HTTP headers parser or NULL on error
            **/
			IHttp * NET_API_CALL GetHttpInstance( void *(MC_EXPORT_API *get_rc)(const char*) );

#if defined (__cplusplus)
		}
#endif

	};
};

#endif //__MC_HTTP_H__
