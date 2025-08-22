// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ViewModel/MinesweeperCellData.h"
#include "Widgets/SCompoundWidget.h"


class SMinesweeperCell : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperCell) : _CellData(MakeShared<MinesweeperCellData>())
		{}
		
		SLATE_ARGUMENT(TSharedRef<MinesweeperCellData>, CellData)
		
		SLATE_EVENT(FSimpleDelegate, OnUnrevelaedCellClicked)
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

private:
	void InitFromCellData(MinesweeperCellData& InitialData);
	
	void OnIsRevealedChanged(bool bInIsRevealed);
	void OnAdjacentBombsChanged(uint16 Bombs);

	FReply OnButtonClicked() const;
	
	TSharedPtr<SButton> MainButton;
	
	TSharedPtr<MinesweeperCellData> CellData;
	TSharedPtr<SWidget> ButtonContent;
	FSimpleDelegate OnUnrevelaedCellClicked;
};
