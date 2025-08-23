// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
struct TArray2D
{
	void Init(const T& Value, const FUintPoint& Size)
	{
		Columns = Size.X;
		Rows = Size.Y;
		Data.Init(Value, Columns * Rows);
	}

	bool AreCoordinatesValid(const FUintPoint& Coords) const
	{
		return Coords.X < Columns && Coords.Y < Rows;
	}
	
	T& Get(const FUintPoint& Coords)
	{
		check(AreCoordinatesValid(Coords))
		return Data[CoordinateToIndex(Coords)];
	}

	const T& Get(const FUintPoint& Coords) const
	{
		check(AreCoordinatesValid(Coords))
		return Data[CoordinateToIndex(Coords)];
	}

	void Foreach(const TFunctionRef<void(const FUintPoint& Coords, const T& Value)>& Func) const
	{
		for (uint16 y = 0; y < Rows; ++y)
		{
			for (uint16 x = 0; x < Columns; ++x)
			{
				const FUintPoint Coords(x,y);
				Func(Coords, Data[CoordinateToIndex(Coords)]);
			}	
		}
	}

private:
	uint16 CoordinateToIndex(const FUintPoint& Coords) const
	{
		return Coords.Y * Columns + Coords.X;
	}
	
	uint16 Columns = 0;
	uint16 Rows = 0;
	TArray<T> Data;
};