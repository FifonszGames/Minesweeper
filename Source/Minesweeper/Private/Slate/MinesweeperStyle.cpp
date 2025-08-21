// Copyright FifonszGames 2025 All Rights Reserved.

#include "Slate/MinesweeperStyle.h"

#include "Minesweeper.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyle.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperStyle::MinesweeperStyle;

void FMinesweeperStyle::Initialize()
{
	if (!MinesweeperStyle.IsValid())
	{
		MinesweeperStyle = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MinesweeperStyle);
	}
}

void FMinesweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*MinesweeperStyle);
	ensure(MinesweeperStyle.IsUnique());
	MinesweeperStyle.Reset();
}

FName FMinesweeperStyle::GetName()
{
	return FName(TEXT("MinesweeperStyle"));
}

FName FMinesweeperStyle::GetToolbarIconStyleName()
{
	return FName(TEXT("Minesweeper.ToolbarIcon"));
}

TSharedRef<FSlateStyleSet> FMinesweeperStyle::Create()
{
	const FVector2D Icon20x20(20.0f, 20.0f);
	
	TSharedRef<FSlateStyleSet> Style = MakeShared<FSlateStyleSet>(GetName());
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(MinesweeperUtils::PluginName.ToString())->GetBaseDir() / TEXT("Resources"));

	Style->Set(GetToolbarIconStyleName(), new IMAGE_BRUSH_SVG(TEXT("ToolbarButtonIcon"), Icon20x20));

	return Style;
}

const ISlateStyle& FMinesweeperStyle::Get()
{
	check(MinesweeperStyle.IsValid())
	return *MinesweeperStyle;
}

#undef RootToContentDir