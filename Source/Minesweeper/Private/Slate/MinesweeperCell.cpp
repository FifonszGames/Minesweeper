// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperCell.h"

void SMinesweeperCell::Construct(const FArguments& InArgs)
{
	CellData = InArgs._CellData;
	OnUnrevelaedCellClicked = InArgs._OnUnrevelaedCellClicked;
	
	ChildSlot
	[
		SAssignNew(MainButton, SButton)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperCell::OnButtonClicked))
	];

	InitFromCellData(*CellData.Get());
}

void SMinesweeperCell::InitFromCellData(MinesweeperCellData& InitialData)
{
	BIND_VM_WITH_CALLBACK(InitialData.bIsRevealed, SMinesweeperCell::OnIsRevealedChanged);
	BIND_VM_WITH_CALLBACK(InitialData.AdjacentBombs, SMinesweeperCell::OnAdjacentBombsChanged);
}

void SMinesweeperCell::OnIsRevealedChanged(bool bInIsRevealed)
{
	MainButton->SetEnabled(!bInIsRevealed);
	if (!bInIsRevealed || ButtonContent.IsValid())
	{
		return;
	}
}

void SMinesweeperCell::OnAdjacentBombsChanged(uint16 Bombs)
{
	if (!CellData->bIsRevealed)
	{
		return;
	}
}

FReply SMinesweeperCell::OnButtonClicked() const
{
	if (CellData.IsValid() && !CellData->bIsRevealed)
	{
		OnUnrevelaedCellClicked.ExecuteIfBound();
		return FReply::Handled(); 	
	}
	return FReply::Unhandled(); 
}

