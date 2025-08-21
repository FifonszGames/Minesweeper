// Copyright FifonszGames 2025 All Rights Reserved.

#include "Minesweeper.h"
#include "Slate/MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

namespace MinesweeperUtils
{
	const FName PluginName(TEXT("Minesweeper"));	
}

void FMinesweeperModule::StartupModule()
{
	FMinesweeperStyle::Initialize();
	FMinesweeperCommands::Register();
	
	PluginCommands = MakeShared<FUICommandList>();
	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenMinesweeperWindow,
		FExecuteAction::CreateStatic(&FMinesweeperModule::MinesweeperButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperUtils::PluginName, FOnSpawnTab::CreateStatic(&FMinesweeperModule::SpawnMinesweeperTab))
		.SetDisplayName(FText::FromName(MinesweeperUtils::PluginName))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FMinesweeperStyle::Shutdown();

	FMinesweeperCommands::Unregister();
	
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperUtils::PluginName);
}

TSharedRef<SDockTab> FMinesweeperModule::SpawnMinesweeperTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::GetEmpty())
			]
		];
}

void FMinesweeperModule::MinesweeperButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperUtils::PluginName);
}

void FMinesweeperModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = ToolbarMenu->FindOrAddSection(MinesweeperUtils::PluginName);
	FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenMinesweeperWindow,
		{}, {}, FSlateIcon(FMinesweeperStyle::GetName(), FMinesweeperStyle::GetToolbarIconStyleName())));
	Entry.SetCommandList(PluginCommands);
}
	
IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)