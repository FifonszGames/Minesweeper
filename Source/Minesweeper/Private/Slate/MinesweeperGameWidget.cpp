// Copyright FifonszGames 2025 All Rights Reserved.


#include "Slate/MinesweeperGameWidget.h"
#include "IStructureDetailsView.h"
#include "Minesweeper.h"
#include "MinesweeperGameInstance.h"
#include "PropertyEditorModule.h"
#include "Slate/MinesweeperCell.h"
#include "ViewModel/MinesweeperCellData.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SCheckBox.h"

namespace MinesweeperWidgetUtils
{
	TSharedRef<SWidget> CreateSettingsView(const TSharedRef<TStructOnScope<FMinesweeperGameSettings>>& InitialSettings)
	{
		FDetailsViewArgs ViewArgs;
		{
			ViewArgs.bAllowSearch = false;
			ViewArgs.bShowScrollBar = true;
		}
		FPropertyEditorModule& PropertyEditor = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>(MinesweeperUtils::PropertyEditorModuleName);
	
		TSharedRef<IStructureDetailsView> StructureView = PropertyEditor.CreateStructureDetailView(ViewArgs, {}, InitialSettings, INVTEXT("Game Settings"));
		TSharedPtr<SWidget> ViewAsWidget = StructureView->GetWidget();
		check(ViewAsWidget.IsValid())
		return ViewAsWidget.ToSharedRef();
	}
}

void SMinesweeperGameWidget::Construct(const FArguments& InArgs)
{
	const FMinesweeperGameSettings& InitialSettings = InArgs._InitialSettings.IsSet() ? InArgs._InitialSettings.GetValue() : UMinesweeperSettings::GetDefaultSettings();
	
	const TSharedRef<TStructOnScope<FMinesweeperGameSettings>> Settings = MakeShared<TStructOnScope<FMinesweeperGameSettings>>(InitialSettings);
	MinesweeperGame = MakeShared<FMinesweeperGameInstance>(Settings);
	MinesweeperGame->OnFinished.BindSP(this, &SMinesweeperGameWidget::OnGameFinished);

	constexpr float MinSlotSize = 20.f;
	TSharedRef<SUniformGridPanel> LocalGrid = SAssignNew(Grid, SUniformGridPanel)
	.SlotPadding(FMargin(2.0f))
	.MinDesiredSlotHeight(MinSlotSize)
	.MinDesiredSlotWidth(MinSlotSize);
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.MaxHeight(150.f)
		[
			MinesweeperWidgetUtils::CreateSettingsView(Settings)
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
					.Text(INVTEXT("Recreate Grid"))
					.OnClicked(FOnClicked::CreateSPLambda(this, [this]()
					{
						RecreateGridSlots();
						return FReply::Handled();
					}))
				]
			]
		]
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(16.f))
		[
			SAssignNew(RatioBox, SBox)
			[
				SNew(SBorder)
				.Padding(FMargin(16.f))
				[
					LocalGrid
				]
			]
		]
	];
	RecreateGridSlots();
}

void SMinesweeperGameWidget::RecreateGridSlots()
{
	if (!Grid.IsValid() || !MinesweeperGame.IsValid())
	{
		return;
	}
	Grid->ClearChildren();
	
	const FMinesweeperGameSettings& Settings = MinesweeperGame->GetSettings();
	const float Ratio = static_cast<float>(Settings.Width) / Settings.Height;
	RatioBox->SetMaxAspectRatio(Ratio);
	RatioBox->SetMinAspectRatio(Ratio);
	
	MinesweeperGame->Init();
	MinesweeperGame->GetCells().Foreach([this](const FUintPoint& Coords, const TSharedPtr<MinesweeperCellData>& Value)
	{
		Grid->AddSlot(Coords.X,Coords.Y)
		[
			SNew(SBox)
			.MaxAspectRatio(1.f)
			.MinAspectRatio(1.f)
			[
				SNew(SMinesweeperCell)
				.CellData(Value.ToSharedRef())
				.OnCellClicked(FSimpleDelegate::CreateSPLambda(this, [Coords, this]
				{
					if (MinesweeperGame.IsValid())
					{
						MinesweeperGame->CellSelected(Coords);
					}
				}))
			]
		];
	});
	Grid->SetEnabled(true);
}

void SMinesweeperGameWidget::OnGameFinished(const EGameEndResult Result)
{
	Grid->SetEnabled(false);
	FTimerHandle Handle;
	GEditor->GetTimerManager()->SetTimer(Handle, FTimerDelegate::CreateSPLambda(this, [Result, this]()
	{
		const EAppMsgCategory Category = Result == EGameEndResult::Success ? EAppMsgCategory::Success : EAppMsgCategory::Error;
		const FText Message = Result == EGameEndResult::Success ? INVTEXT("Congratulations, you won!") : INVTEXT("Unfortunately, you lost");
		FMessageDialog::Open(Category, EAppMsgType::Type::Ok, Message, INVTEXT("Game Finished"));
		RecreateGridSlots();
	}), 0.25f, false);
}
