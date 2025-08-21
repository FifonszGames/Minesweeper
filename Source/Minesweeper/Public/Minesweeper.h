// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class SDockTab;
class FUICommandList;
class FSpawnTabArgs;

namespace MinesweeperUtils
{
	extern const FName PluginName;	
}

class FMinesweeperModule : public IModuleInterface
{
	
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void RegisterMenus();
	
	static void MinesweeperButtonClicked();
	static TSharedRef<SDockTab> SpawnMinesweeperTab(const FSpawnTabArgs& SpawnTabArgs);
	
	TSharedPtr<FUICommandList> PluginCommands;
};
