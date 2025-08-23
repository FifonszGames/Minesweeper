// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
struct TArray2D
{
	void Init(const T& Value, const int32 InColumns, const int32 InRows)
	{
		Columns = InColumns;
		Rows = InRows;
		Data.Init(Value, Columns * Rows);
	}

	bool AreCoordinatesValid(const uint16 X, const uint16 Y) const
	{
		return X < Columns && Y < Rows;
	}
	
	T& Get(const uint16 X, const uint16 Y)
	{
		check(AreCoordinatesValid(X, Y))
		return Data[CoordinateToIndex(X, Y)];
	}

	const T& Get(const uint16 X, const uint16 Y) const
	{
		check(AreCoordinatesValid(X, Y))
		return Data[CoordinateToIndex(X, Y)];
	}

	void Foreach(const TFunctionRef<void(const uint16 X, const uint16 Y, const T& Value)>& Func) const
	{
		for (uint16 y = 0; y < Rows; ++y)
		{
			for (uint16 x = 0; x < Columns; ++x)
			{
				Func(x, y, Data[CoordinateToIndex(x, y)]);
			}	
		}
	}

private:
	uint16 CoordinateToIndex(const uint16 X, const uint16 Y) const
	{
		return Y * Columns + X;
	}
	
	uint16 Columns = 0;
	uint16 Rows = 0;
	TArray<T> Data;
};