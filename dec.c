/******************************************************************
 * SUN/DEC AF Support by Ricky Beam (cramer@catt.ncsu.edu)
 *
 ******************************************************************/


#ifdef DEC
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include "main.h"
#include "dsp.h"

static int audio;

int get_dsp_device(void)
{
    uint32 j;

    audio = fileno(popen("aplay -f -e pcm16", "wb"));
    if (audio < 1)
    {
	printf("Could not open audio device!\n");
	exit(1);
    }
	mixspeed = 8000;
	bit16 = 1;

    /* I don't know; what's a good size? */
    audio_buffer_size = 32768;
    if (!(audio_start_buffer = (uint8 *) malloc(audio_buffer_size)))
    {
	printf("Could not get audio buffer memory!\n");
	exit(1);
    }
    audio_end_buffer = &audio_start_buffer[audio_buffer_size];
    audio_curptr = audio_start_buffer;
}

void write_dsp_device(void *buf, int size) {
        write(audio,audio_start_buffer,audio_buffer_size);
        return;
}
 
void close_dsp_device() {
    close(audio);
    return;
}
 
#endif /* ?DEC */
