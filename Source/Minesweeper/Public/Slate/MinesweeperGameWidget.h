// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Minesweeper/Public/MinesweeperSettings.h"

class SUniformGridPanel;
class SCheckBox;

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
	void RecreateGrid(const FMinesweeperGameSettings& InSettings) const;
	TSharedRef<SWidget> CreateGrid(const FMinesweeperGameSettings& InitialSettings);
	
	FReply OnRecreateClicked() const;

	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> Settings;
	
	TSharedPtr<SCheckBox> SettingsCheckbox;
	TSharedPtr<SUniformGridPanel> Grid;
};
