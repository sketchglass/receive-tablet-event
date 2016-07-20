const {EventEmitter} = require('events');
const addon = require('bindings')('intercept_tablet_event.node');

class TabletEventReceiver extends EventEmitter {
  constructor(window: Buffer) {
    this.window = window;
    addon.intercept(window, (event) => {
      this.emit("event", event);
    });
  }
}

exports.TabletEventReceiver = TabletEventReceiver;
