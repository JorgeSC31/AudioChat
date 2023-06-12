#include "audio-api.h"

#include <stdio.h>
#include <stdlib.h>

DataBuffer::DataBuffer() {
    rawBuffer = NULL;
    bufferSize = 0;
    readIndex = 0;
    writeIndex = 0;
}

void DataBuffer::initialize(size_t formatSize, unsigned int bufferSize) {
    this->bufferSize = bufferSize;
    rawBuffer = malloc(formatSize * bufferSize);
}

Audio::Audio() {
    playbackStream = NULL;
    captureStream = NULL;
    error = paNoError;
}

void Audio::initialize() {
    error = Pa_Initialize();
    catchError(error);
}

void Audio::openCaptureStream(PaSampleFormat format, unsigned int rate,
                              unsigned long frames) {
    captureBuffer.initialize(format, frames);
    error = Pa_OpenDefaultStream(&captureStream, 1, 0, format, rate, frames,
                                 captureCallback, &captureBuffer);
    catchError(error);
}
void Audio::openPlaybackStream(PaSampleFormat format, unsigned int rate,
                               unsigned long frames) {
    playbackBuffer.initialize(format, frames);
    error = Pa_OpenDefaultStream(&playbackStream, 0, 1, format, rate, frames,
                                 playbackCallback, &playbackBuffer);
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
                           PaStreamCallbackFlags statusFlags, void *userData) {
    DataBuffer *data = (DataBuffer *)userData;
    float *out = &((float *)data->rawBuffer)[0];
    float *in = (float *)input;

    for (int i = 0; i < frameCount; i++) {
        out[i] = in[i];
    }
}
int Audio::playbackCallback(const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    DataBuffer *data = (DataBuffer *)userData;
    float *in = &((float *)data->rawBuffer)[0];
    float *out = (float *)output;

    for (int i = 0; i < frameCount; i++) {
        out[i] = in[i];
    }
}

void Audio::catchError(PaError error) {
    if (error != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(error));
    }
}
