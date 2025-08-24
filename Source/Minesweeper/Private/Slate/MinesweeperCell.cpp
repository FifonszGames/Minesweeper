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

void SMinesweeperCell::OnIsBombChanged(bool bInIsBomb)
{
	const MinesweeperCellData& Cell = *CellData.Get();
	if (!Cell.bIsRevealed)
	{
		return;
	}
	if (bInIsBomb)
	{
		SetContentAsBomb();
	}
	else if (!BombImage.IsValid())
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}

void SMinesweeperCell::OnAdjacentBombsChanged(TOptional<uint16> Bombs)
{
	const MinesweeperCellData& Cell = *CellData.Get();
	if (!Cell.bIsRevealed)
	{
		return;
	}
	if (Bombs.IsSet() && Bombs.GetValue() > 0)
	{
		SetContentAsNumber(Bombs.GetValue());	
	}
	else if (!AdjacentBombsText.IsValid())
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}

void SMinesweeperCell::SetContentAsBomb()
{
	if (BombImage.IsValid())
	{
		return;
	}
	MainButton->SetContent(SAssignNew(BombImage, SImage)
	.Image(CellUtils::GetBombBrush()));
}

void SMinesweeperCell::SetContentAsNumber(const uint16 InNumber)
{
	const FText NewText = FText::AsNumber(InNumber);
	if (AdjacentBombsText.IsValid())
	{
		AdjacentBombsText.Pin()->SetText(NewText);
		return;
	}
	MainButton->SetContent(SAssignNew(AdjacentBombsText, STextBlock)
	.Text(NewText)
	.Justification(ETextJustify::Type::Center));
}

void SMinesweeperCell::SetupContentAfterBeingRevealed()
{
	check(CellData.IsValid())
	const MinesweeperCellData& Data = *CellData.Get();
	if (Data.bIsBomb)
	{
		SetContentAsBomb();
	}
	else if (Data.AdjacentBombs.GetRef().IsSet() && Data.AdjacentBombs.GetRef().GetValue() > 0)
	{
		SetContentAsNumber(Data.AdjacentBombs.GetRef().GetValue());
	}
	else
	{
		MainButton->SetContent(SNullWidget::NullWidget);
	}
}
