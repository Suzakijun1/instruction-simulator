const { exec } = require("child_process");
const { ipcRenderer } = require("electron");

let jsonData = [];
let currentIndex = 0; // Start with no highlight

function runExecutable() {
  let rawData = "";
  const child = exec("hw2.exe");

  child.stdout.on("data", (chunk) => {
    rawData += chunk;
  });

  child.stderr.on("data", (data) => {
    console.error(`stderr: ${data}`);
    updateConsoleLog(`stderr: ${data}`);
  });

  child.on("close", (code) => {
    console.log(`Process exited with code ${code}`);
    processOutput(rawData);
  });
}

let lastProcessedData = null;

function processOutput(data) {
  try {
    const firstBraceIndex = Math.min(
      data.indexOf("{") === -1 ? Infinity : data.indexOf("{"),
      data.indexOf("[") === -1 ? Infinity : data.indexOf("[")
    );
    const lastBraceIndex = Math.max(
      data.lastIndexOf("}") === -1 ? -Infinity : data.lastIndexOf("}"),
      data.lastIndexOf("]") === -1 ? -Infinity : data.lastIndexOf("]")
    );

    if (firstBraceIndex === Infinity || lastBraceIndex === -Infinity) {
      throw new SyntaxError("Invalid JSON format: no braces found");
    }

    const jsonString = data.substring(firstBraceIndex, lastBraceIndex + 1);
    const newJsonData = JSON.parse(jsonString);

    if (JSON.stringify(newJsonData) !== JSON.stringify(lastProcessedData)) {
      jsonData = newJsonData;
      lastProcessedData = newJsonData;
      currentIndex = 0; // No highlight initially
      updateUI();
    }
  } catch (error) {
    console.error("Failed to process output data:", error);
    updateConsoleLog("Failed to process output data: " + error);
  }
}

function updateUI() {
  if (
    jsonData.length === 0 ||
    currentIndex < -1 ||
    currentIndex >= jsonData.length
  ) {
    return;
  }

  const set = currentIndex >= 0 ? jsonData[currentIndex] : null;

  // Displaying instructions
  updateSection(
    "instructions-text",
    set ? set.assemblyInstructions : [],
    set ? set.currentAssemblyInstruction : ""
  );

  // Displaying registers
  const registers = set
    ? Object.entries(set)
        .filter(([key, _]) => key.startsWith("R") && key.length === 2)
        .map(([key, value]) => `${key}: ${value}`)
    : [];
  updateSection("registers-text", registers, "");

  // Displaying memory (binary instructions)
  updateSection(
    "memory-text",
    set ? set.binaryInstructions : [],
    set ? set.currentBinaryInstruction : ""
  );

  // Displaying console log
  const consoleElement = document.getElementById("console-text");
  consoleElement.textContent = set
    ? JSON.stringify(
        {
          currentAssemblyDestReg: set.currentAssemblyDestReg,
          currentAssemblyFirstReg: set.currentAssemblyFirstReg,
          currentAssemblyInstruction: set.currentAssemblyInstruction,
          currentAssemblyOffset: set.currentAssemblyOffset,
          currentAssemblyOpCode: set.currentAssemblyOpCode,
          currentAssemblySecondReg: set.currentAssemblySecondReg,
          currentBinaryDestReg: set.currentBinaryDestReg,
          currentBinaryFirstReg: set.currentBinaryFirstReg,
          currentBinaryInstruction: set.currentBinaryInstruction,
          currentBinaryOffset: set.currentBinaryOffset,
          currentBinaryOpCode: set.currentBinaryOpCode,
          currentBinarySecondReg: set.currentBinarySecondReg,
          programCounter: set.programCounter,
          programStatus: set.programStatus,
        },
        null,
        2
      )
    : "";

  // Update button states
  document.getElementById("next").disabled =
    currentIndex >= jsonData.length - 1;
  document.getElementById("prev").disabled = currentIndex <= 0;
}

function updateSection(elementId, lines, instruction) {
  const element = document.getElementById(elementId);
  element.innerHTML = lines
    .map((line) => {
      if (line === instruction) {
        return `<span class="highlight">${line}</span>`;
      }
      return line;
    })
    .join("\n");
}

function next() {
  currentIndex = Math.min(currentIndex + 1, jsonData.length - 1);
  updateUI();
}

function previous() {
  currentIndex = Math.max(currentIndex - 1, 0);
  updateUI();
}

function updateConsoleLog(message) {
  const consoleElement = document.getElementById("console-text");
  consoleElement.textContent += message + "\n";
}

document.addEventListener("DOMContentLoaded", () => {
  document.getElementById("run-exe").addEventListener("click", runExecutable);
  document.getElementById("next").addEventListener("click", next);
  document.getElementById("prev").addEventListener("click", previous);
  updateUI(); // initialize UI state
});
document.getElementById("generate-exe").addEventListener("click", () => {
  const editorContent = document.getElementById("editor").value;
  ipcRenderer.send("generate-exe", editorContent);
});
ipcRenderer.on("log-message", (event, message) => {
  document.getElementById("console-text").textContent += message + "\n";
});

ipcRenderer.on("output-data", (event, data) => {
  document.getElementById("memory-text").textContent = data;
});
document.getElementById("clear").addEventListener("click", () => {
  document.getElementById("editor").value = "";
  document.getElementById("instructions-text").textContent = "";
  document.getElementById("registers-text").textContent = "";
  document.getElementById("memory-text").textContent = "";
  document.getElementById("console-text").textContent = "";
});
