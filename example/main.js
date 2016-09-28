const {app, BrowserWindow, ipcMain} = require('electron');
const {TabletEventReceiver} = require("..");

let win;

function createWindow() {
  win = new BrowserWindow({width: 800, height: 600});

  win.loadURL(`file://${__dirname}/index.html`);
  win.webContents.openDevTools();

  const receiver = new TabletEventReceiver(win);
  const eventNames = ["enterProximity", "leaveProximity", "down", "move", "up"];
  for (const name of eventNames) {
    receiver.on(name, (ev) => {
      console.log(name);
      console.log(ev);
      win.webContents.send(`tablet:${name}`, ev);
    });
  }

  win.on('closed', () => {
    win = null;
    receiver.dispose();
  });

  ipcMain.on("tablet:install", (ev, captureArea) => {
    receiver.captureArea = captureArea;
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  app.quit();
});
