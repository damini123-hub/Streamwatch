/* ----------------------------------------------------------------------------
* File: sample_net_config.cpp   
*
* Desc: Read configurating settings from the file (implementation)
*
* Copyright (c) 2007-2010, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#include "sample_net_config.h"

#define CFG_DELIMITER '='
#define CFG_COMMENT '#'

CBaseFileConfig::CBaseFileConfig()
{

}

CBaseFileConfig::~CBaseFileConfig()
{

}

int32_t CBaseFileConfig::GetFileSize( FILE* pFile )
{
    int32_t iSize = 0;

    fseek( pFile, 0L, SEEK_END );
    iSize = (int32_t)ftell( pFile );
    fseek( pFile, 0L, SEEK_SET );

    return iSize;
}

int32_t CBaseFileConfig::SkipComments( const int8_t *pBuff, int32_t aLen )
{
    int32_t index = 0;
    while ( index < aLen && pBuff[index] != '\n' && pBuff[index] != '\r' ) {
        index++;
    };

    return index;
}

int32_t CBaseFileConfig::SkipEmptyLines( const int8_t *pBuff, int32_t aLen )
{
    int32_t index = 0;
    while ( (pBuff[index] == '\r' || pBuff[index] == '\n') && index < aLen ) {
        index++;
    };

    return index;
}

double CBaseFileConfig::ReadNumber( const int8_t *pBuff, int32_t aLen, int32_t *aSkip )
{
    int32_t index = 0;
    while ( pBuff[index] != CFG_DELIMITER && index < aLen ) {
        index++;
    };

    while ( pBuff[index] != '-' && !isdigit(pBuff[index]) && index < aLen ) {
        index++;
    }; 

    int32_t iParamLen = 0;
    while ( pBuff[index] != '\r' && pBuff[index] != '\n' && index < aLen ) {

        iParamLen++;
        index++;
    };

    int8_t csBuff[128] = {0};
    (*aSkip) = aLen;

    if ( index <= aLen && iParamLen < 128 ) {
        memcpy( csBuff, &pBuff[index - iParamLen], iParamLen );
    } else {
        return -1;
    }

    (*aSkip) = index;

    return atof((char*)csBuff);
}

int32_t CBaseFileConfig::ReadString( const int8_t *pBuff, int32_t aLen, int32_t *aSkip, int32_t *iParamPos, bool bIsSecureString /*= false*/ )
{
    int32_t index = 0;
    while ( (pBuff[index] == ' ' || pBuff[index] == '\t' || pBuff[index] == CFG_DELIMITER) && index < aLen ) {
        index++;
    };

    int32_t iParamLen = 0;
    (*iParamPos) = 0;
    while ( pBuff[index] != '\r' && pBuff[index] != '\n' && index < aLen ) {

        if ( bIsSecureString ) {

            while ( pBuff[index] == ' ' || pBuff[index] == '\t' ) {
                index++;
            };
        }

        if ( (*iParamPos) == 0 ) {
            (*iParamPos) = index;
        }

        iParamLen++;
        index++;
    };

    if ( index > aLen ) {
        return -1;
    }

    (*aSkip) = index;
    return iParamLen;
}

int32_t CBaseFileConfig::GetStringValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, int8_t*& pParamValue, bool bIsSecureString )
{
    int32_t len = 0;
    int32_t skipLen = 0;

    if ( _strnicmp( (char*)pBuff, pParamName, strlen(pParamName) ) == 0 ) {

        len += (int32_t)strlen(pParamName);
        int32_t iParamPos = 0;
        int32_t offset = ReadString( &pBuff[len], (aLen - len), &skipLen, &iParamPos, bIsSecureString );

        if ( offset < aLen && offset > 0 && iParamPos < aLen ) {

            int32_t iSafe = offset;
            if ( pBuff[iParamPos + len + iSafe] == 0 ){
                iSafe--;
            }
            while ( (isspace(pBuff[iParamPos + len + iSafe]) && iSafe > 0)) {
                --iSafe; 
            };
            offset -= (offset - iSafe) - 1;

            SAFE_DELETE_ARRAY(pParamValue);
            pParamValue = new int8_t[offset + 1];
            memset(pParamValue, 0, (offset + 1) * sizeof(int8_t));
            memcpy( pParamValue, (char*)&pBuff[iParamPos + len], offset );
            len += skipLen;
        }
    }

    return len;
}

