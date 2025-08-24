// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

class FMinesweeperGameSettingsCustomizationBase
{
protected:
	void Init(TSharedRef<IPropertyHandle> InWidthProperty, TSharedRef<IPropertyHandle> InHeightProperty, TSharedRef<IPropertyHandle> InBombsHandle, FDetailWidgetRow& MineWidgetRow);

private:
	TOptional<uint16> GetMaxValue() const;
	void SetMinesAmount(uint16 InValue) const;
	
	void OnSizePropertyChanged() const;
	
	TSharedPtr<IPropertyHandle> WidthHandle;
	TSharedPtr<IPropertyHandle> HeightHandle;
	TSharedPtr<IPropertyHandle> MinesHandle;
};

class FMinesweeperGameSettingsPropertyCustomization : public IPropertyTypeCustomization, FMinesweeperGameSettingsCustomizationBase
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override {}
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
};

class FMinesweeperGameSettingsClassCustomization : public IDetailCustomization, FMinesweeperGameSettingsCustomizationBase
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};