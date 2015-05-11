# WDT Native

[![Build Status](https://travis-ci.org/dasantonym/node-wdt-native.svg)](https://travis-ci.org/dasantonym/node-wdt-native) [![Dependency Status](https://david-dm.org/dasantonym/node-wdt-native.svg)](https://david-dm.org/dasantonym/node-wdt-native)

Native functionality for the [nw.js](http://nwjs.io/) version of the [Whatever Dance Toolbox](https://github.com/dasantonym/wdt).

The module uses [node-opencv](https://github.com/peterbraden/node-opencv) for OpenCV functionality. Build system is [cmake-js](https://github.com/unbornchikken/cmake-js).

## Install

The module requires cmake and the opencv library and headers.

To build for a specific platform other than your current node/iojs, add this to your package.json (see cmake-js docs for more info):

```shell
"cmake-js": {
  "runtime": "nw",
  "runtimeVersion": "0.12.1",
  "arch": "x64"
}
```

To build and install the module

```shell
npm install wdt-native
```

## Credits

All original image analysis code is taken from [Daniel Turing](https://github.com/dturing)'s [ppgst](https://github.com/dturing/ppgst) project. Adapted as Node module by Anton Koch.
