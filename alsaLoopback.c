#include <stdio.h>

#include <stdlib.h>

#include <alsa/asoundlib.h>

#define BUF_BYTES 128

 

int main (int argc, char *argv[]) {

int err;

unsigned char buf[BUF_BYTES];

snd_pcm_t *playback_handle;

snd_pcm_t *capture_handle;

char* device = "default";

if (argc > 1) device = argv[1];

unsigned int rate = 8000;

unsigned int nchannels = 2;

snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

unsigned int buf_frames = BUF_BYTES / nchannels / 2;

 

if ((err = snd_pcm_open (&playback_handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {

fprintf (stderr, "cannot open audio device %s (%s)\n", device, snd_strerror (err)); exit (1);

}

if ((err = snd_pcm_set_params(playback_handle, format, SND_PCM_ACCESS_RW_INTERLEAVED, nchannels, rate, 1, 500000)) < 0) { /* 0.5sec */

fprintf(stderr, "Playback open error: %s\n", snd_strerror(err)); exit(1);

}

if ((err = snd_pcm_open (&capture_handle, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {

fprintf (stderr, "cannot open audio device %s (%s)\n", device, snd_strerror (err)); exit (1);

}

if ((err = snd_pcm_set_params(capture_handle, format, SND_PCM_ACCESS_RW_INTERLEAVED, nchannels, rate, 1, 500000)) < 0) { /* 0.5sec */

fprintf(stderr, "capture open error: %s\n", snd_strerror(err)); exit(1);

}

while(1) {

if ((err = snd_pcm_readi (capture_handle, buf, buf_frames)) != buf_frames) {

fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror (err)); exit (1);

}

if ((err = snd_pcm_writei (playback_handle, buf, buf_frames)) != buf_frames) {

fprintf (stderr, "write to audio interface failed (%s)\n", snd_strerror (err)); exit (1);

}

}

 

fprintf (stderr, "close handles\n");

snd_pcm_close (playback_handle);

snd_pcm_close (capture_handle);

return 0;

}
