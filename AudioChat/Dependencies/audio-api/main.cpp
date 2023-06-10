#include <bits/stdc++.h>

#include "audio-api.h"
using namespace std;

int main() {
    PlaybackDevice playback;
    playback.open();
    int rate = 8'000;
    int latency = 200'000;  // 0.2 seconds
    int buff_size = rate * latency / 1e6;
    playback.setParams(SND_PCM_FORMAT_FLOAT_LE, rate, latency);
    printf("buffer  size: %d\n", buff_size);

    playback.close();
    return 0;
}