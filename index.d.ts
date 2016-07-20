import {EventEmitter} from "events";

type TabletEventTypes = "enterProximity" | "leaveProximity" | "down" | "up" | "move";

export interface TabletEvent {
  type: TabletEventTypes;
  altKey: boolean;
  ctrlKey: boolean;
  metaKey: boolean;
  shiftKey: boolean;
  button: number;
  buttons: number;
  clientX: number;
  clientY: number;
  screenX: number;
  screenY: number;
  pressure: number;
  tiltX: number;
  tiltY: number;
  pointerId: number;
}

export declare class TabletEventReceiver extends EventEmitter {
  constructor(window: Buffer);
  on(name: "event", callback: (event: TabletEvent) => void);
}
