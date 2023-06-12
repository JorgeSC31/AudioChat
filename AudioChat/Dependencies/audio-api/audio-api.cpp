#include "audio-api.h"

#include <stdio.h>
#include <stdlib.h>

Audio::Audio() {}

void Audio::initialize() {
    error = Pa_Initialize();
    catchError(error);
}

void Audio::openCaptureStream() {
    error = Pa_OpenDefaultStream(&captureStream, 1, 0, paFloat32, 8'000, 256,
                                 captureCallback, NULL);
    catchError(error);
}
void Audio::openPlaybackStream() {
    error = Pa_OpenDefaultStream(&playbackStream, 0, 1, paFloat32, 8'000, 256,
                                 playbackCallback, NULL);
    catchError(error);
}

void Audio::startCaptureStream() {
    error = Pa_StartStream(captureStream);
    catchError(error);
}
void Audio::startPlaybackStream() {
    error = Pa_StartStream(playbackStream);
    catchError(error);
}

void Audio::stopCaptureStream() {
    error = Pa_StopStream(captureStream);
    catchError(error);
}
void Audio::stopPlaybackStream() {
    error = Pa_StopStream(playbackStream);
    catchError(error);
}

int Audio::captureCallback(const void *input, void *output,
                           unsigned long frameCount,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags, void *userData) {}
int Audio::playbackCallback(const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
}

void Audio::catchError(PaError error) {
    if (error != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(error));
    }
}
