// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperCell.h"

void SMinesweeperCell::Construct(const FArguments& InArgs)
{
	CellData = InArgs._CellData;
	OnCellClicked = InArgs._OnCellClicked;
	
	ChildSlot
	[
		SAssignNew(MainButton, SButton)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.OnClicked(FOnClicked::CreateSPLambda(this, [this]()
		{
			OnCellClicked.ExecuteIfBound();
			return FReply::Handled(); 
		}))
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
