const {EventEmitter} = require('events');
const addon = require('bindings')('addon.node');

class TabletEventReceiver extends EventEmitter {
  constructor(windowHandle) {
    super();
    this.windowHandle = windowHandle;
    addon.intercept(windowHandle, this.emit.bind(this));
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

exports.TabletEventReceiver = TabletEventReceiver;
