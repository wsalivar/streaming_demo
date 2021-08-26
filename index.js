const addon = require('D:/Projects/streaming_demo/build/Debug/streaming_demo.node');
const twitchKey = "test key;

console.log('Running index.js');
const demoInstance = new addon.demo_nnm(twitchKey);
console.log('launch ', demoInstance.launch());

module.exports = addon;
