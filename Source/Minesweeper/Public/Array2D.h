// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Array2DUtils
{
	bool AreNeighbours(const FIntPoint& A, const FIntPoint& B);
}

template<typename T>
struct TArray2D
{
	virtual ~TArray2D() = default;

	void Init(const T& Value, const FUintPoint& Size)
	{
		Columns = Size.X;
		Rows = Size.Y;
		InitInternal(Value, Columns * Rows);
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

	uint16 Num() const { return Data.Num(); }

	uint16 GetNumNeighbors(const FUintPoint& Cell) const
	{
		uint16 MaxNeighbors = 8;
		
		bool bLeft   = (Cell.X == 0);
		bool bRight  = (Cell.X == Columns - 1);
		bool bTop    = (Cell.Y == 0);
		bool bBottom = (Cell.Y == Rows - 1);

		if ((bLeft && bTop) || (bLeft && bBottom) ||
			(bRight && bTop) || (bRight && bBottom))
		{
			MaxNeighbors = 3;
		}
		else if (bLeft || bRight || bTop || bBottom)
		{
			MaxNeighbors = 5;
		}

		return MaxNeighbors;
	}

protected:
	virtual void InitInternal(const T& InValue, const uint16 InNum)
	{
		Data.Init(InValue, InNum);
	}
	
	uint16 Columns = 0;
	uint16 Rows = 0;
	TArray<T> Data;
	
private:
	uint16 CoordinateToIndex(const FUintPoint& Coords) const
	{
		return Coords.Y * Columns + Coords.X;
	}
};

template<typename T>
struct TSharedArray2D : TArray2D<TSharedPtr<T>>
{
	using TArray2D<TSharedPtr<T>>::Data;

protected:
	virtual void InitInternal(const TSharedPtr<T>& InValue, const uint16 InNum) override
	{
		check(InValue.IsValid());
		const T& RawValue = *InValue.Get();
		
		Data.Reset(InNum);
		for (uint16 i = 0; i < InNum; ++i)
		{
			Data.Add(MakeShared<T>(RawValue));
		}
	}
};