// #include <bits/stdc++.h>
#include <portaudio.h>
#include <stdio.h>
#include <string.h>

// using namespace std;

#define SAMPLE_RATE (44100)
#define NUM_SECONDS 5

typedef struct {
    float left_phase;
    float right_phase;
} paTestData;

float gBuffer[256 * 4];

static int PlaybackCallback(const void *inputBuffer, const void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    memcpy(outputBuffer, gBuffer, sizeof(float) * framesPerBuffer);
    return 0;
}

static int CaptureCallback(const void *inputBuffer, const void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags, void *userData) {
    memcpy(gBuffer, inputBuffer, sizeof(float) * framesPerBuffer);
    return 0;
}

int main() {
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    PaStream *playbackStream;
    PaStream *captureStream;
    static paTestData data;
    data.left_phase = 0.0f;
    data.right_phase = 0.0f;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(
        &playbackStream, 0, /* no input channels */
        1,                  /* stereo output */
        paFloat32,          /* 32 bit floating point output */
        SAMPLE_RATE, 256,   /* frames per buffer, i.e. the number
                                   of sample frames that PortAudio will
                                   request from the callback. Many apps
                                   may want to use
                                   paFramesPerBufferUnspecified, which
                                   tells PortAudio to pick the best,
                                   possibly changing, buffer size.*/
        PlaybackCallback,   /* this is your callback function */
        &data);             /*This is a pointer that will be passed
                               to your callback*/
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_OpenDefaultStream(
        &captureStream, 1, /* no input channels */
        0,                 /* stereo output */
        paFloat32,         /* 32 bit floating point output */
        SAMPLE_RATE, 256,  /* frames per buffer, i.e. the number
                                  of sample frames that PortAudio will
                                  request from the callback. Many apps
                                  may want to use
                                  paFramesPerBufferUnspecified, which
                                  tells PortAudio to pick the best,
                                  possibly changing, buffer size.*/
        CaptureCallback,   /* this is your callback function */
        &data);            /*This is a pointer that will be passed
                              to your callback*/
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_StartStream(playbackStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_StartStream(captureStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    /* Sleep for several seconds. */
    Pa_Sleep(NUM_SECONDS * 1000);

    err = Pa_StopStream(playbackStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    err = Pa_StopStream(captureStream);
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    err = Pa_Terminate();
    if (err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

    return 0;
}