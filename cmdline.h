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
 * cmdline.h - header file for command line parser
 *
 */

#ifndef _CMDLINE_H
#define _CMDLINE_H 1

#ifndef NULL
#define NULL 0
#endif

int parm_setup(int argc,char **argv,char *str_prms,char *flag_prms,
		char *num_prms);

/* My own favorite GETOPT substitute */
/* letter <= 32 means get non-flagged regular parameter # letter */
/* num_types  = letters that take numerical parameters */
/* str_types  = letters that take string parameters */
/* flag_types = letters that take no parameters */
/* str_value  = returned string value */
/* num_value  = returned numerical value */
/* argv/argc  = parameter list / parameter count */

int read_next_parm(char letter,
                   char *num_types, char *str_types, char *flag_types,
                   char **str_value, long int *num_value,
                   int argc, char **argv);

int read_parm(char letter, char **str_value, long int *num_value);
#define NON_FLAGGED 1

#endif /* _CMDLINE_H */
