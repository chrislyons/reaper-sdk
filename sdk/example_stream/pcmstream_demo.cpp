/*
  Simple REAPER extension demonstrating the reaper_stream API.
  On load, a one second 440Hz sine wave is transmitted to
  ws://127.0.0.1:9000 using stream_open/stream_send.
*/

#include "../reaper_plugin.h"
#include "../../WDL/wdltypes.h"
#include "reaper_plugin_functions.h"

#include <math.h>
#include <stdlib.h>

REAPER_PLUGIN_HINSTANCE g_hInst;

static void stream_test_tone()
{
  void* s = stream_open("ws://127.0.0.1:9000");
  if(!s) { ShowConsoleMsg("stream_open failed\n"); return; }

  PCM_source_transfer_t block = {};
  block.samplerate = 44100.0;
  block.nch = 2;
  block.length = 44100; // 1 second
  block.samples = (ReaSample*)malloc(block.length * block.nch * sizeof(ReaSample));
  block.samples_out = block.length;

  for(int i=0;i<block.length;i++)
  {
    double v = sin(2.0 * M_PI * 440.0 * (double)i / block.samplerate);
    block.samples[i*2] = (ReaSample)v;
    block.samples[i*2+1] = (ReaSample)v;
  }

  stream_send(s, &block);
  free(block.samples);
  ShowConsoleMsg("stream_send complete\n");
}

extern "C" {

REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t *rec)
{
  g_hInst = hInstance;
  if(rec)
  {
    if(rec->caller_version != REAPER_PLUGIN_VERSION || !rec->GetFunc || REAPERAPI_LoadAPI(rec->GetFunc))
      return 0;
    stream_test_tone();
    return 1;
  }
  return 0;
}

}
