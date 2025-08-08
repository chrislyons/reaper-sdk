#ifndef REAPER_STREAM_H_
#define REAPER_STREAM_H_

#include "reaper_plugin.h" // for PCM_source_transfer_t

#ifdef __cplusplus
extern "C" {
#endif

typedef void* REAPER_STREAM;

// Open a network stream using a ws:// or srt:// style URL.
// Returns a handle that can be used with stream_send/stream_receive, or NULL on failure.
REAPER_STREAM stream_open(const char* url);

// Transmit an audio block over the stream. The block should contain
// PCM data in block->samples and block->samples_out will be used as the
// number of sample pairs transmitted.
bool stream_send(REAPER_STREAM handle, PCM_source_transfer_t* block);

// Receive audio into the provided block. The block's fields should
// describe the desired format and buffer size. On success, samples_out
// is set to the number of sample pairs received.
bool stream_receive(REAPER_STREAM handle, PCM_source_transfer_t* block);

// Close an open stream handle. Safe to call with NULL.
void stream_close(REAPER_STREAM handle);

#ifdef __cplusplus
}
#endif

#endif // REAPER_STREAM_H_
