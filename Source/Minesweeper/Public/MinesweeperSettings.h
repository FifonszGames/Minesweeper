// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MinesweeperSettings.generated.h"

USTRUCT()
struct FMinesweeperGameSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=2, ClampMax=50))
	int32 Width = 4;
	UPROPERTY(EditAnywhere, meta=(ClampMin=2, ClampMax=50))
	int32 Height = 4;
	//TODO:: adjust so that its always lower than width * height
	UPROPERTY(EditAnywhere, meta=(ClampMin=1, ClampMax=100))
	int32 NumberOfMines = 4;
};

UCLASS(Config=Editor, DefaultConfig)
class UMinesweeperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const FMinesweeperGameSettings& GetDefaultSettings();
	
private:
	UPROPERTY(Config, EditAnywhere)
	FMinesweeperGameSettings DefaultSettings;
};
