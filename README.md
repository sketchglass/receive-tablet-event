# intercept-tablet-event

Intercept native tablet (digitizer) event in Electron

## Features

* Receive tablet proximity / up / down / move events for Electron windows from JavaScript

## Supported platform

* Currently Mac only

## Usage

```
npm install --save seanchas116/intercept-tablet-event
$(npm bin)/electron-rebuild
```

```js
const {TabletEventReceiver} = require("intercept-tablet-event");

const win = new BrowserWindow({width: 800, height: 600});

const receiver = new TabletEventReceiver(win.getNativeWindowHandle());

receiver.on("enterProximity", (ev) => {
  console.log("pen incoming");
  console.log(ev.pointerId);
});
receiver.on("enterProximity", (ev) => {
  console.log("pen outgoing");
  console.log(ev.pointerId);

});
receiver.on("down", (ev) => {
  console.log("start drawing");
  console.log(ev.pointerId, ev.clientX, ev.clientY, ev.pressure);
});
receiver.on("move", (ev) => {
  console.log(ev.pointerId, ev.clientX, ev.clientY, ev.pressure);
});
receiver.on("down", (ev) => {
  console.log("end drawing");
  console.log(ev.pointerId, ev.clientX, ev.clientY, ev.pressure);
});
```
