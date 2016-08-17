const {app, BrowserWindow, ipcMain} = require('electron');
const {TabletEventReceiver} = require("receive-tablet-event");

let win;

function createWindow() {
  win = new BrowserWindow({width: 800, height: 600});

  win.loadURL(`file://${__dirname}/index.html`);

  win.webContents.openDevTools();

  win.on('closed', () => {
    win = null;
  });

  ipcMain.on("tablet:install", (ev, captureArea) => {
    const receiver = new TabletEventReceiver(win);
    receiver.captureArea = captureArea;
    const eventNames = ["enterProximity", "leaveProximity", "down", "move", "up"];
    for (const name of eventNames) {
      receiver.on(name, (ev) => {
        console.log(name);
        console.log(ev);
        win.webContents.send(`tablet:${name}`, ev);
      });
    }
    win.on('closed', () => {
      receiver.dispose();
    })
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  app.quit();
});
