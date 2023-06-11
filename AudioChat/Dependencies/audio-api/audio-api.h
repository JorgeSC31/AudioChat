#pragma once

#include <alsa/asoundlib.h>
class PlaybackDevice {
   public:
    PlaybackDevice();
    void open(const char* device = "default");
    void setParams(snd_pcm_format_t format, unsigned int rate,
                   unsigned int latency);
    void play(void* buffer, unsigned int buffLen);
    void close();
    int getFD();

   private:
    snd_pcm_t* handle;
};

class CaptureDevice {
   public:
    CaptureDevice();
    void open(const char* device = "default");
    void setParams(snd_pcm_format_t format, unsigned int rate,
                   unsigned int latency);
    void capture(void* buffer, unsigned int buffLen);
    void close();
    int getFD();

   private:
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* hw_params;
};
