// Copyright FifonszGames 2025 All Rights Reserved.


#include "Customization/MinesweeperGameSettingsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "MinesweeperSettings.h"
#include "Widgets/Input/SNumericEntryBox.h"

namespace HandleUtils
{
	uint16 GetValue(const TSharedPtr<IPropertyHandle>& Handle)
	{
		uint16 Value = 0;
		if (Handle.IsValid())
		{
			Handle->GetValue(Value);
		}
		return Value;
	}
}


void FMinesweeperGameSettingsCustomizationBase::Init(TSharedRef<IPropertyHandle> InWidthProperty, TSharedRef<IPropertyHandle> InHeightProperty, TSharedRef<IPropertyHandle> InBombsHandle, FDetailWidgetRow& MineWidgetRow)
{
	WidthHandle = InWidthProperty;
	HeightHandle = InHeightProperty;
	MinesHandle = InBombsHandle;

	InWidthProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this, &FMinesweeperGameSettingsCustomizationBase::OnSizePropertyChanged));
	HeightHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this, &FMinesweeperGameSettingsCustomizationBase::OnSizePropertyChanged));
	
	MineWidgetRow
	.NameContent()
	[
		InBombsHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		SNew(SNumericEntryBox<uint16>)
		.AllowSpin(true)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		.ToolTipText(INVTEXT("Number of mines to place"))
		.MinValue(1)
		.MaxValue_Raw(this, &FMinesweeperGameSettingsCustomizationBase::GetMaxValue)
		.MaxSliderValue_Raw(this, &FMinesweeperGameSettingsCustomizationBase::GetMaxValue)
		.Value_Lambda([HandlePtr = InBombsHandle]() { return HandleUtils::GetValue(HandlePtr); })
		.OnValueCommitted_Lambda([this](uint16 InValue, ETextCommit::Type CommitType){ SetMinesAmount(InValue); })
		.OnValueChanged_Raw(this, &FMinesweeperGameSettingsCustomizationBase::SetMinesAmount)
	];
}

TOptional<uint16> FMinesweeperGameSettingsCustomizationBase::GetMaxValue() const
{
	return HandleUtils::GetValue(WidthHandle) * HandleUtils::GetValue(HeightHandle) - 1;
}

void FMinesweeperGameSettingsCustomizationBase::SetMinesAmount(uint16 InValue) const
{
	if (HandleUtils::GetValue(MinesHandle) != InValue)
	{
		MinesHandle->SetValue(InValue);
	}
}

void FMinesweeperGameSettingsCustomizationBase::OnSizePropertyChanged() const
{
	TOptional<uint16> MaxValue = GetMaxValue();
	if (MaxValue.IsSet() && HandleUtils::GetValue(MinesHandle) > MaxValue.GetValue())
	{
		SetMinesAmount(MaxValue.GetValue() - 1);
	}
}

TSharedRef<IPropertyTypeCustomization> FMinesweeperGameSettingsPropertyCustomization::MakeInstance()
{
	return MakeShared<FMinesweeperGameSettingsPropertyCustomization>();
}

void FMinesweeperGameSettingsPropertyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TSharedRef<IPropertyHandle> LocalWidthHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, Width)).ToSharedRef();
	ChildBuilder.AddProperty(LocalWidthHandle);

	TSharedRef<IPropertyHandle> LocalHeightHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, Height)).ToSharedRef();
	ChildBuilder.AddProperty(LocalHeightHandle);

	TSharedRef<IPropertyHandle> LocalMinesHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, NumberOfMines)).ToSharedRef();
	Init(LocalWidthHandle, LocalHeightHandle, LocalMinesHandle, ChildBuilder.AddCustomRow(LocalMinesHandle->GetPropertyDisplayName()));
}

TSharedRef<IDetailCustomization> FMinesweeperGameSettingsClassCustomization::MakeInstance()
{
	return MakeShared<FMinesweeperGameSettingsClassCustomization>();
}

void FMinesweeperGameSettingsClassCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> LocalWidthHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, Width));
	DetailBuilder.HideProperty(LocalWidthHandle);
	TSharedRef<IPropertyHandle> LocalHeightHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, Height));
	DetailBuilder.HideProperty(LocalHeightHandle);
	TSharedRef<IPropertyHandle> LocalMinesHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(FMinesweeperGameSettings, NumberOfMines));
	DetailBuilder.HideProperty(LocalMinesHandle);

	DetailBuilder.AddPropertyToCategory(LocalWidthHandle);
	DetailBuilder.AddPropertyToCategory(LocalHeightHandle);

	Init(LocalWidthHandle, LocalHeightHandle, LocalMinesHandle, DetailBuilder.AddPropertyToCategory(LocalMinesHandle).CustomWidget());
}
