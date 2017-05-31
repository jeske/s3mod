/***************************************************************************
 *   S3m/Mod player by Daniel Marks (dmarks@ais.net)
 *   GUS support by David Jeske (jeske@uiuc.edu)
 *
 * (C) 1994,1995 By Daniel Marks and David Jeske
 *
 * While we retain the copyright to this code, this source code is FREE.
 * You may use it in any way you wish, in any product you wish. You may 
 * NOT steal the copyright for this code from us.
 *
 * We respectfully ask that you email one of us, if possible, if you
 * produce something significant with this code, or if you have any bug 
 * fixes to contribute.  We also request that you give credit where
 * credit is due if you include part of this code in a program of your own.
 *                                                 
 * Email: s3mod@uiuc.edu
 *        jeske@uiuc.edu
 *
 * See the associated README file for Thanks
 ***************************************************************************
 *
 * config.h - compile time configuration options and system specific defines
 *
 */

#ifndef _CONFIG_H
#define _CONFIG_H 1

/***************************************************************************/
/* The following parameters are compile time defaults */
/***************************************************************************/

/* For large amounts of printed debuging information */
/* #define DEBUG /* */

/* Default DSP mixing speed */
#define DSP_SPEED 22000


/***************************************************************************/
/* The following are system specific settings */
/***************************************************************************/

#if defined(SUN)
#define SIXTEEN_BIT_PLAY_ONLY
#define BIT_32
#define __BYTE_ORDER 4321
#define DSP

#elif defined(DEC)
#define SIXTEEN_BIT_PLAY_ONLY
#define DSP

#elif defined(__sgi)
#define BIT_32
#define __BYTE_ORDER 4321
#define DSP

#elif defined(LINUX)
#define BIT_32
#undef NEAR_FAR_PTR
#define DSP
#ifndef GUS
#define GUS
#endif /* GUS */

#elif defined(MSDOS)
#define NEAR_FAR_PTR
#define DSP

#endif /* OS/MACH TYPE */

/***************************************************************************/
/* This should be left alone */
/***************************************************************************/

#define VERSION "v1.09"
#define ROT_BUF_SIZE 10240

/***************************************************************************/
/* 16/32 Bit and Byte Order hacks
/***************************************************************************/

#ifdef BIT_32
typedef short int int16;
typedef unsigned short int uint16;
typedef int int32;
typedef unsigned int uint32;
typedef char int8;
typedef unsigned char uint8;
#else
typedef int int16;
typedef unsigned int uint16;
typedef long int int32;
typedef unsigned long int uint32;
typedef char int8;
typedef unsigned char uint8;
#endif

#define endian_switch(x) (((((uint16)(x)) & 0xFF00) >> 8) | \
			  ((((uint16)(x)) & 0xFF) << 8))

#define long_endian_switch(x) ( ((((uint32)(x)) & 0xFF00) << 8) | \
			        ((((uint32)(x)) & 0xFF) << 24) | \
			        ((((uint32)(x)) & 0xFF0000) >> 8) | \
			        ((((uint32)(x)) & 0xFF000000) >> 24))

#if __BYTE_ORDER == 4321
#define big_endian(x) (x)
#define long_big_endian(x) (x)
#define little_endian(x) (endian_switch(x))
#define long_little_endian(x) (long_endian_switch(x))
#else
#define big_endian(x) (endian_switch(x))
#define long_big_endian(x) (long_endian_switch(x))
#define little_endian(x) (x)
#define long_little_endian(x) (x)
#endif


#endif /* _CONFIG_H */
