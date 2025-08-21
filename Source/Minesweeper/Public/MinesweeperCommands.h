// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"

class FMinesweeperCommands : public TCommands<FMinesweeperCommands>
{
public:
	FMinesweeperCommands();
	
	virtual void RegisterCommands() override;
	
	TSharedPtr<FUICommandInfo> OpenMinesweeperWindow;
};