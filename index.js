"use strict";

const {EventEmitter} = require('events');
const addon = require('bindings')('addon.node');

class TabletEventReceiver extends EventEmitter {
  constructor(browserWindow) {
    super();
    this.captureArea = {left: 0, top: 0, width: 0, height: 0};
    this.capturedPointers = new Set();
    this.windowHandle = browserWindow.getNativeWindowHandle();
    addon.intercept(this.windowHandle, this.handleEvent.bind(this));
    addon.onReload(this.windowHandle);
    browserWindow.webContents.on("dom-ready", () => {
      addon.onReload(this.windowHandle);
    });
  }

  insideCaptureArea(clientX, clientY) {
    const {left, top, width, height} = this.captureArea;
    return left <= clientX && clientX < left + width && top <= clientY && clientY < top + height;
  }

  handleEvent(type, button, clientX, clientY, pressure, pointerType, pointerId) {
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
    this.emit(type, {button, clientX, clientY, pressure, pointerType, pointerId})
    // do not prevent original event handler outside Windows
    return process.platform == "win32";
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

exports.TabletEventReceiver = TabletEventReceiver;
