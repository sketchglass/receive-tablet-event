const electron = require('electron');
const {app} = electron;
const {BrowserWindow} = electron;
const {TabletEventReceiver} = require("receive-tablet-event");

let win;

function createWindow() {
  win = new BrowserWindow({width: 800, height: 600});

  win.webContents.once("did-finish-load", () => {
    receiver = new TabletEventReceiver(win);
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
  })

  win.loadURL(`file://${__dirname}/index.html`);

  win.webContents.openDevTools();

  win.on('closed', () => {
    receiver.dispose();
    win = null;
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  app.quit();
});
