// Copyright FifonszGames 2025 All Rights Reserved.

#pragma once

template <class T>
class TVMField
{
public:
	TVMField(const T Value) : Value(Value) {}

	T Get() const { return Value; }
	
	void Set(const T NewValue)
	{
		Value = NewValue;
		OnChanged.ExecuteIfBound(NewValue);
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
	TVMField<uint16> AdjacentBombs = 0; 
};