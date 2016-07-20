const electron = require('electron');
const {app} = electron;
const {BrowserWindow} = electron;
const {TabletEventReceiver} = require("..");

let win;

function createWindow() {
  win = new BrowserWindow({width: 800, height: 600});

  const receiver = new TabletEventReceiver(win.getNativeWindowHandle());
  receiver.on("move", (ev) => {
    console.log(ev.x, ev.y, ev.pressure);
  });

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
