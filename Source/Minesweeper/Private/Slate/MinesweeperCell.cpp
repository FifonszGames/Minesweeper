// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperCell.h"

namespace CellUtils
{
	const FSlateBrush* GetBombBrush() { return FCoreStyle::Get().GetBrush("Icons.ErrorWithColor"); }
}

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
	BIND_VM_WITH_CALLBACK(InitialData.bIsBomb, SMinesweeperCell::OnIsBombChanged);
}

void SMinesweeperCell::OnIsRevealedChanged(bool bInIsRevealed)
{
	MainButton->SetBorderBackgroundColor(bInIsRevealed ? FSlateColor(FColor::Silver) : FSlateColor(EStyleColor::Foreground));
	if (MainButton->IsEnabled() == !bInIsRevealed)
	{
		return;
	}
	
	MainButton->SetEnabled(!bInIsRevealed);
	
	if (bInIsRevealed)
	{
		SetupContentAfterBeingRevealed();
	}
	else
	{
		MainButton->SetContent(SNullWidget::NullWidget);	
	}
}

void SMinesweeperCell::OnIsBombChanged(bool bInIsBomb) const
{
	if (!BombImage.IsValid())
	{
		return;
	}
	if (bInIsBomb)
	{
		BombImage.Pin()->SetImage(CellUtils::GetBombBrush());
	}
	else if (CellData.Get()->AdjacentBombs.Get() == 0)
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}

void SMinesweeperCell::OnAdjacentBombsChanged(uint16 Bombs) const
{
	if (!AdjacentBombsText.IsValid())
	{
		return;
	}
	if (Bombs > 0)
	{
		AdjacentBombsText.Pin()->SetText(FText::AsNumber(Bombs));	
	}
	else if (!CellData.Get()->bIsBomb)
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}

void SMinesweeperCell::SetupContentAfterBeingRevealed()
{
	check(CellData.IsValid())
	const MinesweeperCellData& Data = *CellData.Get();
	if (Data.bIsBomb)
	{
		MainButton->SetContent(SAssignNew(BombImage, SImage)
			.Image(CellUtils::GetBombBrush()));
	}
	else if (Data.AdjacentBombs > 0)
	{
		MainButton->SetContent(SAssignNew(AdjacentBombsText, STextBlock)
			.Text(FText::AsNumber(Data.AdjacentBombs.Get()))
			.Justification(ETextJustify::Type::Center));
	}
	else
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}
