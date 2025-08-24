// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

#include "Misc/Optional.h"
#include "Delegates/Delegate.h"

template <class T>
class TVMField
{
public:
	TVMField() = default;
	TVMField(const T Value) : Value(Value) {}

	T Get() const { return Value; }
	const T& GetRef() const { return Value; }
	
	void Set(const T NewValue)
	{
		if (NewValue == Value)
		{
			return;
		}
		Value = NewValue;
		OnChanged.ExecuteIfBound(Value);
	}
	
	operator T() const { return Get(); }
	
	TDelegate<void(const T)> OnChanged;
private:
	T Value;
};

#define BIND_VM_WITH_CALLBACK(Field, Callback) \
Callback(Field.Get()); \
Field.OnChanged.BindSP(this, &Callback);

class MinesweeperCellData
{
public:
	TVMField<bool> bIsRevealed = false; 
	TVMField<bool> bIsBomb = false;
	TVMField<TOptional<uint16>> AdjacentBombs; 
};