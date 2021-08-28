#pragma once

#include <napi.h>
#include "Streamer.h"

class demo_nnm : public Napi::ObjectWrap<demo_nnm>
{
   public:
      static Napi::Object Init(Napi::Env env, Napi::Object exports);
      demo_nnm(const Napi::CallbackInfo& info);
      ~demo_nnm();

      Streamer* getStreamer();

   private:
      static Napi::FunctionReference constructor;
      void Launch(const Napi::CallbackInfo& info);
      void demo_nnm::StartStream(const Napi::CallbackInfo& info);
      void demo_nnm::StopStream(const Napi::CallbackInfo& info);
      Streamer* streamer = nullptr;

};

