/********************************************************************
** cryptkey.cpp : Encryption keys (and 'type' codes)
**
** Created: 3/1/2003, 2:59
** Author:  TJ Houston <tjh@myrathi.co.uk>
**
**
** Notes
**********
**    TJ: up to 3.0.6j added
**
*********************************************************************/
#include "clib/stl_inc.h"

#include "cryptkey.h"

CCryptInfo::CCryptInfo()
{
	InitList();
}

CCryptInfo::~CCryptInfo()
{
}

void CCryptInfo::InitList()
{
	CryptList.clear();
/*
	// (TJ) TWOFISH support disabled coz I haven't a bloody clue how to do it yet! >:/

	AddClient("3.0.6j",   0x2cc3ed9d, 0xa374227f, CRYPT_TWOFISH );
	AddClient("3.0.5d",   0x2c8b97ad, 0xa350de7f, CRYPT_TWOFISH );
	AddClient("3.0.2g",   0x2de3addd, 0xa3e5227f, CRYPT_TWOFISH );
	AddClient("3.0.2d",   0x2de3addd, 0xa3e5227f, CRYPT_TWOFISH );
	AddClient("3.0.2b",   0x2de3addd, 0xa3e5227f, CRYPT_TWOFISH );
	AddClient("3.0.1a",   0x2daba7ed, 0xa3c17e7f, CRYPT_TWOFISH );
	AddClient("3.0.0g",   0x2d93a5fd, 0xa3dd527f, CRYPT_TWOFISH );
	AddClient("3.0.0c",   0x2d93a5fd, 0xa3dd527f, CRYPT_TWOFISH );
	AddClient("3.0.0a",   0x2d93a5fd, 0xa3dd527f, CRYPT_TWOFISH );
	AddClient("2.0.8t",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8s",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8r",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8q",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8o",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8n",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8m",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.8l",   0x2cd3257d, 0xa37f527f, CRYPT_TWOFISH );
	AddClient("2.0.7",    0x2c9bc78d, 0xa35bfe7f, CRYPT_TWOFISH );
	AddClient("2.0.6c",   0x2c43ed9d, 0xa334227f, CRYPT_TWOFISH );
	AddClient("2.0.4d",   0x2df385bd, 0xa3ed127f, CRYPT_TWOFISH );
	AddClient("2.0.4c",   0x2df385bd, 0xa3ed127f, CRYPT_TWOFISH );
	AddClient("2.0.4b",   0x2df385bd, 0xa3ed127f, CRYPT_TWOFISH );
	AddClient("2.0.4a",   0x2df385bd, 0xa3ed127f, CRYPT_TWOFISH );
	AddClient("2.0.4",    0x2df385bd, 0xa3ed127f, CRYPT_TWOFISH );
	AddClient("2.0.3b5",  0x2dbbb7cd, 0xa3c95e7f, CRYPT_BOTH    );
	AddClient("2.0.3",    0x2dbbb7cd, 0xa3c95e7f, CRYPT_BOTH    );
*/
	AddClient("2.0.0",    0x2d13a5fd, 0xa39d527f, CRYPT_BLOWFISH);
	AddClient("1.26.4",   0x32750719, 0x0a2d100b, CRYPT_BLOWFISH);
	AddClient("client",   0,          0,          CRYPT_CLIENT  , false); // wtf is this? :P
	AddClient("ignition", 0,          0,          CRYPT_NONE    );
	AddClient("none",	  0,          0,          CRYPT_NONE    ); //dave added 3/14, avoid newbie confusion
	AddClient("uorice",	  0,          0,          CRYPT_NONE    ); //dave added 3/14, avoid newbie confusion
}

bool CCryptInfo::AddClient(std::string sClient,
						   unsigned int uiKey1,
						   unsigned int uiKey2,
						   ECryptType eType,
						   bool bEnabled /*=true*/)
{
	// Does it already exist?
	CryptInfoList::const_iterator cIter = CryptList.find(sClient);
	if ( cIter != CryptList.end() )
		return false;

	// Add it :)
	TCryptInfo encInfo = {uiKey1, uiKey2, eType, bEnabled};
	CryptList[sClient] = encInfo;
	return true;
}

bool CCryptInfo::LookupClient(std::string sClient, TCryptInfo & refInfo)
{
	CryptInfoList::const_iterator cIter = CryptList.find(sClient);
	if ( cIter == CryptList.end() )
		return false;

	refInfo = cIter->second;
	return true;
}
