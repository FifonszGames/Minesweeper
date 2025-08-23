// Copyright FifonszGames 2025 All Rights Reserved.


#include "MinesweeperGameInstance.h"

#include "MinesweeperSettings.h"
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
	//TODO:: get all valid candidates -> all cells - (safe cells + negighbours)
	bHasPlacedMines = true;
}
