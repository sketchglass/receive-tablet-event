import {EventEmitter} from "events";
import Electron = require("electron");

declare namespace receiveTabletEvent {
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
    preventDefault(): void;
  }

  export interface Rectangle {
    left: number;
    top: number;
    width: number;
    height: number;
  }

  export interface TabletEventReceiver extends EventEmitter {
    captureArea: Rectangle;
    on(name: "enterProximity", callback: (event: TabletEvent) => void): this;
    on(name: "leaveProximity", callback: (event: TabletEvent) => void): this;
    on(name: "down", callback: (event: TabletEvent) => void): this;
    on(name: "up", callback: (event: TabletEvent) => void): this;
    on(name: "move", callback: (event: TabletEvent) => void): this;
  }
}

declare function receiveTabletEvent(browserWindow: Electron.BrowserWindow): receiveTabletEvent.TabletEventReceiver;

export = receiveTabletEvent;
