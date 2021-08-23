#pragma once

#include <string>
#include <obs.h>
#include <util/profiler.h>

class Streamer
{
public:
   Streamer(std::string key);
   void Launch();
   void GetVideoSettings(obs_video_info& ovi);
   void GetAudioSettings(obs_audio_info& oai);
   bool InitializeOBS(obs_video_info& ovi, obs_audio_info& oai);
   void LoadModules();
   void CreateEncoders(obs_video_info& ovi, obs_audio_info& oai);
   void ShutdownOBS();
   bool ConnectStreamingService();
   bool Stream();

protected:

   obs_video_info ovi = {};
   obs_audio_info oai = {};
   obs_encoder_t* video_encoder = nullptr;
   obs_encoder_t* audio_encoder = nullptr;
   obs_output_t* stream_output = nullptr;
   obs_service_t* stream_service = nullptr;

   std::string twitchKey;
};

