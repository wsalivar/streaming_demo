#pragma once

#include <string>
#include <obs.h>
#include <util/profiler.h>

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

