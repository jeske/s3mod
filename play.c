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
 * play.c - the player itself
 *
 */

#include "config.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "main.h"
#include "mod.h"
#include "play.h"
#include "dsp.h"

#ifdef GUS
#include <sys/ultrasound.h>
#include "gus.h"
#endif /* GUS */


union
{
  uint8                   rot_buf[ROT_BUF_SIZE];
  uint16                  rot_buf16[ROT_BUF_SIZE];
} buf;

void updatetracks(void)
{
  track_info_ptr track;
  int16 count;

  tempo_wait = tempo;
  if (row >= 64)
  {
    if (order_pos >= mod.song_length_patterns)
    {
      order_pos = mod.song_repeat_patterns;
      if (order_pos >= mod.song_length_patterns)
      {
        order_pos = 0;
        mod_done = 1;
      }
    }
    row = break_row;
    break_row = 0;
    if (mod.positions[order_pos] == 0xFF)
    {
      mod_done = 1;
      return;
    }
    if (mod.s3m)
      note = mod.patterns[mod.positions[order_pos]];
      else
      note = mod.patterns[mod.positions[order_pos]] + 
             (((uint16)row) * sizeof(uint8) * 4 * mod.tracks);
    order_pos++;
  }
  row++;
  if (mod.s3m)
    get_track_s3m(&note);
    else
    {
      track = tracks;
      for (count=0;count<mod.tracks;count++)
        get_track(track++,&note);
     
    }
  track = tracks;
  for (count=0;count<mod.tracks;count++)
  {
    track->playing_period = track->period;
    track->playing_volume = track->volume;
    track++;
  }

#ifdef DEBUG
  printf("00-01-02-03-04-05-06-07-pattern %d row %d\n",order_pos,row);
#endif
  return;
}


void play_mod(int loud)
{
  register uint8 *c;
  register uint16 *d;
  int16 i;
  track_info_ptr track;
  int16 count;

  startplaying(loud);
  mod_done = 0;
  while (!mod_done)
  {
    if (--tempo_wait)
      {
	track = tracks;
	for (count=0;count<mod.tracks;count++)
	  beattrack(track++);
      }
    else
      updatetracks();

    track = tracks;
    if (bit16)
    {
      if (stereo)
         for (d=&buf.rot_buf16[bpm_samples << 1];d > buf.rot_buf16;*(--d) = 0x8000);
         else
         for (d=&buf.rot_buf16[bpm_samples];d > buf.rot_buf16;*(--d) = 0x8000);
    } else
    {
      if (stereo)
         for (c=&buf.rot_buf[bpm_samples << 1];c > buf.rot_buf;*(--c) = 0x80);
         else
         for (c=&buf.rot_buf[bpm_samples];c > buf.rot_buf;*(--c) = 0x80);
    }
    for (i=0;i<mod.tracks;i++)
    {

      if (bit16)
      {
        if (stereo)
          mixtrack_16_stereo(track,buf.rot_buf16,bpm_samples,
                             i & 0x01);
          else
          mixtrack_16_mono(track,buf.rot_buf16,bpm_samples);
      } else
      {
        if (stereo)
          mixtrack_8_stereo(track,buf.rot_buf,bpm_samples,
                             i & 0x01);
          else
          mixtrack_8_mono(track,buf.rot_buf,bpm_samples);
      }
      track++;
    }
    c = (uint8 *) buf.rot_buf;
    if (stereo)
    {
      if (bit16)
        d = (uint16 *) &c[bpm_samples << 2];
        else
        d = (uint16 *) &c[bpm_samples << 1];
    } else
    {
      if (bit16)
        d = (uint16 *) &c[bpm_samples << 1];
        else
        d = (uint16 *) &c[bpm_samples];
    }
    while (c < ((uint8 *)d))
    {
      if (audio_curptr >= audio_end_buffer)
      {
      	write_dsp_device(audio_start_buffer,audio_buffer_size);
        audio_curptr = audio_start_buffer;
      }
      *audio_curptr++ = *c++;
    }
  } 
}
