// Copyright FifonszGames 2025 All Rights Reserved.


#include "MinesweeperGameInstance.h"

#include "MinesweeperSettings.h"
#include "Algo/RandomShuffle.h"
#include "ViewModel/MinesweeperCellData.h"


void FMinesweeperGameInstance::Init()
{
	bHasPlacedMines = false;
	
	const FMinesweeperGameSettings* Settings = GameSettings->Get();
	Cells.Init(MakeShared<MinesweeperCellData>(), FUintPoint(Settings->Width, Settings->Height));
}

void FMinesweeperGameInstance::CellSelected(const FUintPoint& Coords)
{
	if (!Cells.AreCoordinatesValid(Coords))
	{
		return;
	}
	
	TSharedPtr<MinesweeperCellData> CellData = Cells.Get(Coords);
	check(CellData.IsValid())
	if (CellData->bIsRevealed)
	{
		return;
	}
	if (!bHasPlacedMines)
	{
		PlaceMines(Coords);
	}
	CellData->bIsRevealed.Set(true);
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
	
	Algo::RandomShuffle(Candidates);
	const FMinesweeperGameSettings* Settings = GameSettings->Get();
	for (int32 i = 0; i < Settings->NumberOfMines; ++i)
	{
		Cells.Get(Candidates[i])->bIsBomb.Set(true);
	}
	bHasPlacedMines = true;
}
