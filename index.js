const addon = require('D:/Projects/streaming_demo/build/Debug/streaming_demo.node');
const twitchKey = "live_160175550_F99OqzWDza3Blac90r4LZAvN6gi9Nq";

console.log('Running index.js');
const demoInstance = new addon.demo_nnm(twitchKey);
console.log('launch ', demoInstance.launch());

module.exports = addon;
