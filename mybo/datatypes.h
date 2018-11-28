#ifndef DATATYPES_H
#define DATATYPES_H

#ifndef WIN32
#define __int64 long
#endif

#if 1
#define UN_SETTINGS_CONSOLE 1
#endif

typedef unsigned char UInt8;
typedef char Int8;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned int UInt32;
typedef int Int32;

#if defined(WIN32)
typedef unsigned __int64 UInt64;
#endif

typedef __int64 Int64;
typedef unsigned long ULong;

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;

#if defined(WIN32)
typedef unsigned __int64 Uint64;
#endif

typedef char FourCC;
typedef unsigned int TextureID;

#define PI			3.141592

#endif