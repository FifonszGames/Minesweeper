// Copyright FifonszGames 2025 All Rights Reserved.


#include "MinesweeperGameInstance.h"

#include "MinesweeperSettings.h"
#include "Algo/RandomShuffle.h"
#include "ViewModel/MinesweeperCellData.h"


void FMinesweeperGameInstance::Init()
{
	bHasPlacedMines = false;
	ValidCellsLeft.Reset();
	const FMinesweeperGameSettings& Settings = *GameSettings->Get();
	Cells.Init(MakeShared<MinesweeperCellData>(), FUintPoint(Settings.Width, Settings.Height));
}

void FMinesweeperGameInstance::GameEnded(const EGameEndResult InResult) const
{
	Cells.Foreach([&](const FUintPoint& Coords, const TSharedPtr<MinesweeperCellData>& Value)
	{
		if (Value->bIsBomb && !Value->bIsRevealed)
		{
			Value->bIsRevealed.Set(true);
		}
	});
	OnFinished.ExecuteIfBound(InResult);
}

void FMinesweeperGameInstance::CellSelected(const FUintPoint& SelectedCoords)
{
	if (!Cells.AreCoordinatesValid(SelectedCoords))
	{
		return;
	}
	
	TSharedPtr<MinesweeperCellData> CellData = Cells.Get(SelectedCoords);
	check(CellData.IsValid())
	if (!bHasPlacedMines)
	{
		PlaceMines(SelectedCoords);
	}
	
	const ERevealCellResult Result = RevealCell(CellData.ToSharedRef(), SelectedCoords);
	switch (Result)
	{
		case ERevealCellResult::IsBomb:
			{
				GameEnded(EGameEndResult::Failure);
				break;
			}
		case ERevealCellResult::Valid:
			{
				if (ValidCellsLeft.IsEmpty())
				{
					GameEnded(EGameEndResult::Success);
				}
				break;
			}
		default:
			break;
	}
}

void FMinesweeperGameInstance::PlaceMines(const FUintPoint& SafeCell)
{
	TArray<FUintPoint> Candidates;
	Candidates.Reserve(Cells.Num() - Cells.GetNumNeighbors(SafeCell) - 1);
	
	Cells.Foreach([&](const FUintPoint& Coords, const TSharedPtr<MinesweeperCellData>& Value)
	{
		if (Coords == SafeCell || Array2DUtils::AreNeighbours(FIntPoint(SafeCell), FIntPoint(Coords)))
		{
			return;
		}
		Candidates.Add(Coords);
	});
	
	if (!Candidates.IsEmpty())
	{
		Algo::RandomShuffle(Candidates);
		const int32 NumOfMines = FMath::Min(GameSettings->Get()->NumberOfMines, Candidates.Num());
		for (int32 i = 0; i < NumOfMines; ++i)
		{
			Cells.Get(Candidates[i])->bIsBomb.Set(true);
		}
		
		ValidCellsLeft.Empty(Candidates.Num() - NumOfMines);
		for (int32 i = NumOfMines; i < Candidates.Num(); ++i)
		{
			ValidCellsLeft.Add(Candidates[i]);
		}	
	}
	else
	{
		ValidCellsLeft.Reset();
	}
	
	bHasPlacedMines = true;
}

ERevealCellResult FMinesweeperGameInstance::RevealCell(const TSharedRef<MinesweeperCellData>& InRevealedCell, const FUintPoint& Coords)
{
	if (InRevealedCell->bIsRevealed)
	{
		return ERevealCellResult::AlreadyRevealed;
	}
	
	InRevealedCell->bIsRevealed.Set(true);
	ValidCellsLeft.Remove(Coords);
	if (InRevealedCell->bIsBomb)
	{
		return ERevealCellResult::IsBomb;
	}
	const uint16 AdjacentBombs = GetAdjacentBombs(InRevealedCell, Coords);
	if (AdjacentBombs == 0)
	{
		Cells.ForeachNeighbour(Coords, [this](const FUintPoint& NeighbourCoords)
		{
			const TSharedPtr<MinesweeperCellData>& Cell = Cells.Get(NeighbourCoords);
			RevealCell(Cell.ToSharedRef(), NeighbourCoords);
		});
	}
	return ERevealCellResult::Valid;
}

uint16 FMinesweeperGameInstance::GetAdjacentBombs(const TSharedRef<MinesweeperCellData>& InRevealedCell, const FUintPoint& Coords)
{
	if (InRevealedCell->AdjacentBombs.GetRef().IsSet())
	{
		return InRevealedCell->AdjacentBombs.GetRef().GetValue();
	}
	uint16 Count = 0;
	Cells.ForeachNeighbour(Coords, [&Count, this](const FUintPoint& NeighbourCoords)
	{
		if (Cells.Get(NeighbourCoords)->bIsBomb)
		{
			Count++;
		}
	});
	InRevealedCell->AdjacentBombs.Set(Count);
	return Count;
}
