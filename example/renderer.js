"use strict";

const {remote} = require("electron");
const {installTabletEventReceiver} = remote.require("receive-tablet-event");

document.addEventListener("DOMContentLoaded", () => {
  const receiver = installTabletEventReceiver(remote.getCurrentWindow());

  receiver.on("enterProximity", (ev) => {
    console.log("enterProximity");
    console.log(ev);
  });
  receiver.on("leaveProximity", (ev) => {
    console.log("leaveProximity");
    console.log(ev);
  });
  receiver.on("down", (ev) => {
    console.log("down");
    console.log(ev);
  });
  receiver.on("move", (ev) => {
    console.log("move")
    console.log(ev);
  });
  receiver.on("up", (ev) => {
    console.log("up");
    console.log(ev);
  });

});
