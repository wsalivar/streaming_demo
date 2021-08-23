#include <util/platform.h>
#include <iostream>
#include <windows.h>
#include "Streamer.h"
#include "napi.h"


constexpr char ModulePath [] = {"D:/Projects/obs-studio-master/obs-studio/libobs/data/"};
constexpr char DataPath [] = {"D:/Projects/obs-studio-master/obs-studio/libobs/data"};
constexpr char ObsBasePath [] = {"D:/Projects/obs-studio-master/obs-studio/build\rundir/Debug"};
constexpr char GraphicsModule [] = {"D:/Projects/demo_NNM/build/Debug/libobs-opengl.dll"};
constexpr char PluginPath [] = {"D:/Projects/obs-studio-master/obs-studio/build/rundir/Debug/obs-plugins/64bit"};


Streamer::Streamer(std::string key)
{
   twitchKey = key;
}

void Streamer::Launch()
{
   if (InitializeOBS(ovi, oai))
   {
      LoadModules();

      CreateEncoders(ovi, oai);

      ConnectStreamingService();

      Stream();

      ShutdownOBS();
   }
}

void Streamer::GetVideoSettings(obs_video_info& ovi)
{
   // Assume we're streaming the whole display
   ovi.adapter = 0;
   ovi.fps_num = 30000;
   ovi.fps_den = 1001;
   ovi.graphics_module = GraphicsModule;
   ovi.output_format = VIDEO_FORMAT_NV12;
   ovi.base_width = GetSystemMetrics(SM_CXSCREEN);
   ovi.base_height = GetSystemMetrics(SM_CYSCREEN);
   ovi.output_width = 1920;
   ovi.output_height = 1080;
   ovi.range = VIDEO_RANGE_PARTIAL;
   ovi.scale_type = OBS_SCALE_BICUBIC;
}

void Streamer::GetAudioSettings(obs_audio_info& oai)
{
   oai.samples_per_sec = 48000;
   oai.speakers = SPEAKERS_STEREO;
}

bool Streamer::InitializeOBS(obs_video_info& ovi, obs_audio_info& oai)
{
   if (obs_startup("en-US", ObsBasePath, nullptr))
   {
      GetVideoSettings(ovi);
      GetAudioSettings(oai);

      obs_add_module_path(ModulePath, ModulePath);
      obs_add_module_path(DataPath, DataPath);
      obs_add_module_path(PluginPath, PluginPath);
      obs_add_data_path(ModulePath);
      obs_add_data_path(PluginPath);

      return obs_reset_audio(&oai) && OBS_VIDEO_SUCCESS == obs_reset_video(&ovi);
   }

   return false;
}

void Streamer::LoadModules()
{
   obs_load_all_modules();
   obs_post_load_modules();
}

void Streamer::CreateEncoders(obs_video_info& ovi, obs_audio_info& oai)
{
      video_encoder = obs_video_encoder_create("obs_x264", "x264", nullptr, nullptr);
      audio_encoder = obs_audio_encoder_create("ffmpeg_aac", "aac", nullptr, 0, nullptr);

      obs_encoder_set_video(video_encoder, obs_get_video());
      obs_encoder_set_audio(audio_encoder, obs_get_audio());
}

void Streamer::ShutdownOBS()
{
   obs_shutdown();
}

bool Streamer::ConnectStreamingService()
{
   stream_service = obs_service_create("rtmp_common", NULL, NULL, nullptr);
   obs_service_release(stream_service);

   return false;
}

bool Streamer::Stream()
{
   if (!obs_output_active(stream_output))
   {
      return true;
   }

   return false;
}
