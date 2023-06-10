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

   private:
    snd_pcm_t* handle;
};
