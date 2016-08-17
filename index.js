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
    this.windowHandle = browserWindow.getNativeWindowHandle();
    addon.intercept(this.windowHandle, (type, clientX, clientY, pressure, pointerType, pointerId) => {
      let handled = false;
      const preventDefault = () => {
        handled = true;
      };
      this.emit(type, {clientX, clientY, pressure, pointerType, pointerId, preventDefault})
      return handled;
    });
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

module.exports = function receiveTabletEvent(browserWindow) {
  return TabletEventReceiver.install(browserWindow);
};
