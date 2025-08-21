// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

class FSlateStyleSet;
class ISlateStyle;

class FMinesweeperStyle
{
public:
	static void Initialize();
	static void Shutdown();
	
	static const ISlateStyle& Get();
	static FName GetName();
	static FName GetToolbarIconStyleName();
	
private:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> MinesweeperStyle;
};