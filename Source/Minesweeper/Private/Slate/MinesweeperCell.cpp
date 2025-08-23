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
	MainButton->SetBorderBackgroundColor(bInIsRevealed ? FSlateColor(FColor::Silver) : FSlateColor(EStyleColor::Foreground));
	if (MainButton->IsEnabled() == !bInIsRevealed)
	{
		return;
	}
	
	MainButton->SetEnabled(!bInIsRevealed);
	
	const bool bButtonHasContent = ButtonContent.IsValid();
	if (bInIsRevealed)
	{
		if (!bButtonHasContent)
		{
			SetupContentAfterBeingRevealed();
		}
	}
	else
	{
		if (bButtonHasContent)
		{
			ButtonContent.Reset();
			MainButton->SetContent(SNullWidget::NullWidget);	
		}
	}
}

void SMinesweeperCell::OnAdjacentBombsChanged(uint16 Bombs)
{
	if (!CellData->bIsRevealed)
	{
		return;
	}
}

void SMinesweeperCell::SetupContentAfterBeingRevealed()
{
	check(CellData.IsValid())
	const MinesweeperCellData& Data = *CellData.Get();
	TSharedPtr<SWidget> Content;
	if (Data.bIsBomb)
	{
		Content = SNew(SImage)
			.Image(FCoreStyle::Get().GetBrush("Icons.ErrorWithColor"));
	}
	else if (Data.AdjacentBombs > 0)
	{
		Content = SNew(STextBlock)
			.Text(FText::Format(INVTEXT("{0}"), Data.AdjacentBombs.Get()))
			.Justification(ETextJustify::Type::Center);
	}
	
	if(Content.IsValid())
	{
		ButtonContent = Content;
		MainButton->SetContent(Content.ToSharedRef());
	}
}
