#if !defined(__MC_SAMPLE_NET_CONFIG_H__)
#define __MC_SAMPLE_NET_CONFIG_H__

/* ----------------------------------------------------------------------------
* File: sample_net_config.h   
*
* Desc: Read configurating settings from the file (declaration)
*
* Copyright (c) 2007-2010, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#include "sample_net_common.h"

class CBaseFileConfig 
{
public:
    CBaseFileConfig();  
    virtual ~CBaseFileConfig();
    void Read(const int8_t* pConfigFile);

protected:
    virtual int32_t GetParam( const int8_t *pBuff, int32_t aLen );
    
    int32_t GetStringValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, int8_t*& pParamValue, bool bIsAddress = false );
    int32_t GetWideStringValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, wchar_t*& pParamValue, bool bIsAddress = false );
    int32_t GetNumberValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, int32_t& iParamValue );
    int32_t GetRealValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, double& iParamValue );

    int32_t SkipComments( const int8_t *pBuff, int32_t aLen );
    int32_t SkipEmptyLines( const int8_t *pBuff, int32_t aLen );
    double ReadNumber( const int8_t *pBuff, int32_t aLen, int32_t *aSkip );
    int32_t ReadString( const int8_t *pBuff, int32_t aLen, int32_t *aSkip, int32_t *iParamPos, bool bIsAddress = false );
    int32_t ParseData( const int8_t *pBuff, int32_t aLen );
    int32_t GetFileSize( FILE* pFile );

protected:

};

#endif