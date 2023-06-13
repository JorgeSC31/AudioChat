#pragma once
#include <config.h>
#include <portaudio.h>
#include <stdlib.h>
#include <unp.h>

class DataBuffer {
   public:
    DataBuffer();
    void initialize(short port);
    void push(const void *buffer);
    void *pop();

    unsigned int bufferSize;
    unsigned int slots;
    unsigned int readSlot;
    unsigned int writeSlot;
    void *rawBuffer;
    int socketFD;
    sockaddr_in sockAddr;
};

class Audio {
   public:
    Audio();
    void initialize();

    void openCaptureStream(PaSampleFormat format, unsigned int rate,
                           unsigned long frames, short port);
    void openPlaybackStream(PaSampleFormat format, unsigned int rate,
                            unsigned long frames);

    void startCaptureStream();
    void startPlaybackStream();

    void stopCaptureStream();
    void stopPlaybackStream();

    int getCaptureFD();

    DataBuffer playbackBuffer;
    DataBuffer captureBuffer;

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
