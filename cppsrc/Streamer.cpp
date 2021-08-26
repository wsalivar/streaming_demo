#include <util/platform.h>
#include <windows.h>
#include "Streamer.h"
#include "napi.h"

#include <iostream>
using namespace std;

constexpr char ModulePath [] = {"D:/Projects/obs-studio-master/obs-studio/libobs/data/"};
constexpr char DataPath [] = {"D:/Projects/obs-studio-master/obs-studio/libobs/data/"};
constexpr char ObsBasePath [] = {"D:/Projects/obs-studio-master/obs-studio/build/rundir/Debug"};
constexpr char GraphicsModule [] = {"D:/Projects/demo_NNM/build/Debug/libobs-opengl.dll"};
constexpr char PluginPath [] = {"D:/Projects/obs-studio-master/obs-studio/build/rundir/Debug/obs-plugins/64bit/"};

Streamer::Streamer(std::string key)
{
   twitchKey = key;
}

void Streamer::Launch()
{
   if (InitializeOBS())
   {
      if (SetupAudioVideo())
      {
         if (ConnectTwitchService() && CreateStreamOutput())
         {
            StreamStart();
         }
      }


      StreamStop();

      ShutdownOBS();
   }
}

void Streamer::GetVideoSettings()
{
   // Assume we're streaming the whole display
   ovi.adapter = 0;
   ovi.fps_num = 30;
   ovi.fps_den = 1;
   ovi.graphics_module = GraphicsModule;
   ovi.output_format = VIDEO_FORMAT_I420;
   ovi.base_width = GetSystemMetrics(SM_CXSCREEN);
   ovi.base_height = GetSystemMetrics(SM_CYSCREEN);
   ovi.output_width = 1920;
   ovi.output_height = 1080;
   ovi.gpu_conversion = true;
   ovi.colorspace = VIDEO_CS_DEFAULT;
   ovi.range = VIDEO_RANGE_DEFAULT;
   ovi.scale_type = OBS_SCALE_BICUBIC;
}

void Streamer::GetAudioSettings()
{
   oai.samples_per_sec = 48000;
   oai.speakers = SPEAKERS_STEREO;
}

bool Streamer::InitializeOBS()
{
   wchar_t* dll_path = L"D://Projects//obs-studio-master//obs-studio//build//rundir//Debug//bin//64bit";
   auto result = SetDllDirectoryW(dll_path);

   if (obs_startup("en-US", ObsBasePath, nullptr))
   {
      GetVideoSettings();
      GetAudioSettings();

      //obs_add_module_path(ModulePath, DataPath);
      //obs_add_module_path(DataPath, DataPath);
      obs_add_module_path(PluginPath, DataPath);
      obs_add_module_path(ObsBasePath, nullptr);
      obs_add_data_path(DataPath);
      //obs_add_data_path(PluginPath);

      return LoadModules();
   }

   return false;
}

bool Streamer::LoadModules()
{
   obs_module_t* rtmp_module = nullptr;
   obs_module_t* ffmpeg_module = nullptr;
   obs_module_t* outputs_module = nullptr;

   auto rtmp_path = std::string(PluginPath) + "rtmp-services";
   auto outputs_path = std::string(PluginPath) + "obs-outputs";
   auto ffmpeg_path = std::string("D:/Projects/obs-studio-master/obs-studio/build/rundir/Debug/bin/64bit/obs-ffmpeg");

   return OpenAndInitModule(rtmp_module, rtmp_path) &&
      OpenAndInitModule(ffmpeg_module, outputs_path) &&
      OpenAndInitModule(outputs_module, ffmpeg_path);

   //obs_load_all_modules();
   //obs_post_load_modules();
}

bool Streamer::OpenAndInitModule(obs_module_t*& module, const std::string path)
{
   if (obs_open_module(&module, path.c_str(), nullptr) == MODULE_SUCCESS)
   {
      auto init_result = obs_init_module(module);

      return init_result;
   }

   return false;
}

bool Streamer::SetupAudioVideo()
{
   TCHAR buffer [MAX_PATH] = {0};
   GetModuleFileName(NULL, buffer, MAX_PATH);

   if (obs_reset_audio(&oai) && OBS_VIDEO_SUCCESS == obs_reset_video(&ovi))
   {
      if (CreateSource())
      {
         obs_module_t* obs_x264 = nullptr;
         auto obs_x264_path = std::string(PluginPath) + "obs-x264";

         if (OpenAndInitModule(obs_x264, obs_x264_path))
         {
            videoEncoder = obs_video_encoder_create("obs_x264", "", nullptr, nullptr);
            obs_encoder_set_video(videoEncoder, obs_get_video());

            audioEncoder = obs_audio_encoder_create("ffmpeg_aac", "aac", nullptr, 0, nullptr);
            obs_encoder_set_audio(audioEncoder, obs_get_audio());

            return true;
         }
      }
   }

   return false;
}

bool Streamer::CreateSource()
{
   obs_module_t* win_capture = nullptr;
   obs_module_t* win_wasapi = nullptr;

   auto win_capture_path = std::string(PluginPath) + "win-capture";
   auto win_wasapi_path = std::string(PluginPath) + "win-wasapi";

   if (OpenAndInitModule(win_capture, win_capture_path) &&
      OpenAndInitModule(win_wasapi, win_wasapi_path))
   {
      videoSource = obs_source_create("monitor_capture", "", nullptr, nullptr);
      audioSource = obs_source_create("wasapi_input_capture", "", nullptr, nullptr);

      if (videoSource != nullptr && audioSource != nullptr)
      {
         obs_set_output_source(0, videoSource);
         obs_set_output_source(1, audioSource);

         return true;
      }
   }

   return false;
}

void Streamer::ShutdownOBS()
{
   obs_service_release(streamService);

   obs_shutdown();
}

bool Streamer::ConnectTwitchService()
{
   bool result = false;

   auto data = obs_data_create();
   obs_data_set_string(data, "service", "Twitch");
   obs_data_set_string(data, "key", twitchKey.c_str());
   obs_data_set_string(data, "server", "auto");

   result = (streamService = obs_service_create("rtmp_common", "Twitch", data, nullptr)) != nullptr;

   obs_data_release(data);

   return result;
}

bool Streamer::CreateStreamOutput()
{
   if ((streamOutput = obs_output_create("rtmp_output", "", nullptr, nullptr)) != nullptr)
   {
      obs_output_set_video_encoder(streamOutput, videoEncoder);
      obs_output_set_audio_encoder(streamOutput, audioEncoder, 0);
      obs_output_set_service(streamOutput, streamService);

      return true;
   }

   return false;
}

bool Streamer::StreamStart()
{
   if (obs_output_active(streamOutput))
   {
      obs_output_stop(streamOutput);
   }

   return obs_output_start(streamOutput);
}

void Streamer::StreamStop()
{
   if (obs_output_active(streamOutput))
   {
      obs_output_stop(streamOutput);
   }
}

