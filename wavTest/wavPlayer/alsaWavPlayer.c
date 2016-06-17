#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
//   #define __USE_OSS__
#ifdef __USE_OSS__
#include <sys/soundcard.h>
#else
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#endif

#include "wavFile.h"

#define	BUF_SIZE	1024
#define	AUDIO_DEV	"/dev/dsp"


int main(int argc, char** argv)
{
	int fd = -1, fd_sd = -1;
	
	long loops;
	int rc;
	int size;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	char *buffer;
	
	long count;
	unsigned char data[BUF_SIZE];
	int channels;
	int format;
	WAVHEADER wavheader;

	// Initialize Header of Wave
	memset(&wavheader, 0, sizeof(wavHeader));

	if(argc <= 1) {
		printf("usage : %s filename\n", argv[0]);
		exit(-1);
	} else {
		printf("Playing file : %s\n", argv[1]);
	}

	if((fd = open(argv[1], O_RDONLY)) == -1) {
		printf("Could not open the specified wave file : %s\n", argv[1]);
		exit(-1);
	}

	// Read Head of wave file
	if((count = read(fd, &wavheader, sizeof(wavHeader))) < 1) {
		printf("Could not read wave data\n");
		exit(-1);
	}
#ifdef __USE_OSS__
	if((fd_sd = open(AUDIO_DEV, O_WRONLY)) == -1) {
		printf("Error : Cound not open the audio device\n");
		exit(-1);
	}
#else
	/*Open PCM device for playback*/
	rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(rc < 0){
		fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
		exit(-1);
	}
#endif
	
	channels = wavheader.nChannels - 1;
	printf("Wave Channel Mode : %s\n", (channels)? "Stereo":"Mono");
#ifdef __USE_OSS__
	if((ioctl(fd_sd, SNDCTL_DSP_STEREO, &channels)) == -1) {
		printf("Error : Cound not set Channels\n");
		exit(-1);
	} else {
		printf("Channel set to %s\n", (channels)? "Stereo":"Mono");
	}
#else
	// Allocate a hardware parameters object
	snd_pcm_hw_params_alloca(&params);
	
	//Fill it in with default values	
	snd_pcm_hw_params_any(handle, params);

	//Two channels (stereo)
	snd_pcm_hw_params_set_channels(handle, params, channels);
	
#endif

	//Set the desires hardware parameters

	printf("Wave Bytes : %d\n", wavheader.nblockAlign);
	switch(wavheader.nblockAlign) {
		case 1:
#ifdef __USE_OSS__			
			format = AFMT_U8;
#else
			format = SND_PCM_FORMAT_U8;
#endif
			break;
		case 2:
#ifdef __USE_OSS__			
			format = (!channels)?AFMT_S16_LE:AFMT_U8;
#else
			format = (!channels)?SND_PCM_FORMAT_S16_LE:SND_PCM_FORMAT_U8;
#endif
			break;
		case 4:
			printf("Stereo Wave file\n");
#ifdef __USE_OSS__			
			format = AFMT_S16_LE;
#else
			format = SND_PCM_FORMAT_S16_LE;
#endif
			break;
		default:
			printf("Unknown byte rate for sound\n");
			break;
	};

#ifdef __USE_OSS__	
	if((ioctl(fd_sd, SNDCTL_DSP_SETFMT, &format)) == -1) {
		printf("Error : Cound not set format\n");
		exit(-1);
	} else {
		printf("Format set to %d\n", format);
	}
#else
	//Interleaved mode
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	//Signed 16-bit littel-endian format
	snd_pcm_hw_params_set_format(handle, params, format);
#endif
	
	printf("Wave Sampling Rate : 0x%d\n", wavheader.sampleRate);
#ifdef __USE_OSS__
	if((ioctl(fd_sd, SNDCTL_DSP_SPEED, &wavheader.sampleRate)) == -1) {
		close(fd_sd);
		return -1;
	} else {
		printf("Speed rate set to %d\n", wavheader.sampleRate);
	}
#else
	//44100 bits/second sampling rate(CD Quality)
	val = wavheader.sampleRate;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
	
	//Set period size to 32 frames
	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
	
	//Write the parameters  to the driver
	rc = snd_pcm_hw_params(handle, params);

	//Use a buffer large enough to hold one period
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	size = frames * channels * ((format == SND_PCM_FORMAT_S16_LE)?2:1); //2bytes/sample, 2 channels
	buffer = (char*)malloc(size);
	
	//We want to loop for 5 seconds	
	snd_pcm_hw_params_get_period_time(params, &val, &dir);
#endif
	if(rc<0){
		fprintf(stderr, "Unable to set hw parameters: %s\n", snd_strerror(rc));
		return -1;
	}
	
	do {
//		memset(data, 0, BUF_SIZE);
		if((count = read(fd, buffer, BUF_SIZE)) <= 0) break;
#ifdef __USE_OSS__
		if(write(fd_sd, data, count) == -1) {
			printf("Error : Could not write to Sound Device\n");
			exit(-1);
		} else {
			printf("Play Sound : %d bytes\n", count);
		}
#else
		rc = snd_pcm_writei(handle, buffer, frames);
		if(rc == -EPIPE){
			//EPIPE means inderrun
			fprintf(stderr, "Underrun occurred\n");
			snd_pcm_prepare(handle);
		}else if(rc<0){
			fprintf(stderr, "error from write: %s\n", snd_strerror(rc));
		}else if(rc != (int)frames){
			fprintf(stderr, "short write, write %d frames\n", rc);
		}
#endif
	} while(count == BUF_SIZE);

end:
	close(fd);
#ifdef __USE_OSS__
	close(fd_sd);
#else
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(buffer);
#endif
	return 0;
}
