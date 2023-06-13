#include "audio-api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DataBuffer::DataBuffer() {
    rawBuffer = NULL;
    bufferSize = 0;
    readIndex = 0;
    writeIndex = 0;
    socketFD = -1;
}

void DataBuffer::initialize(size_t formatSize, unsigned int bufferSize,
                            short port) {
    this->bufferSize = formatSize * bufferSize;
    rawBuffer = malloc(this->bufferSize);
    memset(rawBuffer, 0, this->bufferSize);
    if (port != -1) {
        // Create socket
        socketFD = Socket(AF_INET, SOCK_DGRAM, 0);
        sockAddr = SocketAddr(AF_INET, port, "127.0.0.1");
        Bind(socketFD, sockAddr);
    }
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
                              unsigned long frames, short port) {
    captureBuffer.initialize(sizeof(float), frames, port);
    error = Pa_OpenDefaultStream(&captureStream, 1, 0, format, rate, frames,
                                 captureCallback, &captureBuffer);
    catchError(error);
}
void Audio::openPlaybackStream(PaSampleFormat format, unsigned int rate,
                               unsigned long frames) {
    playbackBuffer.initialize(sizeof(float), frames, -1);
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
    memcpy(data->rawBuffer, input, sizeof(float) * frameCount);

    sendto(data->socketFD, input, 1, 0, (SA *)&data->sockAddr,
           sizeof(data->sockAddr));
    return 0;
}
int Audio::playbackCallback(const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    DataBuffer *data = (DataBuffer *)userData;
    memcpy(output, data->rawBuffer, sizeof(float) * frameCount);
    memset(data->rawBuffer, 0, sizeof(float) * frameCount);
    return 0;
}

void Audio::catchError(PaError error) {
    if (error != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(error));
    }
}

int Audio::getCaptureFD() { return captureBuffer.socketFD; }