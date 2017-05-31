/******************************************************************
 * S G I Support by Mike Muuss (Mike@arl.mil)
 *
 ******************************************************************/

#if defined(__sgi)
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <audio.h>
#include "main.h"
#include "dsp.h"
static ALport	sgi_port;
static ALconfig	sgi_config;


void write_dsp_device(void *buf, int size)
{
	int	nsamp;
	int	ret;

	/* need number of samples, not number of bytes */
	nsamp = size/4;
	ret = ALwritesamps(sgi_port, buf, nsamp);
	if( ret != 0 )  {
		perror("ALwritesamps()");
		fprintf(stderr,"ALwritesamps(%d) failed, ret=%d\n", nsamp, ret);
		exit(1);
	}
}

static long sgi_params[] = { AL_OUTPUT_RATE, 0 };

int get_dsp_device(void)
{
	mixspeed = 44100;
	sgi_params[1] = mixspeed;
	if( ALsetparams( AL_DEFAULT_DEVICE, sgi_params, 2 ) < 0 )  {
		perror("ALsetparams()");
		fprintf(stderr,"ALsetparams() unable to set speed %d\n", mixspeed);
	}
	sgi_config = ALnewconfig();
	stereo = 1;
	bit16 = 1;
	ALsetwidth( sgi_config, AL_SAMPLE_16);   /* range -32768 to 32767. */
	ALsetchannels( sgi_config, AL_STEREO );
	sgi_port = ALopenport("s3m", "w", sgi_config );

	audio_buffer_size = 44100 * 4;
	 if (!(audio_start_buffer = (uint8 *) malloc(audio_buffer_size)))  {
		fprintf(stderr,"Could not get audio buffer memory!\n");
		exit(1);
	}
	audio_end_buffer = &audio_start_buffer[audio_buffer_size];
	audio_curptr = audio_start_buffer;
/* phony fd */
/* Should no longer be necessary under new code layout */
/*	audio = 99; */
}

void close_dsp_device(void)
{
	static char	zeros[4];	/* one sample of zeros */
	int	left;

	ALwritesamps(sgi_port, zeros, 1);
	while( (left = ALgetfilled(sgi_port)) > 1 )  {
		sleep(1);
	}
	ALcloseport(sgi_port);
	ALfreeconfig(sgi_config);
}


#endif /* SGI */
