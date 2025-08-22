// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Array2D.h"
#include "Widgets/SCompoundWidget.h"
#include "Minesweeper/Public/MinesweeperSettings.h"

class SUniformGridPanel;
class SCheckBox;
class MinesweeperCellData;

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
	TSharedRef<SWidget> CreateGrid(const FMinesweeperGameSettings& InitialSettings);
	
	void RecreateGrid(const FMinesweeperGameSettings& InSettings);

	FReply OnRecreateClicked();
	void UnrevelaedCellClicked(uint16 CellX, uint16 CellY);

	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> Settings;
	
	TSharedPtr<SCheckBox> SettingsCheckbox;
	TSharedPtr<SUniformGridPanel> Grid;

	TArray2D<TSharedPtr<MinesweeperCellData>> Cells;
};
