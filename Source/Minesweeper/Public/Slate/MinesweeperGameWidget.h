// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Minesweeper/Public/MinesweeperSettings.h"

class SUniformGridPanel;
class SCheckBox;
class MinesweeperCellData;
class FMinesweeperGameInstance;

class SMinesweeperGameWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperGameWidget)
		: _InitialSettings(TOptional<FMinesweeperGameSettings>()){}

	SLATE_ARGUMENT(TOptional<FMinesweeperGameSettings>, InitialSettings)
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

private:
	void RecreateGridSlots();
	
	TSharedPtr<SCheckBox> SettingsCheckbox;
	TSharedPtr<SUniformGridPanel> Grid;
	TSharedPtr<FMinesweeperGameInstance> MinesweeperGame;
};
