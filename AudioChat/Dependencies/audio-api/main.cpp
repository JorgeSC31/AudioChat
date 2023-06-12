#include <stdio.h>
#include <string.h>

#include "audio-api.h"

#define SAMPLE_RATE (8000)
#define BUFFER_SIZE (256)

int main() {
    Audio audio;
    audio.initialize();

    audio.openCaptureStream(paFloat32, SAMPLE_RATE, BUFFER_SIZE);
    audio.openPlaybackStream(paFloat32, SAMPLE_RATE, BUFFER_SIZE);

    audio.startCaptureStream();
    Pa_Sleep(5 * 1000);
    audio.stopCaptureStream();

    audio.startPlaybackStream();
    Pa_Sleep(4 * 1000);
    audio.stopPlaybackStream();

    Pa_Terminate();
    return 0;
}