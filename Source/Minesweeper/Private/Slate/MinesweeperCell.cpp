// Copyright FifonszGames 2025 All Rights Reserved.

#include "Slate/MinesweeperCell.h"

#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

void SMinesweeperCell::Construct(const FArguments& InArgs)
{
	CellData = InArgs._CellData;
	OnCellClicked = InArgs._OnCellClicked;
	
	ChildSlot
	[
		SAssignNew(MainBorder, SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.ShowEffectWhenDisabled(false)
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
	MainBorder->SetEnabled(!bInIsRevealed);
	
	if (bInIsRevealed)
	{
		SetupContentAfterBeingRevealed();
	}
	else
	{
		MainBorder->SetContent(
			SNew(SButton)
			.OnClicked(FOnClicked::CreateSPLambda(this, [this]()
			{
				OnCellClicked.ExecuteIfBound();
				return FReply::Handled(); 
			})));	
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
		MainBorder->SetContent(SNullWidget::NullWidget);
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
		MainBorder->SetContent(SNullWidget::NullWidget);
	}
}

void SMinesweeperCell::SetContentAsBomb()
{
	if (BombImage.IsValid())
	{
		return;
	}
	MainBorder->SetContent(
		SAssignNew(BombImage, SImage)
		.Image(FCoreStyle::Get().GetBrush("Icons.ErrorWithColor")));
}

void SMinesweeperCell::SetContentAsNumber(const uint16 InNumber)
{
	const FText NewText = FText::AsNumber(InNumber);
	if (AdjacentBombsText.IsValid())
	{
		AdjacentBombsText.Pin()->SetText(NewText);
		return;
	}
	MainBorder->SetContent(
		SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SAssignNew(AdjacentBombsText, STextBlock)
			.Text(NewText)
			.Font(FAppStyle::Get().GetFontStyle("Font.Large.Bold"))
			.Justification(ETextJustify::Type::Center)
		]);
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
		MainBorder->SetContent(SNullWidget::NullWidget);
	}
}
