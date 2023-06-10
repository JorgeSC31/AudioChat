#include "audio-api.h"

#include <stdio.h>

#include <iostream>

PlaybackDevice::PlaybackDevice() { handle = NULL; }

void PlaybackDevice::open(const char* device) {
    int err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        printf("Error PlaybackDevice::open : %s\n", snd_strerror(err));
        exit(1);
    }
}

void PlaybackDevice::setParams(snd_pcm_format_t format, unsigned int rate,
                               unsigned int latency) {
    int err = snd_pcm_set_params(handle, format, SND_PCM_ACCESS_RW_INTERLEAVED,
                                 1, rate, 1, latency);
    if (err < 0) {
        printf("Error PlaybackDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }
}

void PlaybackDevice::play(void* buffer, unsigned int buffLen) {
    int frames = snd_pcm_writei(handle, buffer, buffLen);
}

void PlaybackDevice::close() { snd_pcm_close(handle); }