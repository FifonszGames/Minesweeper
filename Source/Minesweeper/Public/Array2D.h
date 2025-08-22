// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
struct TArray2D
{
	void Init(const T& Value, const int32 InSizeX, const int32 InSizeY)
	{
		SizeX = InSizeX;
		SizeY = InSizeY;
		Data.Init(Value, SizeX * SizeY);
	}

	T& Get(const uint16 X, const uint16 Y)
	{
		check(!Data.IsEmpty())
		check(X < SizeX && Y < SizeY);
		return Data[SizeX * Y + X];
	}
private:
	uint16 SizeX = 0;
	uint16 SizeY = 0;
	TArray<T> Data;
};