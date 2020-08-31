/*************************************************************

				FastCRC Library 1.51

Module Name:    FastCRC.h

Abstract:       Master include file for FastCRC Library
			
	
			Copyright (c) 1999-2007, SlavaSoft Inc.All rights reserved.

*************************************************************/

#ifndef __FASTCRC_H__
#define __FASTCRC_H__

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined (_MSC_VER)
#if !defined( _SL_FASTCRC_IMPLEMENTATION ) && !defined( _SL_NOFORCE_LIBS )

#ifdef _SL_STATIC
	#pragma comment( lib, "FastCRCS.lib" )
#else
	#pragma comment( lib, "FastCRC.lib" )
#endif //_SL_STATIC

#endif

#endif


#if ( defined (_MSC_VER) && (_MSC_VER >= 800) ) || defined(_STDCALL_SUPPORTED)
#define SL_FCRCCALL _stdcall
#else
#define SL_FCRCCALL
#endif

#define SLC_FCRC_MAXHEXSIZE		9

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void				SL_FCRCCALL SL_FCRC_ConvertToHex32( char* pDest, unsigned long nCRC, int bUpper );
void				SL_FCRCCALL SL_FCRC_ConvertToHex16( char* pDest, unsigned long nCRC, int bUpper );

void				SL_FCRCCALL SL_FCRC32_Init( unsigned long* pCRC );
void				SL_FCRCCALL SL_FCRC32_Update( unsigned long* pCRC, const void* pSrc, unsigned int nSrcLength );
void				SL_FCRCCALL SL_FCRC32_UpdateStr( unsigned long* pCRC, const char* pSrc );
unsigned long		SL_FCRCCALL SL_FCRC32_Final( unsigned long* pCRC );
unsigned long		SL_FCRCCALL SL_FCRC32_Calculate( const void* pSrc, unsigned int nSrcLength );
unsigned long		SL_FCRCCALL SL_FCRC32_CalculateStr( const char* pSrc );
int					SL_FCRCCALL SL_FCRC32_CalculateFile( unsigned long* pCRC, const char* pFileName );

void				SL_FCRCCALL SL_FCRC16_Init( unsigned long* pCRC );
void				SL_FCRCCALL SL_FCRC16_Update( unsigned long* pCRC, const void* pSrc, unsigned int nSrcLength );
void				SL_FCRCCALL SL_FCRC16_UpdateStr( unsigned long* pCRC, const char* pSrc );
unsigned long		SL_FCRCCALL SL_FCRC16_Final( unsigned long* pCRC );
unsigned long		SL_FCRCCALL SL_FCRC16_Calculate( const void* pSrc, unsigned int nSrcLength );
unsigned long		SL_FCRCCALL SL_FCRC16_CalculateStr( const char* pSrc );
int					SL_FCRCCALL SL_FCRC16_CalculateFile( unsigned long* pCRC, const char* pFileName );

void				SL_FCRCCALL SL_FCRC16C_Init( unsigned long* pCRC );
void				SL_FCRCCALL SL_FCRC16C_Update( unsigned long* pCRC, const void* pSrc, unsigned int nSrcLength );
void				SL_FCRCCALL SL_FCRC16C_UpdateStr( unsigned long* pCRC, const char* pSrc );
unsigned long		SL_FCRCCALL SL_FCRC16C_Final( unsigned long* pCRC );
unsigned long		SL_FCRCCALL SL_FCRC16C_Calculate( const void* pSrc, unsigned int nSrcLength );
unsigned long		SL_FCRCCALL SL_FCRC16C_CalculateStr( const char* pSrc );
int					SL_FCRCCALL SL_FCRC16C_CalculateFile( unsigned long* pCRC, const char* pFileName );

#ifdef __cplusplus
}

#ifdef NO_NAMESPACE

#ifndef USING_NAMESPACE
#define USING_NAMESPACE( X )
#endif

#ifndef NAMESPACE_BEGIN
#define NAMESPACE_BEGIN( X )
#endif

#ifndef NAMESPACE_END
#define NAMESPACE_END
#endif

#else

#ifndef USING_NAMESPACE
#define USING_NAMESPACE( X ) using namespace X;
#endif

#ifndef NAMESPACE_BEGIN
#define NAMESPACE_BEGIN( X ) namespace X {
#endif

#ifndef NAMESPACE_END
#define NAMESPACE_END }
#endif

#endif


NAMESPACE_BEGIN( FastCRC )



class CFastCRC32
{
public:
	CFastCRC32(){ SL_FCRC32_Init( &m_nCRC ); }
	~CFastCRC32(){}

	void					Update( const unsigned char* pSrc, unsigned int nSrcLength ){ SL_FCRC32_Update( &m_nCRC, pSrc, nSrcLength ); }
	void					Update( const char* pSrc ){ SL_FCRC32_UpdateStr( &m_nCRC, pSrc ); }
	unsigned long			Final(){ return SL_FCRC32_Final( &m_nCRC ); }

	static unsigned long	Calculate( const unsigned char* pSrc, unsigned int nSrcLength ) { return SL_FCRC32_Calculate( pSrc, nSrcLength ); }
	static unsigned long	Calculate( const char* pSrc ) { return SL_FCRC32_CalculateStr( pSrc ); }
	static int				Calculate( unsigned long* pCRC, const char* pFileName ){ return SL_FCRC32_CalculateFile( pCRC, pFileName ); }
private:
	unsigned long m_nCRC;
};

class CFastCRC16
{
public:
	CFastCRC16(){ SL_FCRC16_Init( &m_nCRC ); }
	~CFastCRC16(){}

	void					Update( const unsigned char* pSrc, unsigned int nSrcLength ){ SL_FCRC16_Update( &m_nCRC, pSrc, nSrcLength ); }
	void					Update( const char* pSrc ){ SL_FCRC16_UpdateStr( &m_nCRC, pSrc ); }
	unsigned long			Final(){ return SL_FCRC16_Final( &m_nCRC ); }

	static unsigned long	Calculate( const unsigned char* pSrc, unsigned int nSrcLength ) { return SL_FCRC16_Calculate( pSrc, nSrcLength ); }
	static unsigned long	Calculate( const char* pSrc ) { return SL_FCRC16_CalculateStr( pSrc ); }
	static int				Calculate( unsigned long* pCRC, const char* pFileName ){ return SL_FCRC16_CalculateFile( pCRC, pFileName ); }
private:
	unsigned long m_nCRC;
};

class CFastCRC16C
{
public:
	CFastCRC16C(){ SL_FCRC16C_Init( &m_nCRC ); }
	~CFastCRC16C(){}

	void					Update( const unsigned char* pSrc, unsigned int nSrcLength ){ SL_FCRC16C_Update( &m_nCRC, pSrc, nSrcLength ); }
	void					Update( const char* pSrc ){ SL_FCRC16C_UpdateStr( &m_nCRC, pSrc ); }
	unsigned long			Final(){ return SL_FCRC16C_Final( &m_nCRC ); }

	static unsigned long	Calculate( const unsigned char* pSrc, unsigned int nSrcLength ) { return SL_FCRC16C_Calculate( pSrc, nSrcLength ); }
	static unsigned long	Calculate( const char* pSrc ) { return SL_FCRC16C_CalculateStr( pSrc ); }
	static int				Calculate( unsigned long* pCRC, const char* pFileName ){ return SL_FCRC16C_CalculateFile( pCRC, pFileName ); }
private:
	unsigned long m_nCRC;
};

NAMESPACE_END

#endif /* __cplusplus */


#endif//#ifndef __FASTCRC_H__
