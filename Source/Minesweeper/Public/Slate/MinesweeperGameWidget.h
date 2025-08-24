// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Minesweeper/Public/MinesweeperSettings.h"

class SUniformGridPanel;
class SCheckBox;
class FMinesweeperGameInstance;

enum class EGameEndResult : uint8;

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
	TSharedRef<SWidget> CreateSettingsView(const TSharedRef<TStructOnScope<FMinesweeperGameSettings>>& InitialSettings);
	void OnGameFinished(const EGameEndResult Result);
	
	TSharedPtr<SCheckBox> SettingsCheckbox;
	TSharedPtr<SUniformGridPanel> Grid;
	TSharedPtr<SBox> RatioBox;
	TSharedPtr<FMinesweeperGameInstance> MinesweeperGame;
};
