// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperGameWidget.h"
#include "IStructureDetailsView.h"
#include "PropertyEditorModule.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SCheckBox.h"

void SMinesweeperGameWidget::Construct(const FArguments& InArgs)
{
	const FMinesweeperGameSettings InitialSettings = InArgs._InitialSettings.IsSet() ? InArgs._InitialSettings.GetValue() : UMinesweeperSettings::GetDefaultSettings();
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.MaxHeight(150.f)
		[
			CreateSettingsView(InitialSettings)
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.Padding(FMargin(0.f, 8.f, 0.f, 0.f))
		[
			SNew(SBorder)
			.Padding(FMargin(4.f))
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(SettingsCheckbox, SCheckBox)
						.IsChecked(ECheckBoxState::Unchecked)
					]
					+SHorizontalBox::Slot()
					.Padding(FMargin(8.f, 0.f, 0.f, 0.f))
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(INVTEXT("Recreate grid when settings change"))
					]	
				]
				+SVerticalBox::Slot()
				.Padding(FMargin(0.f, 8.f, 0.f, 0.f))
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SButton)
					.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperGameWidget::OnRecreateClicked))
					.Text(INVTEXT("Recreate Grid"))
				]
			]
		]
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(16.f))
		[
			CreateGrid(InitialSettings)
		]
	];
}

TSharedRef<SWidget> SMinesweeperGameWidget::CreateSettingsView(const FMinesweeperGameSettings& InitialSettings)
{
	FDetailsViewArgs ViewArgs;
	{
		ViewArgs.bAllowSearch = false;
		ViewArgs.bShowScrollBar = true;
	}
	FPropertyEditorModule& PropertyEditor = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	Settings = MakeShared<TStructOnScope<FMinesweeperGameSettings>>(InitialSettings);
	TSharedRef<IStructureDetailsView> StructureView = PropertyEditor.CreateStructureDetailView(ViewArgs, {}, Settings, INVTEXT("Game Settings"));
	TSharedPtr<SWidget> ViewAsWidget = StructureView->GetWidget();
	check(ViewAsWidget.IsValid())
	return ViewAsWidget.ToSharedRef();
}

void SMinesweeperGameWidget::RecreateGrid(const FMinesweeperGameSettings& InSettings) const
{
	if (!Grid.IsValid())
	{
		return;
	}
	for (int32 i = 0; i < InSettings.Width; ++i)
	{
		for (int32 j = 0; j < InSettings.Height; ++j)
		{
			Grid->AddSlot(i,j)
			[
				SNew(STextBlock)
				.Text(FText::Format(INVTEXT("{0} {1}"), i, j))
				.Justification(ETextJustify::Type::Center)
			];
		}	
	}
}

TSharedRef<SWidget> SMinesweeperGameWidget::CreateGrid(const FMinesweeperGameSettings& InitialSettings)
{
	constexpr float MinSize = 20.f;
	TSharedRef<SUniformGridPanel> LocalGrid = SAssignNew(Grid, SUniformGridPanel)
	.SlotPadding(FMargin(2.0f))
	.MinDesiredSlotHeight(MinSize)
	.MinDesiredSlotWidth(MinSize);

	RecreateGrid(InitialSettings);
	return LocalGrid;
}

FReply SMinesweeperGameWidget::OnRecreateClicked() const
{
	if (Settings.IsValid())
	{
		if (const FMinesweeperGameSettings* CurrentSettings = Settings->Get())
		{
			RecreateGrid(*CurrentSettings);
		}
	}
	return FReply::Handled();
}
