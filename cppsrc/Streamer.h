#pragma once

#include <string>
#include <obs.h>
#include <util/profiler.h>

static const char* ftl_stream_getname([[maybe_unused]] void* unused)
{
   return "FTLStream";
}

static int CountVideoSources()
{
   int count = 0;
   auto countSources = [](void* param, obs_source_t* source) 
   {
      if (!source)
         return true;

      uint32_t flags = obs_source_get_output_flags(source);
      if ((flags & OBS_SOURCE_VIDEO) != 0)
         (*reinterpret_cast<int*>(param))++;

      return true;
   };

   obs_enum_sources(countSources, &count);
   return count;
}

class Streamer
{
public:
   Streamer(std::string key);
   void Launch();
   void GetVideoSettings();
   void GetAudioSettings();
   bool InitializeOBS();
   void LoadModules();
   bool SetupAudioVideo();
   bool CreateSource();
   void ShutdownOBS();
   bool ConnectTwitchService();
   bool CreateStreamOutput();
   bool StreamStart();
   void StreamStop();

protected:

   obs_video_info ovi = {};
   obs_audio_info oai = {};
   obs_service_info twitchInfo = {};
   obs_source_info sourceInfo = {};

   // These should all be unique_ptr types but the
   // compiler was not cooperating for some reason
   obs_encoder_t* videoEncoder = nullptr;
   obs_encoder_t* audioEncoder = nullptr;
   obs_output_t* streamOutput = nullptr;
   obs_service_t* streamService = nullptr;
   obs_source_t* videoSource = nullptr;
   obs_source_t* audioSource = nullptr;

   std::string twitchKey;
};

