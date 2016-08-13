const {EventEmitter} = require('events');
const addon = require('bindings')('addon.node');

class TabletEventReceiver extends EventEmitter {
  constructor(browserWindow) {
    super();
    this.windowHandle = browserWindow.getNativeWindowHandle();
    addon.intercept(this.windowHandle, this.emit.bind(this));
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

exports.TabletEventReceiver = TabletEventReceiver;
