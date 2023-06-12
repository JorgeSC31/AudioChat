#pragma once

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
    static int captureCallback();
    static int playbackCallback();
};
