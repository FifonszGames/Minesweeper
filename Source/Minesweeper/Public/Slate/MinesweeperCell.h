// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ViewModel/MinesweeperCellData.h"
#include "Widgets/SCompoundWidget.h"

class SBorder;
class SImage;
class STextBlock;

class SMinesweeperCell : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperCell) : _CellData(MakeShared<MinesweeperCellData>())
		{}
		
		SLATE_ARGUMENT(TSharedRef<MinesweeperCellData>, CellData)
		
		SLATE_EVENT(FSimpleDelegate, OnCellClicked)
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

private:
	void InitFromCellData(MinesweeperCellData& InitialData);

	void OnIsBombChanged(bool bInIsBomb);
	void OnIsRevealedChanged(bool bInIsRevealed);
	void OnAdjacentBombsChanged(TOptional<uint16> Bombs);
	void SetContentAsBomb();
	void SetContentAsNumber(const uint16 InNumber);

	void SetupContentAfterBeingRevealed();
	
	TSharedPtr<SBorder> MainBorder;
	TWeakPtr<SImage> BombImage;
	TWeakPtr<STextBlock> AdjacentBombsText;

	TSharedPtr<MinesweeperCellData> CellData;
	FSimpleDelegate OnCellClicked;
};
