// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Array2D.h"

struct FMinesweeperGameSettings;
class MinesweeperCellData;

enum class ERevealCellResult : uint8
{
	AlreadyRevealed,
	IsBomb,
	Valid
};

enum class EGameEndResult : uint8
{
	Success,
	Failure
};

DECLARE_DELEGATE_OneParam(FOnFinished, const EGameEndResult)

class FMinesweeperGameInstance
{
public:
	explicit FMinesweeperGameInstance(const TSharedRef<TStructOnScope<FMinesweeperGameSettings>>& InSettings) : GameSettings(InSettings) {}

	void Init();
	void CellSelected(const FUintPoint& SelectedCoords);
	
	const TSharedArray2D<MinesweeperCellData>& GetCells() const { return Cells; }
	
	FOnFinished OnFinished;
private:
	void PlaceMines(const FUintPoint& SafeCell);
	ERevealCellResult RevealCell(const TSharedRef<MinesweeperCellData>& InRevealedCell, const FUintPoint& Coords);
	uint16 GetAdjacentBombs(const TSharedRef<MinesweeperCellData>& InRevealedCell, const FUintPoint& Coords);

	void GameEnded(const EGameEndResult InResult) const;
	
	TSharedPtr<TStructOnScope<FMinesweeperGameSettings>> GameSettings;
	TSharedArray2D<MinesweeperCellData> Cells;
	TSet<FUintPoint> ValidCellsLeft;
	bool bHasPlacedMines = false;
};
