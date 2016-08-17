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
      return receiver;
    }
  }

  constructor(browserWindow) {
    super();
    this.windowHandle = browserWindow.getNativeWindowHandle();
    addon.intercept(this.windowHandle, this.emit.bind(this));
    browserWindow.on('closed', () => {
      this.dispose();
    });
  }

  dispose() {
    addon.unintercept(this.windowHandle);
  }
}

exports.TabletEventReceiver = TabletEventReceiver;

// workaround for https://github.com/electron/electron/issues/6863
exports.installTabletEventReceiver = function installTabletEventReceiver(browserWindow) {
  return TabletEventReceiver.install(browserWindow);
}
