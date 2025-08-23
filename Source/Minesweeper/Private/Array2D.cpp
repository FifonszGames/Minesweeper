// Copyright FifonszGames 2025 All Rights Reserved.

#include "Array2D.h"

namespace Array2DUtils
{
	bool AreNeighbours(const FIntPoint& A, const FIntPoint& B)
	{
		const uint32 dx = FMath::Abs(A.X - B.X);
		const uint32 dy = FMath::Abs(A.Y - B.Y);
		return dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0);
	}	
}