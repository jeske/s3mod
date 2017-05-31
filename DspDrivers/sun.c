/******************************************************************
 * SUN/DEC AF Support by Ricky Beam (cramer@catt.ncsu.edu)
 *
 ******************************************************************/


#ifdef SUN

#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <multimedia/audio_hdr.h>
#include <multimedia/audio_errno.h>
#include <multimedia/audio_device.h>
#include "main.h"
#include "dsp.h"

Audio_hdr hdr_save, hdr;
static int audio;

int get_dsp_device() {
  uint32 j;
  
  audio = open("/dev/audio",O_WRONLY,0);
  if (audio < 1) {
    printf("Could not open audio device|\n");
    exit(1);
  }
  
  /* Save the device configuration 'cause it don't reset on its own */
  audio_get_play_config(audio, &hdr_save);
  audio_get_play_config(audio, &hdr);
  
  hdr.encoding = AUDIO_ENCODING_LINEAR;
  hdr.bytes_per_unit = 2;
  if (audio_set_play_config(audio, &hdr) != AUDIO_SUCCESS)
    fprintf(stderr, "unable to set sample encoding\n");
  
  if (stereo)
    {  /* setup stereo */
      hdr.channels = 2;
      if (audio_set_play_config(audio, &hdr) != AUDIO_SUCCESS)
	fprintf(stderr, "unable to setup stereo\n");
      audio_get_play_config(audio, &hdr);
      stereo = (hdr.channels==1)?0:1;
    }
  
  if (bit16)
    {  /* set 16 bit? */
      j = 16;
      hdr.bytes_per_unit = 2;
      if (audio_set_play_config(audio, &hdr) != AUDIO_SUCCESS)
	fprintf(stderr, "unable to setup 16 bit samples\n");
      audio_get_play_config(audio, &hdr);
      j = (hdr.bytes_per_unit==2)?16:8;
      if (j != 16) bit16 = 0;
    }
  
  /* Setup playback speed */
  hdr.sample_rate = mixspeed;
  if (audio_set_play_config(audio, &hdr) != AUDIO_SUCCESS)
    fprintf(stderr, "unable to setup sample speed\n");
  audio_get_play_config(audio, &hdr);
  mixspeed = hdr.sample_rate;
  
  /* I don't know; what's a good size? */
  audio_buffer_size = 32768;
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

#endif /* SUN */
