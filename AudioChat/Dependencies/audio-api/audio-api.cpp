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

int PlaybackDevice::getFD() {
    pollfd* pfds = new pollfd();
    snd_pcm_poll_descriptors(handle, pfds, 1);
    return pfds->fd;
}

// Capture Device

CaptureDevice::CaptureDevice() {
    handle = NULL;
    hw_params = NULL;
}

void CaptureDevice::open(const char* device) {
    int err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        printf("Error CaptureDevice::open : %s\n", snd_strerror(err));
        exit(1);
    }
}

void CaptureDevice::setParams(snd_pcm_format_t format, unsigned int rate,
                              unsigned int latency) {
    int err;
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_any(handle, hw_params)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_access(
             handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_format(handle, hw_params, format)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &rate, 0)) <
        0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_channels(handle, hw_params, 1)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params(handle, hw_params)) < 0) {
        printf("Error CaptureDevice::setParams : %s\n", snd_strerror(err));
        exit(1);
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_prepare(handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
                snd_strerror(err));
        exit(1);
    }
}

void CaptureDevice::capture(void* buffer, unsigned int buffLen) {
    int frames = snd_pcm_readi(handle, buffer, buffLen);
}

void CaptureDevice::close() { snd_pcm_close(handle); }

int CaptureDevice::getFD() {
    pollfd* pfds = new pollfd();
    snd_pcm_poll_descriptors(handle, pfds, 1);
    return pfds->fd;
}