const addon = require('bindings')('intercept_tablet_event.node')

addon.intercept(new Buffer("hogehoge"))
