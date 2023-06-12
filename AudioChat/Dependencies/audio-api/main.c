// #include <bits/stdc++.h>
#include <portaudio.h>
#include <stdio.h>
#include <string.h>

// using namespace std;

#define SAMPLE_RATE (44100)
#define BUFFER_SIZE (256)
#define NUM_SECONDS 2

typedef struct {
    int maxIndex;
    int recordedIndex;
    int outputIndex;
    float *recordedSample;
} paTestData;

float gBuffer[BUFFER_SIZE * 4];

static int PlaybackCallback(const void *inputBuffer, const void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    paTestData *data = (paTestData *)userData;
    float *in = &data->recordedSample[data->outputIndex];
    float *out = (float *)outputBuffer;

    for (int i = 0; i < framesPerBuffer; i++) {
        out[i] = in[data->outputIndex++];
    }
    return 0;
}

static int CaptureCallback(const void *inputBuffer, const void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags, void *userData) {
    paTestData *data = (paTestData *)userData;
    float *out = &data->recordedSample[data->recordedIndex];
    float *in = (float *)inputBuffer;
    // printf("caputreIndex: %d\n", data->recordedIndex);

    for (int i = 0; i < framesPerBuffer; i++) {
        out[data->recordedIndex++] = in[i];
    }
    return 0;
}

void record(int sec, void *userData) {
    PaError err;
    PaStream *captureStream;
    paTestData *data = (paTestData *)userData;

    data->outputIndex = 0;
    data->recordedIndex = 0;

    err = Pa_OpenDefaultStream(
        &captureStream, 1,        /* no input channels */
        0,                        /* stereo output */
        paFloat32,                /* 32 bit floating point output */
        SAMPLE_RATE, BUFFER_SIZE, /* frames per buffer, i.e. the number
                                 of sample frames that PortAudio will
                                 request from the callback. Many apps
                                 may want to use
                                 paFramesPerBufferUnspecified, which
                                 tells PortAudio to pick the best,
                                 possibly changing, buffer size.*/
        CaptureCallback,          /* this is your callback function */
        data);                    /*This is a pointer that will be passed
                                      to your callback*/
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_StartStream(captureStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    /* Sleep for several seconds. */
    Pa_Sleep(sec * 1000);

    err = Pa_StopStream(captureStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void play(int sec, void *userData) {
    PaError err;
    PaStream *playbackStream;
    paTestData *data = (paTestData *)userData;

    err = Pa_OpenDefaultStream(
        &playbackStream, 0,       /* no input channels */
        1,                        /* stereo output */
        paFloat32,                /* 32 bit floating point output */
        SAMPLE_RATE, BUFFER_SIZE, /* frames per buffer, i.e. the number
                                 of sample frames that PortAudio will
                                 request from the callback. Many apps
                                 may want to use
                                 paFramesPerBufferUnspecified, which
                                 tells PortAudio to pick the best,
                                 possibly changing, buffer size.*/
        PlaybackCallback,         /* this is your callback function */
        data);                    /*This is a pointer that will be passed
                                      to your callback*/

    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_StartStream(playbackStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    Pa_Sleep(sec * 1000);

    err = Pa_StopStream(playbackStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

int main() {
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    paTestData data;
    data.maxIndex = 10024 * BUFFER_SIZE;
    data.recordedSample = (float *)malloc(data.maxIndex * sizeof(float));

    record(5, &data);
    play(4, &data);

    err = Pa_Terminate();
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    return 0;
}