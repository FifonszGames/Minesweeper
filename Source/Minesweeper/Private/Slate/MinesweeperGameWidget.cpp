// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperGameWidget.h"
#include "IStructureDetailsView.h"
#include "PropertyEditorModule.h"
#include "Slate/MinesweeperCell.h"
#include "ViewModel/MinesweeperCellData.h"
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

void SMinesweeperGameWidget::RecreateGrid(const FMinesweeperGameSettings& InSettings)
{
	if (!Grid.IsValid())
	{
		return;
	}
	
	Cells.Init(MakeShared<MinesweeperCellData>(), InSettings.Width, InSettings.Height);
	Grid->ClearChildren();
	for (uint16 i = 0; i < InSettings.Width; ++i)
	{
		for (uint16 j = 0; j < InSettings.Height; ++j)
		{
			Grid->AddSlot(i,j)
			    .VAlign(VAlign_Fill)
			    .HAlign(HAlign_Fill)
			[
				SNew(SMinesweeperCell)
				.CellData(Cells.Get(i, j).ToSharedRef())
				.OnUnrevelaedCellClicked(FSimpleDelegate::CreateSP(this, &SMinesweeperGameWidget::UnrevelaedCellClicked ,i, j))
			];
		}	
	}
}

FReply SMinesweeperGameWidget::OnRecreateClicked()
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

void SMinesweeperGameWidget::UnrevelaedCellClicked(uint16 CellX, uint16 CellY)
{
	TSharedPtr<MinesweeperCellData> CellData = Cells.Get(CellX, CellY);
	check(CellData.IsValid())
	CellData->bIsRevealed.Set(true);
}
