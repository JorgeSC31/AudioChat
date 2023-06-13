#pragma once

#define SERVER_PORT (7500)
#define RATE (8'000)
#define FORMAT float
#define FRAMES_PER_SECOND (256)
#define BUFFER_SIZE (FRAMES_PER_SECOND * sizeof(FORMAT))