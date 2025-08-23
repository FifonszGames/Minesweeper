// Copyright FifonszGames 2025 All Rights Reserved.


#include "MinesweeperGameInstance.h"

#include "MinesweeperSettings.h"
#include "ViewModel/MinesweeperCellData.h"


void FMinesweeperGameInstance::Init()
{
	bHasPlacedMines = false;
	
	const FMinesweeperGameSettings* Settings = GameSettings->Get();
	Cells.Init(MakeShared<MinesweeperCellData>(), Settings->Width, Settings->Height);
}

void FMinesweeperGameInstance::CellSelected(const uint16 CellX, const uint16 CellY)
{
	if (!Cells.AreCoordinatesValid(CellX, CellY))
	{
		return;
	}
	
	TSharedPtr<MinesweeperCellData> CellData = Cells.Get(CellX, CellY);
	check(CellData.IsValid())
	if (CellData->bIsRevealed)
	{
		return;
	}
	if (!bHasPlacedMines)
	{
		PlaceMines(CellX, CellY);
	}
	CellData->bIsRevealed.Set(true);
}

void FMinesweeperGameInstance::PlaceMines(const uint16 SafeX, const uint16 SafeY)
{
	//TODO::implement
	bHasPlacedMines = true;
}
