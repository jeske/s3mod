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
 * gus.h - Header for Gravis Ultrasound support
 *
 */

#ifndef _GUS_H
#define _GUS_H 

#include <sys/soundcard.h>

#define ERR_SEQUENCER    51
#define ERR_NOGUS        53
#define PAN_HARDWARE      1


extern int ticks_per_division;
extern double tick_duration;
extern double this_time,next_time;

void sync_time(void);

extern unsigned char _seqbuf[];
extern int _seqbuflen, _seqbufptr;
unsigned short base_freq_table[];
extern unsigned int gus_total_mem;

int gus_mem_free(int dev);
void play_mod_gus(int loud);
int get_gus_device(void);
void gus_set_speed(int parm);

#endif /* _GUS_H */
