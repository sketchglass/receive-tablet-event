const addon = require('bindings')('tablet_event_interceptor.node')

addon.intercept(new Buffer("hogehoge"))
