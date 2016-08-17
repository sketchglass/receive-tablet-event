"use strict";

const {ipcRenderer} = require("electron");

document.addEventListener("DOMContentLoaded", () => {
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
  const captureArea = {
    left: Math.round(rect.left),
    top: Math.round(rect.top),
    width: Math.round(rect.width),
    height: Math.round(rect.height),
  }
  ipcRenderer.send("tablet:install", captureArea);

  ipcRenderer.on("tablet:down", (ev, arg) => {
    pressed = true;
    x = arg.clientX;
    y = arg.clientY;
    pressure = arg.pressure;
    updateInfo();
  });
  ipcRenderer.on("tablet:move", (ev, arg) => {
    x = arg.clientX;
    y = arg.clientY;
    pressure = arg.pressure;
    updateInfo();
  });
  ipcRenderer.on("tablet:up", (ev, arg) => {
    pressed = false;
    x = arg.clientX;
    y = arg.clientY;
    pressure = arg.pressure;
    updateInfo();
  });
});
