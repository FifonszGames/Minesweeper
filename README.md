# Minesweeper (Unreal Editor Plugin)

A Minesweeper game plugin created fully using Unreal Engine's Slate UI framework.
This plugin is Editor-only, without relying on UMG or runtime gameplay systems.

## Features

🎮 Playable Minesweeper Game inside the Unreal Editor

🪟 Exist in a dockable tab

🔳 Openable via command or level editor play toolbar button
<img width="585" height="57" alt="image" src="https://github.com/user-attachments/assets/68e72b7c-25bb-49ef-b5d2-5bb85c76c213" />

🖼️ Custom Slate Widgets representing grid cells

📊 2D Array wrapper

📦 ViewModel layer to handle cell data binding and updates

⚙️ Struct Customizations for game settings

## AI Usage

🤖 Used ChatGPT & Gemini 2.5 Pro models for:

  - 🗒️ this readme file 

  - 🏘️ `Array2DUtils::AreNeighbours` and `TArray2D::ForeachNeighbour` functions to speed things up

## Misc

➡️ Made with UE **5.6.1**. Did not check compatibility with previous versions

## Preview
<img width="1907" height="1032" alt="minesweeper" src="https://github.com/user-attachments/assets/fe4c31db-0099-4083-8939-74abefe20500" />
