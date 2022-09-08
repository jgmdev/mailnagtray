-- Project build commands and shortcuts for the lite-xl code editor:
-- https://github.com/lite-xl/lite-xl

local core = require "core"
local command = require "core.command"
local keymap = require "core.keymap"
local _, console = pcall(require, "plugins.console")

if type(console) ~= "table" then
  return
end

command.add(nil, {
  ["project:clean"] = function()
    core.log "Cleaning..."
    console.run {
      command = "rm -rf build",
      file_pattern = "(.*):(%d+):(%d+): (.*)$",
      on_complete = function() core.log "Clean complete" end,
    }
  end,

  ["project:setup"] = function()
    core.log "Setting up..."
    console.run {
      command = "meson setup build",
      file_pattern = "(.*):(%d+):(%d+): (.*)$",
      on_complete = function() core.log "Setup complete" end,
    }
  end,

  ["project:build-project"] = function()
    core.log "Building..."
    console.run {
      command = "meson compile -C build",
      file_pattern = "(.*):(%d+):(%d+): (.*)$",
      on_complete = function() core.log "Build complete" end,
    }
  end,

  ["project:run"] = function()
    core.log "Running..."
    console.run {
      command = "./build/mailnagtray",
      file_pattern = "^%.%./(.*):(%d+):(%d+): (.*)$",
      on_complete = function() core.log "Run complete" end,
    }
  end
})

keymap.add { ["ctrl+alt+c"] = "project:clean" }
keymap.add { ["ctrl+alt+s"] = "project:setup" }
keymap.add { ["ctrl+b"]     = "project:build-project" }
keymap.add { ["ctrl+e"]     = "project:run" }
