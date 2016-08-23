# receive-tablet-event

Receive native digitizer (tablet) event in Electron

## Supported platforms

- [x] Mac
- [x] Windows
- [ ] Linux (X11)

## Usage

```
npm install --save seanchas116/receive-tablet-event

# Don't forget to rebuild native module against Electron
npm install --save-dev electron-rebuild
$(npm bin)/electron-rebuild -f
```

```js
// receive-tablet-event only works in main process
const {TabletEventReceiver} = require("receive-tablet-event");

const win = new BrowserWindow({width: 800, height: 600});
const receiver = new TabletEventReceiver(win);

receiver.captureArea = {
  left: 0, top: 0, width: 800, height: 600
};

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

## Run example

```
cd example
npm install
npm start
```
