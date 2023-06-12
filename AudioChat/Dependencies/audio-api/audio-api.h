#pragma once
#include <portaudio.h>

class Audio {
   public:
    Audio();
    void initialize();

    void openCaptureStream();
    void openPlaybackStream();

    void startCaptureStream();
    void startPlaybackStream();

    void stopCaptureStream();
    void stopPlaybackStream();

   private:
    PaStream *playbackStream;
    PaStream *captureStream;

    static int captureCallback(const void *input, void *output,
                               unsigned long frameCount,
                               const PaStreamCallbackTimeInfo *timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData);
    static int playbackCallback(const void *input, void *output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData);

    PaError error;
    void catchError(PaError);
};
