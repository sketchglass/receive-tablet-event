# receive-tablet-event

Receive native digitizer (tablet) event in Node.js (mainly for Electron)

## Supported platform

- [x] Mac
- [ ] Windows
- [ ] Linux (X11)

## Usage

```
npm install --save seanchas116/receive-tablet-event
npm install --save-dev electron-rebuild
$(npm bin)/electron-rebuild -f
```

```js
// receive-tablet-event only supports main process
const {TabletEventReceiver} = require("receive-tablet-event");

const win = new BrowserWindow({width: 800, height: 600});

const receiver = new TabletEventReceiver(win.getNativeWindowHandle());

receiver.on("enterProximity", (ev) => {
  console.log("pen incoming");
  console.log(ev.pointerId);
});
receiver.on("leaveProximity", (ev) => {
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
receiver.on("up", (ev) => {
  console.log("end drawing");
  console.log(ev.pointerId, ev.clientX, ev.clientY, ev.pressure);
});
```
