#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include "wavFile.h"
#include <stdio.h>
#define PCM_DEVICE "default"

WAVHEADER wavheader;

int main(int argc, char** argv)
{
	int fd = -1;
	int rc, buf_size, dir;
	long loop, count;
	int channels;
	int format;
	unsigned int val;
	char *buffer;

	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;

	if(argc <= 1) {
		printf("usage : %s filename\n", argv[0]);
		return -1;
	} else {
		printf("Playing file : %s\n", argv[1]);
	}

	if((fd = open(argv[1], O_RDONLY)) == -1) {
		printf("Could not open the specified wave file : %s\n", argv[1]);
		exit(-1);
	}

	// Read Head of wave file
	if((count = read(fd, &wavheader, sizeof(WAVHEADER))) < 1) {
		printf("Could not read wave data\n");
		exit(-1);
	}
	
	rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(rc<0) {
	  fprint(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
	  return -1;
	}

	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle,params);

	channels = wavheader.nChannels;
	printf("wave channel mode :%s\n", (channels)? "Stero":"Mono");
	snd_pcm_hw_params_set_channels(handle, params, channels);

	printf("WaveByes: %d\n", wavheader.nblockAlign);
	switch(wavheader.nblockAlign) {
	  case 1:
		format = SND_PCM_FORMAT_U8;
		break;
	  case 2:
		format = (channels==1)? SND_PCM_FORMAT_S16_LE : SND_PCM_FORMAT_U8;
		break;
	  case 4:
		format= SND_PCM_FORMAT_S16_LE;
		break;
	  default:
		printf("Unknown byte rate for sound\n");
		break;
	};
	
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(handle, params, format);

	printf("Wave Sampling Rate : 0x%d\n", wavheader.sampleRate);
	val = wavheader.sampleRate;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

	frames=32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	rc = snd_pcm_hw_params(handle, params);
	if(rc<0) {
	  fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
	  return -1;
	}

	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	buf_size=frames*channels*((format==SND_PCM_FORMAT_S16_LE)?2:1);
	buffer=(char*)malloc(buf_size);

	snd_pcm_hw_params_get_period_time(params, &val, &dir);

	do {
		if((count = read(fd, buffer, buf_size)) <= 0) break;
		rc=snd_pcm_writei(handle, buffer, frames);

		if(rc ==-EPIPE) {
			fprintf(stderr,"underrun occured\n");
			snd_pcm_prepare(handle);
		} else if(rc<0){
			fprintf(stderr, "error from write: %s\n", snd_strerror(rc));
		} else if(rc!=(int)frames) {
		  fprintf(stderr, "short write, write %d frames \n", rc);
		} while(count == buf_size);


end:
  close(fd);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
  return 0;
}
