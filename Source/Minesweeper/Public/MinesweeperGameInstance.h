// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Array2D.h"

struct FMinesweeperGameSettings;
class MinesweeperCellData;

class FMinesweeperGameInstance
{
public:
	explicit FMinesweeperGameInstance(const TSharedRef<TStructOnScope<FMinesweeperGameSettings>>& InSettings) : GameSettings(InSettings) {}

	void Init();
	void CellSelected(const FUintPoint& Coords);
	
	const TSharedArray2D<MinesweeperCellData>& GetCells() const { return Cells; }

private:
	void PlaceMines(const FUintPoint& SafeCell);
	
	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> GameSettings;
	TSharedArray2D<MinesweeperCellData> Cells;
	bool bHasPlacedMines = false;
};
