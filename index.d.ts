import {EventEmitter} from "events";

type TabletEventTypes = "enterProximity" | "leaveProximity" | "down" | "up" | "move";

interface TabletEvent {
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

declare class InterceptTabletEvent extends EventEmitter {
  constructor(window: Buffer);
  on(name: "event", callback: (event: TabletEvent) => void);
}

export = InterceptTabletEvent;
