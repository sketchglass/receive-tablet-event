"use strict";

const {EventEmitter} = require('events');
const addon = require('bindings')('addon.node');

const receivers = new Map();

class TabletEventReceiver extends EventEmitter {
  static install(browserWindow) {
    if (receivers.has(browserWindow)) {
      return receivers.get(browserWindow);
    } else {
      const receiver = new TabletEventReceiver(browserWindow);
      receivers.set(browserWindow, receiver);
      browserWindow.on('closed', () => {
        receivers.delete(browserWindow);
        receiver.dispose();
      });
      return receiver;
    }
  }

  constructor(browserWindow) {
    super();
    this.captureArea = {left: 0, top: 0, width: 0, height: 0};
    this.capturedPointers = new Set();
    this.windowHandle = browserWindow.getNativeWindowHandle();
    addon.intercept(this.windowHandle, this.handleEvent.bind(this));
  }

  insideCaptureArea(clientX, clientY) {
    const {left, top, width, height} = this.captureArea;
    return left <= clientX && clientX < left + width && top <= clientY && clientY < top + height;
  }

  handleEvent(type, clientX, clientY, pressure, pointerType, pointerId) {
    console.log("handling events");
    switch (type) {
      case "down":
        if (this.insideCaptureArea(clientX, clientY)) {
          this.capturedPointers.add(pointerId);
        } else {
          return false;
        }
        break;
      case "up":
        if (!this.capturedPointers.has(pointerId)) {
          return false;
        }
        this.capturedPointers.delete(pointerId);
        break;
      case "move":
        if (!this.capturedPointers.has(pointerId)) {
          return false;
        }
        break;
    }
    this.emit(type, {clientX, clientY, pressure, pointerType, pointerId})
    return true;
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

module.exports = function receiveTabletEvent(browserWindow) {
  return TabletEventReceiver.install(browserWindow);
};
