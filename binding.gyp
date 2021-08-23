{
    "targets": [{
        "target_name": "streaming_demo",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "cppsrc/main.cpp",
            "cppsrc/demo_nnm.cpp",
            "cppsrc/Streamer.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "D:\\Projects\\obs-studio-master\\obs-studio\\libobs"
        ],
        'libraries': [
            "D:\\Projects\\obs-studio-master\\obs-studio\\build\\libobs\\Debug\\obs.lib",
	],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}