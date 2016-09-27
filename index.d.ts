import {EventEmitter} from "events";
import Electron = require("electron");

export type PointerTypes = "eraser" | "pen" | "cursor" | "unknown";

export interface TabletEvent {
  clientX: number;
  clientY: number;
  pressure: number;
  // TODO
  // tiltX: number;
  // tiltY: number;
  pointerType: PointerTypes;
  pointerId: number;
  button: number;
  preventDefault(): void;
}

export interface Rectangle {
  left: number;
  top: number;
  width: number;
  height: number;
}

export class TabletEventReceiver extends EventEmitter {
  constructor(win: Electron.BrowserWindow);
  dispose(): void;
  captureArea: Rectangle;
  on(name: "enterProximity", callback: (event: TabletEvent) => void): this;
  on(name: "leaveProximity", callback: (event: TabletEvent) => void): this;
  on(name: "down", callback: (event: TabletEvent) => void): this;
  on(name: "up", callback: (event: TabletEvent) => void): this;
  on(name: "move", callback: (event: TabletEvent) => void): this;
}
