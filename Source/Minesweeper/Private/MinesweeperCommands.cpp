// Copyright FifonszGames 2025 All Rights Reserved.

#include "MinesweeperCommands.h"
#include "Minesweeper.h"
#include "Slate/MinesweeperStyle.h"

#define LOCTEXT_NAMESPACE "MinesweeperCommands"

FMinesweeperCommands::FMinesweeperCommands()
		: TCommands(MinesweeperUtils::PluginName, FText::FromName(MinesweeperUtils::PluginName), NAME_None, FMinesweeperStyle::GetName())
{
}

void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenMinesweeperWindow, "Open Minesweeper", "Bring up Minesweeper window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE