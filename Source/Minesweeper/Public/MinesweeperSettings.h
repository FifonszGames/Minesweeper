// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MinesweeperSettings.generated.h"

USTRUCT()
struct FMinesweeperGameSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=4, ClampMax=25))
	uint16 Width = 6;
	UPROPERTY(EditAnywhere, meta=(ClampMin=4, ClampMax=25))
	uint16 Height = 6;
	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	uint16 NumberOfMines = 6;
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
