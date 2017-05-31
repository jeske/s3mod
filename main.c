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
 * main.c - The main part of the s3mod program.
 *
 */

#include "config.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "mod.h"
#include "play.h"
#include "main.h"
#include "cmdline.h"
#include "dsp.h"
#ifdef GUS
#include "gus.h"
#endif /* GUS */

/*
 * Beginning of real code 
 */

struct option_struct options;
int gus_dev=-1;
int use_gus=1;
int stereo = 0;
int bit16 = 0;

void help(void)
{
  printf("\nS3MOD - S3M/MOD tracker (%s) for Linux/Sun/DEC/SGI\n",VERSION);
#ifdef LINUX
  printf("         Linux Support by Daniel Marks \n");
  printf("           GUS Support by David Jeske (jeske@uiuc.edu)\n");
#endif /* LINUX */
#if defined(__sgi)
  printf("           SGI Support by Mike Muuss (Mike@arl.mil)\n\n");
#endif /* __sgi */
#if defined(SUN) || defined(DEC)
  printf("    SUN/DEC AF Support by Ricky Beam (cramer@catt.ncsu.edu)\n\n");
#endif /* SUN || DEC */
  printf("(C) Copyright 1994 by Daniel L. Marks and David Jeske\n\n");
  printf("For information email (s3mod@uiuc.edu).\n");
  printf("s3mod [-sbfqnl] [sampling frequency] filename\n");
  printf("     -s stereo\n");
#ifdef LINUX
  printf("     -b 16 bit samples\n");
#endif /* ?LINUX */
  printf("     -f set frequency\n");
  printf("     -q quiet (don't print anything)\n");
  printf("     -n don't loop\n");
  printf("     -l loud mode (shift voices louder)\n");
#ifdef LINUX
  printf("     -g do NOT use GUS native mode\n");
#endif /* ?LINUX */
  printf("     -p PAL speed adjustment (145bpm start)\n");
  printf("s3mod -sbf 44100 foobar.mod\n");
  printf("   plays in stereo, 16 bits, 44.1 kHz\n\n");
  printf("This player plays Screamtracker 3 files, and\n");
  printf("4,6,or 8 track MODs.\n\n");
#ifdef DEC
  printf("NOTE: DEC AF support is in it's ALPHA stages\n\n");
#endif /* ?LINUX */
  exit(1);
}


void get_audio_device(void)
{
  int found_gus=0;

#ifdef GUS 
  if (use_gus)
    {
      found_gus = get_gus_device();
    }

 if (!found_gus)
   {gus_dev=-1;
    get_dsp_device();
   }
#else /* GUS */
  get_dsp_device();
#endif /* GUS */

}


void main(int argc, char **argv)
{
  uint32 j;
  char *filename;
  long int frequency;
  int quiet = 0;
  int loud = 0;
  int use_pal = 0;

  mixspeed = DSP_SPEED;

  if (argc < 2) 
    { help();
      exit(1);
    }
 
  parm_setup(argc,argv,"","SsBbQqNnLlGgPp","Ff");

  if (read_parm(NON_FLAGGED, &filename, NULL) != 1)
  {
    help();
    printf("No executable filename found!\n");
    exit(1);
  }

  if (read_parm('S',NULL,NULL))
    stereo=1;
  else
    stereo=0;
  if (read_parm('B',NULL,NULL))
    bit16=1;
  else
    bit16=0;
  if (read_parm('Q',NULL,NULL))
    quiet=1;
  else
    quiet=0;
  if (read_parm('N',NULL,NULL))
    loop_mod = 0;
  else
    loop_mod = 1;
  if (read_parm('L',NULL,NULL))
    loud = 1;
  else
    loud = 0;
#ifdef GUS
  if (read_parm('G',NULL,NULL))
    use_gus = 0;
  else
    use_gus = 1;
#endif /* GUS */
  if (read_parm('P',NULL,NULL))
    use_pal = 1;
  else
    use_pal = 0;


#ifdef SIXTEEN_BIT_PLAY_ONLY
bit16 = 1;
#endif /* SIXTEEN_BIT_PLAY_ONLY */

  if (read_parm('F',NULL,&frequency))
    {
      mixspeed = (frequency);
      if ((mixspeed < 8000) || (mixspeed > 44100))
	{ help();
	  printf("Invalid Frequency: %ld",frequency);
	  exit(1);
	}
    }
    
  if (use_pal)
    { options.def_tempo = DEF_TEMPO_PAL; 
      options.def_bpm   = DEF_BPM_PAL;
    }
  else
    { options.def_tempo = DEF_TEMPO_NTSC;
      options.def_bpm   = DEF_BPM_NTSC;
    }


  get_audio_device();

  if (load_s3m(filename,&mod,quiet))
    if (load_mod(filename,&mod,quiet))
    {
      printf("File is not a valid mod or s3m!\n");
      exit(1);
    }



  if (!quiet) 
  {

    if (gus_dev==-1)
      printf("Playing \"%s\"\nAt rate %d, %d bits, %s, blocksize %d\n",
	     filename,
	     mixspeed,
	     bit16 ? 16 : 8,
	     stereo ? "stereo" : "mono",
	     audio_buffer_size);
    
    else {
#ifdef GUS
      printf("GUS (ALPHA support) Playing \"%s\" \n",filename);
      printf("Using %d of %d bytes of GUS RAM\n",
	     (gus_total_mem - gus_mem_free(gus_dev)),
	     (gus_total_mem));
#else
      printf("GUS support not compiled in...\n");
      exit(1);
#endif /* GUS */
    }


  }


  if (gus_dev!=-1) {
#ifdef GUS    
    play_mod_gus(loud);
#else
    printf("GUS support not compiled in...\n");
    exit(1);
#endif
  } else {
    play_mod(loud);
  }
  
  close_dsp_device();
  free(audio_start_buffer);
}
















