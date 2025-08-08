#ifndef REAPER_STT_UTILS_H
#define REAPER_STT_UTILS_H

#include "reaper_plugin.h"
#include <string>
#include <vector>

// Simple struct representing a word marker inserted into the project.
struct WordMarker {
  double position;       // Start time in seconds
  std::string word;      // Recognized word
  int marker_id;         // Marker ID as returned by AddProjectMarker
};

// Minimal local speech-to-text engine interface.  This class simply
// collects audio blocks and would be replaced by a real STT engine in a
// complete implementation.
class LocalSTTEngine {
public:
  // Feed a block of audio to the engine.  In a real implementation this
  // would perform recognition on the audio contained in the block.
  void Feed(const PCM_source_transfer_t* block);
};

// Class that manages speech transcription and synchronisation with the
// REAPER project via project markers.
class STTTranscriber {
public:
  explicit STTTranscriber(ReaProject* project);

  // Feeds an audio block to the STT engine.
  void ProcessBlock(PCM_source_transfer_t* block);

  // Adds a word marker at the specified position.
  void AddWord(const std::string& word, double position);

  // Returns a list of markers matching the target word.
  std::vector<WordMarker> Search(const std::string& target) const;

  // Replaces all occurrences of the target word with the replacement.
  void ReplaceWord(const std::string& target, const std::string& replacement);

  // Accessor for the full transcript.
  const std::string& transcript() const { return transcript_; }

private:
  ReaProject* project_;
  LocalSTTEngine stt_;
  std::vector<WordMarker> markers_;
  std::string transcript_;
};

#endif // REAPER_STT_UTILS_H

