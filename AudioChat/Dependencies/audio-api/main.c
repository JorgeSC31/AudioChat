#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "alsa-lib.h"

static char *device = "default"; /* playback device */
#define lim 4 * 1024
unsigned char buffer[lim]; /* some random data */
// short buf[128];
int err;

snd_pcm_t *handle;

void open2() {
    int err2;

    if ((err2 = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) <
        0) {
        printf("Playback open error: %s\n", snd_strerror(err2));
        exit(EXIT_FAILURE);
    }
    if ((err2 = snd_pcm_set_params(handle, SND_PCM_FORMAT_U8,
                                   SND_PCM_ACCESS_RW_INTERLEAVED, 1, 4100, 1,
                                   50000)) < 0) { /* 0.5sec */
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}

void play() {
    // int sz = 256 * sizeof(char);
    int frames = snd_pcm_writei(handle, buffer, sizeof(buffer));
    if (frames < 0) frames = snd_pcm_recover(handle, frames, 0);
    if (frames < 0) {
        printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
        return;
    }
    if (frames > 0 && frames < (long)sizeof(buffer))
        printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer),
               frames);
}

int main() {
    unsigned int i;
    snd_pcm_t *capture_handle;
    snd_pcm_sframes_t frames;
    snd_pcm_hw_params_t *hw_params;

    if ((err = snd_pcm_open(&capture_handle, device, SND_PCM_STREAM_CAPTURE,
                            0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
                snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
                snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_access(
             capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_format(capture_handle, hw_params,
                                            SND_PCM_FORMAT_U8)) < 0) {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
        exit(1);
    }
    unsigned int rate = 4100;

    if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate,
                                               0)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, 1)) <
        0) {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
        exit(1);
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_prepare(capture_handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
                snd_strerror(err));
        exit(1);
    }
    // memset(buf, 0, sizeof(short) * 128);
    open2();
    // lim = 256;
    for (i = 0; i < 50; ++i) {
        if ((err = snd_pcm_readi(capture_handle, buffer, lim)) != lim) {
            fprintf(stderr, "read from audio interface failed (%s)\n",
                    snd_strerror(err));
            exit(1);
        }
        // for(int j=0; j<err; j++)
        // 	printf("%u", buffer[j]);
        // printf("\n");

        // for (i = 0; i < err; i++)
        // 	buf[i] = random() & 0xff;

        play();
    }
    printf("Termine ok\n");
    snd_pcm_close(capture_handle);
    exit(0);
}
