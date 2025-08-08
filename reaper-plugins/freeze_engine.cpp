#include <map>
#include <string>

#include "reaper_plugin.h"
#define REAPERAPI_IMPLEMENT
#include "../sdk/reaper_plugin_functions.h"

struct FreezeState {
    std::string chunk;
};

static std::map<MediaTrack*, FreezeState> g_freeze_states;

void FreezeTrack(MediaTrack* tr, int flags)
{
    if (!tr) return;
    char* chunk = GetSetObjectState(tr, "");
    if (chunk)
    {
        g_freeze_states[tr].chunk = chunk;
        FreeHeapPtr(chunk);
    }
    Undo_BeginBlock2(NULL);
    if (flags & FREEZE_FLAG_FXONLY)
    {
        int fxcount = TrackFX_GetCount(tr);
        for (int i = 0; i < fxcount; ++i)
        {
            TrackFX_SetOffline(tr, i, true);
        }
    }
    // placeholder: additional flag handling such as preserving sends could be implemented here
    Undo_EndBlock2(NULL, "Freeze Track", UNDO_STATE_TRACKCFG|UNDO_STATE_FX|UNDO_STATE_FREEZE);
}

void UnfreezeTrack(MediaTrack* tr)
{
    auto it = g_freeze_states.find(tr);
    if (it == g_freeze_states.end()) return;
    Undo_BeginBlock2(NULL);
    SetTrackStateChunk(tr, it->second.chunk.c_str(), false);
    g_freeze_states.erase(it);
    int fxcount = TrackFX_GetCount(tr);
    for (int i = 0; i < fxcount; ++i)
    {
        TrackFX_SetOffline(tr, i, false);
    }
    Undo_EndBlock2(NULL, "Unfreeze Track", UNDO_STATE_TRACKCFG|UNDO_STATE_FX|UNDO_STATE_FREEZE);
}

