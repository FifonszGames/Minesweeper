// Copyright FifonszGames 2025 All Rights Reserved.

#include "Minesweeper.h"
#include "Slate/MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "PropertyEditorModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "ToolMenus.h"
#include "Customization/MinesweeperGameSettingsCustomization.h"
#include "Slate/MinesweeperGameWidget.h"

namespace MinesweeperUtils
{
	const FName PluginName(TEXT("Minesweeper"));
	const FName PropertyEditorModuleName(TEXT("PropertyEditor"));
}

void FMinesweeperModule::StartupModule()
{
	FMinesweeperStyle::Initialize();
	FMinesweeperCommands::Register();
	
	PluginCommands = MakeShared<FUICommandList>();
	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenMinesweeperWindow,
		FExecuteAction::CreateStatic(&FMinesweeperModule::MinesweeperButtonClicked));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperUtils::PluginName, FOnSpawnTab::CreateStatic(&FMinesweeperModule::SpawnMinesweeperTab))
		.SetDisplayName(FText::FromName(MinesweeperUtils::PluginName))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FPropertyEditorModule& PropertyEditor = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>(MinesweeperUtils::PropertyEditorModuleName);
	PropertyEditor.RegisterCustomPropertyTypeLayout(FMinesweeperGameSettings::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMinesweeperGameSettingsPropertyCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(FMinesweeperGameSettings::StaticStruct()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FMinesweeperGameSettingsClassCustomization::MakeInstance));
}

void FMinesweeperModule::ShutdownModule()
{
	if (FPropertyEditorModule* PropertyEditor = FModuleManager::Get().LoadModulePtr<FPropertyEditorModule>(MinesweeperUtils::PropertyEditorModuleName))
	{
		PropertyEditor->UnregisterCustomPropertyTypeLayout(FMinesweeperGameSettings::StaticStruct()->GetFName());
		PropertyEditor->UnregisterCustomClassLayout(FMinesweeperGameSettings::StaticStruct()->GetFName());
	}
		
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperUtils::PluginName);
	
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	
	FMinesweeperCommands::Unregister();
	FMinesweeperStyle::Shutdown();
}

TSharedRef<SDockTab> FMinesweeperModule::SpawnMinesweeperTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SMinesweeperGameWidget)
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