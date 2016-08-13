import {EventEmitter} from "events";
import Electron = require("electron");

type PointerTypes = "eraser" | "pen" | "cursor" | "unknown";

export interface ProximityEvent {
  pointerType: PointerTypes;
  pointerId: number;
}

export interface TabletEvent {
  altKey: boolean;
  ctrlKey: boolean;
  metaKey: boolean;
  shiftKey: boolean;
  clientX: number;
  clientY: number;
  screenX: number;
  screenY: number;
  pressure: number;
  // TODO
  // tiltX: number;
  // tiltY: number;
  pointerType: PointerTypes;
  pointerId: number;
}

export declare class TabletEventReceiver extends EventEmitter {
  constructor(window: Electron.BrowserWindow);
  on(name: "enterProximity", callback: (event: ProximityEvent) => void): this;
  on(name: "leaveProximity", callback: (event: ProximityEvent) => void): this;
  on(name: "down", callback: (event: TabletEvent) => void): this;
  on(name: "up", callback: (event: TabletEvent) => void): this;
  on(name: "move", callback: (event: TabletEvent) => void): this;
  dispose(): void;
}
