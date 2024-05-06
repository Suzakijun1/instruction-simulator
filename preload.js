const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("electronAPI", {
  runInstruction: (instruction, callback) => {
    exec(
      `./Simulator`,
      {
        input: instruction,
      },
      (error, stdout, stderr) => {
        if (error) {
          callback(`Error: ${stderr}`);
        } else {
          callback(stdout);
        }
      }
    );
  },
});
