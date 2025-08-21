// ﻿// Copyright FifonszGames 2025 All Rights Reserved.


#include "MinesweeperSettings.h"

const FMinesweeperGameSettings& UMinesweeperSettings::GetDefaultSettings()
{
	const UMinesweeperSettings* Settings = GetDefault<UMinesweeperSettings>();
	check(IsValid(Settings))
	return Settings->DefaultSettings;
}
