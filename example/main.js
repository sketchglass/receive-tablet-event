const electron = require('electron');
const {app} = electron;
const {BrowserWindow} = electron;
const tablet = require("intercept-tablet-event");

let win;

function createWindow() {
  win = new BrowserWindow({width: 800, height: 600});
  //tablet.intercept(win.getNativeWindowHandle());

  win.loadURL(`file://${__dirname}/index.html`);

  win.webContents.openDevTools();

  win.on('closed', () => {
    win = null;
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  app.quit();
});
