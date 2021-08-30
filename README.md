## Simple native node module to stream the desktop to Twitch using OBS Studio API

This is my first attempt at writing a napi module in C++ using OBS Software to stream content.  I learned some useful information along the way regarding the importance of modules.

## Modules

Generally, modules make up the feature set of OBS.  They are used for filters, encoders, decoders, source and output types.  As a rule, in most modules have a dependency on some form of data.  Thus, when loading modules, you have to specify the path to the data as well.  

You can load modules in bulk using _obs_load_all_modules()_ method but have to be careful to specify paths to modules and their data.  Alternatively, you can load only modules that you need with _obs_open_module()_ followed by _obs_init_module()_.  For example,

```C++
   obs_module_t* module = nullptr;
   auto path = std::string(PluginPath) + "rtmp-services";
   auto data_path = std::string(DataPath) + "rtmp-services";

   if (obs_open_module(&module, path.c_str(), data_path.c_str()) == MODULE_SUCCESS)
   {
      auto init_result = obs_init_module(module);

      return init_result;
   }
```

Providing complete paths to any modules in use will reduce headaches with missing dependencies.

## To Run

From a cmd shell on Windows, run node-gyp to generate all the build files including visual studio project and solution.

```
node-gyp configure --debug rebuild
```

From here, I typically load up the solution file and build in the IDE.  To run and debug from the IDE, change your debugger settings to launch the index.js file via node.

![Debug](https://github.com/wsalivar/streaming_demo/blob/master/Debug.PNG)
