#include "demo_nnm.h"

Napi::FunctionReference demo_nnm::constructor;

Napi::Object demo_nnm::Init(Napi::Env env, Napi::Object exports)
{
   Napi::HandleScope scope(env);

   Napi::Function func = DefineClass(env, "demo_nnm", 
   {
     InstanceMethod("launch", &demo_nnm::Launch),
   });

   constructor = Napi::Persistent(func);
   constructor.SuppressDestruct();

   exports.Set("demo_nnm", func);
   return exports;
}

demo_nnm::demo_nnm(const Napi::CallbackInfo& info) : Napi::ObjectWrap<demo_nnm>(info)
{
   Napi::Env env = info.Env();
   Napi::HandleScope scope(env);

   int length = info.Length();

   if (length != 1) {
      Napi::TypeError::New(env, "Twitch key expected").ThrowAsJavaScriptException();
   }

   auto twitchKey = info [0].As<Napi::String>();

   streamer = new Streamer(twitchKey);
}

demo_nnm::~demo_nnm()
{
   if (streamer != nullptr)
   {
      delete streamer;
   }
}

void demo_nnm::Launch(const Napi::CallbackInfo& info)
{
   Napi::Env env = info.Env();
   Napi::HandleScope scope(env);

   if (streamer != nullptr)
   {
      streamer->Launch();
   }
}

Streamer* demo_nnm::getStreamer()
{
   return streamer;
}

