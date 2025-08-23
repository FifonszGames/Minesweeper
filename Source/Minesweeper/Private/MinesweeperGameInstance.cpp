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

void FMinesweeperGameInstance::CellSelected(const FUintPoint& Coords)
{
	if (!Cells.AreCoordinatesValid(Coords))
	{
		return;
	}
	
	TSharedPtr<MinesweeperCellData> CellData = Cells.Get(Coords);
	check(CellData.IsValid())
	if (!bHasPlacedMines)
	{
		PlaceMines(Coords);
	}
	
	const ERevealCellResult Result = RevealCell(CellData.ToSharedRef(), Coords);
	switch (Result)
	{
		case ERevealCellResult::IsBomb:
			OnFinished.ExecuteIfBound(EGameEndResult::Failure);
			break;
		case ERevealCellResult::Valid:
			{
				if (ValidCellsLeft.IsEmpty())
				{
					OnFinished.ExecuteIfBound(EGameEndResult::Success);
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