int32_t CBaseFileConfig::GetWideStringValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, wchar_t*& pParamValue, bool bIsSecureString )
{
    int32_t len = 0;
    int32_t skipLen = 0;

    if ( _strnicmp( (char*)pBuff, pParamName, strlen(pParamName) ) == 0 ) {

        len += (int32_t)strlen(pParamName);
        int32_t iParamPos = 0;
        int32_t offset = ReadString( &pBuff[len], (aLen - len), &skipLen, &iParamPos, bIsSecureString );

        if ( offset < aLen && offset > 0 && iParamPos < aLen ) {

            int32_t iSafe = offset;
            if ( pBuff[iParamPos + len + iSafe] == 0 ){
                iSafe--;
            }
            while ( (isspace(pBuff[iParamPos + len + iSafe]) && iSafe > 0)) {
                --iSafe; 
            };
            offset -= (offset - iSafe) - 1;

            SAFE_DELETE_ARRAY(pParamValue);
            pParamValue = new wchar_t [offset + 1];
            memset(pParamValue, 0, (offset + 1) * sizeof(wchar_t));
            mbstowcs( pParamValue, (char*)&pBuff[iParamPos + len], offset );
            len += skipLen;
        }
    }

    return len;
}

int32_t CBaseFileConfig::GetNumberValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, int32_t& iParamValue )
{
    int32_t len = 0;
    int32_t skipLen = 0;

    if ( _strnicmp( (char*)pBuff, pParamName, strlen(pParamName) ) == 0 ) {

        len += (int32_t)strlen(pParamName);
        iParamValue = (int32_t)ReadNumber( &pBuff[len], (aLen - len), &skipLen );
        len += skipLen;
    }
    return len;
}

int32_t CBaseFileConfig::GetRealValue( const int8_t* pBuff, int32_t aLen, const char* pParamName, double& fParamValue )
{
    int32_t len = 0;
    int32_t skipLen = 0;

    if ( _strnicmp( (char*)pBuff, pParamName, strlen(pParamName) ) == 0 ) {

        len += (int32_t)strlen(pParamName);
        fParamValue = ReadNumber( &pBuff[len], (aLen - len), &skipLen );
        len += skipLen;
    }
    return len;
}

int32_t CBaseFileConfig::GetParam( const int8_t *pBuff, int32_t aLen )
{
    int32_t len = 0;

//     len += GetStringValue( pBuff, aLen, <NAME_PARAM_STR>, <VAR_PARAM_STR> );
//     len += GetNumberValue( pBuff, aLen, <NAME_PARAM_STR>, <VAR_PARAM_NUM> );

    return aLen; //Shift over buffer
}

int32_t CBaseFileConfig::ParseData( const int8_t *pBuff, int32_t aLen )
{
    int32_t index = 0;
    int32_t offset = 0;
    while ( index < aLen ) {

        index += SkipEmptyLines( &pBuff[index], (aLen - index) );
        if ( pBuff[index] == CFG_COMMENT ) {

            index += SkipComments( &pBuff[index], (aLen - index) );
            continue;
        } else if ( isalpha(pBuff[index]) ) {

            offset = GetParam( &pBuff[index], (aLen - index) );
            if ( offset > 0 ) {
                index += offset;
                continue;
            }
        }

        index++;
    };

    return 0;
}

void CBaseFileConfig::Read( const int8_t* pConfigFile )
{
    FILE* pFile;
    pFile = fopen( (char*)pConfigFile, "rb" );
    if ( pFile ) {

        int32_t iFileSize = GetFileSize( pFile );

        if ( iFileSize > 0 ) {

            int8_t *pBuff = new int8_t[iFileSize + 1];
            memset(pBuff, 0, iFileSize + 1);

            fread( pBuff, 1, iFileSize, pFile );
            ParseData( pBuff, iFileSize );

            SAFE_DELETE_ARRAY(pBuff);
        }

        fclose(pFile);
        pFile = NULL;
    }
}
