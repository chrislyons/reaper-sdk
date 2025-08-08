#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#define REAPERAPI_MINIMAL
#define REAPERAPI_WANT_AddMediaItemToTrack
#define REAPERAPI_WANT_AddTakeToMediaItem
#define REAPERAPI_WANT_CreateTrackAudioAccessor
#define REAPERAPI_WANT_DestroyAudioAccessor
#define REAPERAPI_WANT_GetAudioAccessorStartTime
#define REAPERAPI_WANT_GetAudioAccessorEndTime
#define REAPERAPI_WANT_GetAudioAccessorSamples
#define REAPERAPI_WANT_GetMediaTrackInfo_Value
#define REAPERAPI_WANT_GetSetObjectState
#define REAPERAPI_WANT_FreeHeapPtr
#define REAPERAPI_WANT_GetSetProjectInfo
#define REAPERAPI_WANT_GetTrackNumMediaItems
#define REAPERAPI_WANT_GetTrackMediaItem
#define REAPERAPI_WANT_DeleteTrackMediaItem
#define REAPERAPI_WANT_PCM_Source_CreateFromFile
#define REAPERAPI_WANT_SetMediaItemTake_Source
#define REAPERAPI_WANT_SetMediaItemInfo_Value
#define REAPERAPI_WANT_TrackFX_GetCount
#define REAPERAPI_WANT_TrackFX_GetOffline
#define REAPERAPI_WANT_TrackFX_SetOffline
#define REAPERAPI_WANT_Undo_BeginBlock2
#define REAPERAPI_WANT_Undo_EndBlock2
#define REAPERAPI_WANT_UpdateArrange
#define REAPERAPI_WANT_SetTrackStateChunk

#include "reaper_plugin.h"
#undef min
#undef max
#include "../WDL/wdltypes.h"
#include "../sdk/reaper_plugin_functions.h"
#include "../WDL/wavwrite.h"

struct FreezeState {
    std::string chunk;
    std::string media;
    std::vector<bool> fx_offline;
};

static std::map<MediaTrack*, FreezeState> g_freeze_states;

static std::string make_temp_wav()
{
    char buf[L_tmpnam];
    std::tmpnam(buf);
    std::string fn = buf;
    fn += ".wav";
    return fn;
}

void FreezeTrack(MediaTrack* tr, int flags)
{
    if (!tr) return;

    FreezeState st;
    if (char* chunk = GetSetObjectState(tr, ""))
    {
        st.chunk = chunk;
        FreeHeapPtr(chunk);
    }

    Undo_BeginBlock2(NULL);

    AudioAccessor* aa = CreateTrackAudioAccessor(tr);
    double start = GetAudioAccessorStartTime(aa);
    double end = GetAudioAccessorEndTime(aa);
    int nch = (int)GetMediaTrackInfo_Value(tr, "I_NCHAN");
    if (nch < 1) nch = 1;
    double sr = GetSetProjectInfo(NULL, "PROJECT_SRATE", 0.0, false);
    if (sr <= 0.0) sr = 44100.0;

    st.media = make_temp_wav();
    WaveWriter ww(st.media.c_str(), 24, nch, (int)sr, 0);

    const int block = 1024;
    std::vector<double> buf(block * nch);
    for (double pos = start; pos < end; )
    {
        int ns = std::min(block, (int)((end - pos) * sr));
        if (GetAudioAccessorSamples(aa, sr, nch, pos, ns, buf.data()) > 0)
            ww.WriteDoubles(buf.data(), ns * nch);
        pos += (double)ns / sr;
    }
    DestroyAudioAccessor(aa);

    int itemcount = GetTrackNumMediaItems(tr);
    for (int i = itemcount - 1; i >= 0; --i)
        DeleteTrackMediaItem(tr, GetTrackMediaItem(tr, i));

    MediaItem* item = AddMediaItemToTrack(tr);
    MediaItem_Take* take = AddTakeToMediaItem(item);
    PCM_source* src = PCM_Source_CreateFromFile(st.media.c_str());
    SetMediaItemTake_Source(take, src);
    SetMediaItemInfo_Value(item, "D_POSITION", start);
    SetMediaItemInfo_Value(item, "D_LENGTH", end - start);

    int fxcount = TrackFX_GetCount(tr);
    st.fx_offline.resize(fxcount);
    for (int i = 0; i < fxcount; ++i)
    {
        st.fx_offline[i] = TrackFX_GetOffline(tr, i);
        TrackFX_SetOffline(tr, i, true);
    }

    g_freeze_states[tr] = st;
    UpdateArrange();
    Undo_EndBlock2(NULL, "Freeze Track", UNDO_STATE_TRACKCFG|UNDO_STATE_FX|UNDO_STATE_FREEZE);
}

void UnfreezeTrack(MediaTrack* tr)
{
    auto it = g_freeze_states.find(tr);
    if (it == g_freeze_states.end()) return;

    Undo_BeginBlock2(NULL);
    SetTrackStateChunk(tr, it->second.chunk.c_str(), false);
    int fxcount = TrackFX_GetCount(tr);
    for (int i = 0; i < fxcount && i < (int)it->second.fx_offline.size(); ++i)
        TrackFX_SetOffline(tr, i, it->second.fx_offline[i]);
    if (!it->second.media.empty())
        std::remove(it->second.media.c_str());
    g_freeze_states.erase(it);
    UpdateArrange();
    Undo_EndBlock2(NULL, "Unfreeze Track", UNDO_STATE_TRACKCFG|UNDO_STATE_FX|UNDO_STATE_FREEZE);
}

