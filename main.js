const { app, BrowserWindow, ipcMain } = require("electron");
const { exec } = require("child_process");
const fs = require("fs");
const path = require("path");
const { spawn } = require("child_process");

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
    },
  });

  win.loadFile("index.html");
}

app.whenReady().then(createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

ipcMain.on("generate-exe", (event, instructions) => {
  const sourceDir = path.resolve(__dirname);
  const buildDir = path.resolve(__dirname, "tests");

  // Ensure the build directory exists and clean it
  if (fs.existsSync(buildDir)) {
    fs.rmSync(buildDir, { recursive: true });
  }
  fs.mkdirSync(buildDir, { recursive: true });

  // Save the instructions to a JSON file
  const instructionFilePath = path.join(buildDir, "instructions.json");
  fs.writeFile(instructionFilePath, instructions, (err) => {
    if (err) {
      console.error(`Write file error: ${err}`);
      event.sender.send("log-message", `Write file error: ${err}`);
      return;
    }

    console.log("Instructions file written");
    event.sender.send("log-message", "Instructions file written");

    // Run CMake configuration
    const cmakeConfigure = spawn("cmake", ["."], {
      cwd: sourceDir,
      shell: true,
    });
    cmakeConfigure.stdout.on("data", (data) => {
      console.log(`CMake stdout: ${data}`);
      event.sender.send("log-message", `CMake stdout: ${data}`);
    });
    cmakeConfigure.stderr.on("data", (data) => {
      console.error(`CMake stderr: ${data}`);
      event.sender.send("log-message", `CMake stderr: ${data}`);
    });
    cmakeConfigure.on("close", (code) => {
      if (code !== 0) {
        console.error(`CMake process exited with code ${code}`);
        event.sender.send(
          "log-message",
          `CMake process exited with code ${code}`
        );
        return;
      }

      console.log("CMake configuration complete");
      event.sender.send("log-message", "CMake configuration complete");

      // Build the project
      const cmakeBuild = spawn("cmake", ["--build", "."], {
        cwd: sourceDir,
        shell: true,
      });
      cmakeBuild.stdout.on("data", (data) => {
        console.log(`Build stdout: ${data}`);
        event.sender.send("log-message", `Build stdout: ${data}`);
      });
      cmakeBuild.stderr.on("data", (data) => {
        console.error(`Build stderr: ${data}`);
        event.sender.send("log-message", `Build stderr: ${data}`);
      });
      cmakeBuild.on("close", (code) => {
        if (code !== 0) {
          console.error(`Build process exited with code ${code}`);
          event.sender.send(
            "log-message",
            `Build process exited with code ${code}`
          );
          return;
        }

        console.log("Build complete");
        event.sender.send("log-message", "Build complete");

        // Now run the executable
        const executablePath = path.join(sourceDir, "hw2.exe");
        console.log(`Executable path: ${executablePath}`);

        const runExecutable = spawn(executablePath, [], {
          cwd: sourceDir,
          shell: true,
        });
        runExecutable.stdout.on("data", (data) => {
          console.log(`Exec stdout: ${data}`);
          event.sender.send("output-data", data);
        });
        runExecutable.stderr.on("data", (data) => {
          console.error(`Exec stderr: ${data}`);
          event.sender.send("log-message", `Exec stderr: ${data}`);
        });
        runExecutable.on("close", (code) => {
          if (code !== 0) {
            console.error(`Exec process exited with code ${code}`);
            event.sender.send(
              "log-message",
              `Exec process exited with code ${code}`
            );
            return;
          }

          console.log("Execution complete");
          event.sender.send("log-message", "Execution complete");
        });
      });
    });
  });
});
