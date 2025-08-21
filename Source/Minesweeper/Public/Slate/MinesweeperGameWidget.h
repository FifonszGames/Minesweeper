// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Minesweeper/Public/MinesweeperSettings.h"

class SMinesweeperGameWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperGameWidget)
		: _InitialSettings(TOptional<FMinesweeperGameSettings>())
		{}

	SLATE_ARGUMENT(TOptional<FMinesweeperGameSettings>, InitialSettings)
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> CreateSettingsView(const FMinesweeperGameSettings& InitialSettings);

	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> Settings;
};
