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
	void CellSelected(const uint16 CellX, const uint16 CellY);
	
	const TArray2D<TSharedPtr<MinesweeperCellData>>& GetCells() const { return Cells; }

private:
	void PlaceMines(const uint16 SafeX, const uint16 SafeY);
	
	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> GameSettings;
	TArray2D<TSharedPtr<MinesweeperCellData>> Cells;
	bool bHasPlacedMines = false;
};
