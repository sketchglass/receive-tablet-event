"use strict";

const {remote} = require("electron");
const receiveTabletEvent = remote.require("receive-tablet-event");

document.addEventListener("DOMContentLoaded", () => {
  const receiver = receiveTabletEvent(remote.getCurrentWindow());

  let pressed = false;
  let x = 0;
  let y = 0;
  let pressure = 0;

  function updateInfo() {
    document.getElementById("position").innerText = `(${x}, ${y})`;
    document.getElementById("pressure").innerText = pressure;
    document.getElementById("pressed").innerText = pressed;
  }

  const rect = document.getElementById("capture-area").getBoundingClientRect();
  receiver.captureArea = {
    left: Math.round(rect.left),
    top: Math.round(rect.top),
    width: Math.round(rect.width),
    height: Math.round(rect.height),
  }

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
    pressed = true;
    x = ev.clientX;
    y = ev.clientY;
    pressure = ev.pressure;
    updateInfo();
  });
  receiver.on("move", (ev) => {
    console.log("move")
    console.log(ev);
    x = ev.clientX;
    y = ev.clientY;
    pressure = ev.pressure;
    updateInfo();
  });
  receiver.on("up", (ev) => {
    console.log("up");
    console.log(ev);
    pressed = false;
    x = ev.clientX;
    y = ev.clientY;
    pressure = ev.pressure;
    updateInfo();
  });

});
