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
 * main.h - header for main and global variables
 *
 */

#ifndef _MAIN_H
#define _MAIN_H 1

struct option_struct {
	int gravis_playing;
       	int main_volume;
	int def_tempo;
	int def_bpm;
};


extern struct option_struct options;
extern int gus_dev;
extern int stereo;
extern int bit16;
extern uint32 mixspeed;


void help(void);
void get_audio_device(void);
void main(int argc, char **argv);


#endif /* _MAIN_H */
