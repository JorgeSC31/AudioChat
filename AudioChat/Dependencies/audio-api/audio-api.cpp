#include "audio-api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DataBuffer::DataBuffer() {
    rawBuffer = NULL;
    bufferSize = 0;
    readSlot = 0;
    writeSlot = -1;
    slots = 0;
    socketFD = -1;
}

void DataBuffer::initialize(short port) {
    slots = 8;
    this->bufferSize = BUFFER_SIZE * slots;
    rawBuffer = malloc(this->bufferSize);
    memset(rawBuffer, 0, this->bufferSize);
    if (port != -1) {
        // Create socket
        socketFD = Socket(AF_INET, SOCK_DGRAM, 0);
        sockAddr = SocketAddr(AF_INET, port, "127.0.0.1");
        Bind(socketFD, sockAddr);
    }
}

void DataBuffer::push(const void *buffer) {
    if (writeSlot == -1) writeSlot = 0;
    FORMAT *formatBuffer = (FORMAT *)rawBuffer;
    formatBuffer = &formatBuffer[writeSlot * BUFFER_SIZE];
    memcpy(formatBuffer, buffer, BUFFER_SIZE);
    writeSlot = (writeSlot + 1) % slots;
}

void *DataBuffer::pop() {
    FORMAT *formatBuffer = (FORMAT *)rawBuffer;
    formatBuffer = &formatBuffer[readSlot * BUFFER_SIZE];
    void *buffer = formatBuffer;
    if (writeSlot != -1) {
        readSlot = (readSlot + 1) % slots;
    }
    return buffer;
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
    captureBuffer.initialize(port);
    error = Pa_OpenDefaultStream(&captureStream, 1, 0, format, rate, frames,
                                 captureCallback, &captureBuffer);
    catchError(error);
}
void Audio::openPlaybackStream(PaSampleFormat format, unsigned int rate,
                               unsigned long frames) {
    playbackBuffer.initialize(-1);
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
    data->push(input);

    sendto(data->socketFD, input, 1, 0, (SA *)&data->sockAddr,
           sizeof(data->sockAddr));
    return 0;
}
int Audio::playbackCallback(const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    DataBuffer *data = (DataBuffer *)userData;
    void *buffer = data->pop();
    memcpy(output, buffer, BUFFER_SIZE);
    memset(buffer, 0, BUFFER_SIZE);
    return 0;
}

void Audio::catchError(PaError error) {
    if (error != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(error));
    }
}

int Audio::getCaptureFD() { return captureBuffer.socketFD; }