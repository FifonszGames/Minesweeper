// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperGameWidget.h"
#include "IStructureDetailsView.h"
#include "PropertyEditorModule.h"

void SMinesweeperGameWidget::Construct(const FArguments& InArgs)
{
	const FMinesweeperGameSettings InitialSettings = InArgs._InitialSettings.IsSet() ? InArgs._InitialSettings.GetValue() : UMinesweeperSettings::GetDefaultSettings();

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.FillContentHeight(0.3f)
		[
			CreateSettingsView(InitialSettings)
		]
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.FillContentHeight(0.7f)
		[
			//TODO:: grid
			SNullWidget::NullWidget
		]
	];
}

TSharedRef<SWidget> SMinesweeperGameWidget::CreateSettingsView(const FMinesweeperGameSettings& InitialSettings)
{
	FDetailsViewArgs ViewArgs;
	{
		ViewArgs.bAllowSearch = false;
	}
	FPropertyEditorModule& PropertyEditor = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	Settings = MakeShared<TStructOnScope<FMinesweeperGameSettings>>(InitialSettings);
	TSharedRef<IStructureDetailsView> StructureView = PropertyEditor.CreateStructureDetailView(ViewArgs, {}, Settings, INVTEXT("Game Settings"));
	TSharedPtr<SWidget> ViewAsWidget = StructureView->GetWidget();
	check(ViewAsWidget.IsValid())
	return ViewAsWidget.ToSharedRef();
}
