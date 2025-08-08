#include "stt_utils.h"

#include <algorithm>

// REAPER API function pointers that will be provided by the host at runtime.
// Only the subset required for this utility is declared here.
extern int (*AddProjectMarker)(ReaProject* proj, bool isrgn, double pos,
                               double rgnend, const char* name, int wantidx);
extern bool (*SetProjectMarkerByIndex2)(ReaProject* proj, int markrgnidx,
                                        bool isrgn, double pos, double rgnend,
                                        int IDnumber, const char* name,
                                        int color, int flags);

// ----------------------------------------------------------------------------
// LocalSTTEngine
// ----------------------------------------------------------------------------

void LocalSTTEngine::Feed(const PCM_source_transfer_t* block)
{
  // In a production system, this function would pass the audio contained in
  // |block| to a speech-to-text engine.  The mini-SDK does not include such an
  // engine, so the implementation is intentionally left empty.
  (void)block; // suppress unused parameter warning
}

// ----------------------------------------------------------------------------
// STTTranscriber
// ----------------------------------------------------------------------------

STTTranscriber::STTTranscriber(ReaProject* project)
  : project_(project)
{
}

void STTTranscriber::ProcessBlock(PCM_source_transfer_t* block)
{
  stt_.Feed(block);
}

void STTTranscriber::AddWord(const std::string& word, double position)
{
  int id = AddProjectMarker(project_, false, position, position, word.c_str(), -1);
  markers_.push_back({position, word, id});
  if (!transcript_.empty()) transcript_ += ' ';
  transcript_ += word;
}

std::vector<WordMarker> STTTranscriber::Search(const std::string& target) const
{
  std::vector<WordMarker> results;
  for (const auto& m : markers_) {
    if (m.word == target) {
      results.push_back(m);
    }
  }
  return results;
}

void STTTranscriber::ReplaceWord(const std::string& target,
                                 const std::string& replacement)
{
  for (auto& m : markers_) {
    if (m.word == target) {
      m.word = replacement;
      // Update the marker's name inside the project.
      SetProjectMarkerByIndex2(project_, m.marker_id, false, m.position,
                               m.position, m.marker_id, replacement.c_str(), 0,
                               0);
    }
  }

  // Rebuild transcript from markers.
  transcript_.clear();
  for (const auto& m : markers_) {
    if (!transcript_.empty()) transcript_ += ' ';
    transcript_ += m.word;
  }
}

