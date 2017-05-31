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
 *  linux_dsp.c - Support for the Linux DSP driver from the Voxware(C) Drivers.
 */

#ifdef LINUX
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/soundcard.h>
#include <bytesex.h>
#include "main.h"
#include "dsp.h"

static int audio;

int get_dsp_device(void)
{
  uint32 j;

  audio=open("/dev/dsp",O_WRONLY,0);
  if (audio < 1)
  {
    printf("Could not open audio device!\n");
    exit(1);
  }
  if (stereo)
  { 
    if (ioctl(audio,SNDCTL_DSP_STEREO,&stereo) == -1)
      stereo = 0;
  }
  if (bit16)
  {
    j = 16;
    if (ioctl(audio,SNDCTL_DSP_SAMPLESIZE,&j) == -1) bit16 = 0;
    if (j != 16) bit16 = 0;
  }
  j = mixspeed;
  if (ioctl(audio,SNDCTL_DSP_SPEED,&j) == -1) 
  { 
    printf("Error setting sample speed\n");
    exit(1);
  }
  mixspeed = j;
  if (ioctl(audio,SNDCTL_DSP_GETBLKSIZE, &audio_buffer_size) == -1)
  {
    printf("Unable to get audio blocksize\n");
    exit(1);
  }
  if ((audio_buffer_size < 4096) || (audio_buffer_size > 131072))
  {
    printf("Invalid audio buffer size: %d\n",audio_buffer_size);
    exit(1);
  }
  if (!(audio_start_buffer = (uint8 *) malloc(audio_buffer_size)))
  {
    printf("Could not get audio buffer memory!\n");
    exit(1);
  }
  audio_end_buffer = &audio_start_buffer[audio_buffer_size];
  audio_curptr = audio_start_buffer;  
  return;
}

void write_dsp_device(void *buf, int size) {
        write(audio,audio_start_buffer,audio_buffer_size);
	return;
}

void close_dsp_device() {
    close(audio);
    return;
}

#endif /* ?LINUX */

