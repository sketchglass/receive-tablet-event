"use strict";

const {ipcRenderer} = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  let pressed = false;

  function updateInfo(ev) {
    const {x, y, pressure, altKey, ctrlKey, metaKey, shiftKey} = ev
    document.getElementById("position").innerText = `(${x}, ${y})`;
    document.getElementById("pressure").innerText = pressure;
    document.getElementById("pressed").innerText = pressed;
    document.getElementById("alt").innerText = altKey;
    document.getElementById("ctrl").innerText = ctrlKey;
    document.getElementById("meta").innerText = metaKey;
    document.getElementById("shift").innerText = shiftKey;
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
    updateInfo(arg);
  });
  ipcRenderer.on("tablet:move", (ev, arg) => {
    updateInfo(arg);
  });
  ipcRenderer.on("tablet:up", (ev, arg) => {
    pressed = false;
    updateInfo(arg);
  });
});
