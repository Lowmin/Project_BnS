// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UStatComponent::GetCurHp() const
{
	return CurHp;
}

void UStatComponent::SetCurHp(float hp)
{
	if(FMath::IsNearlyEqual(CurHp, hp))
		return;
	
	CurHp = FMath::Min(hp, GetMaxHp());

	if(OnHpChange.IsBound())
		OnHpChange.Broadcast(CurHp, MaxHp);
}

float UStatComponent::GetMaxHp() const 
{
	return MaxHp + ExtraMaxHp;
}

void UStatComponent::SetMaxHp(float hp)
{
	MaxHp = hp;
}

float UStatComponent::GetPureHp() const
{
	return MaxHp;
}

int32 UStatComponent::GetCurMp() const
{
	return CurMp;
}

void UStatComponent::SetCurMp(int32 mp)
{
	if(CurMp == mp)
		return;
		
	CurMp = mp;

	if(OnMpChange.IsBound())
		OnMpChange.Broadcast(CurMp);
}

int32 UStatComponent::GetMaxMp() const
{
	return MaxMp;
}

void UStatComponent::SetLevel(int32 level)
{
	if(Level == level)
		return;
	
	Level = level;
	
	if(OnLevelChange.IsBound())
		OnLevelChange.Broadcast(level);
}

int32 UStatComponent::GetLevel() const
{
	return Level;
}

float UStatComponent::GetAtk() const
{
	return Atk + ExtraAtk;
}

float UStatComponent::GetPureAtk() const
{
	return Atk;
}

void UStatComponent::SetAtk(float atk)
{
	Atk = atk;
}

float UStatComponent::GetDef() const
{
	return Def + ExtraDef;
}

void UStatComponent::SetDef(float def)
{
	Def = def;
}

float UStatComponent::GetPureDef() const
{
	return Def;
}

const FString& UStatComponent::GetCharacterName() const
{
	return CharacterName;
}

void UStatComponent::SetCharacterName(const FString name)
{
	CharacterName = name;
}

void UStatComponent::AddExtraMaxHp(float value)
{
	ExtraMaxHp += value;
	CurHp = FMath::Max(CurHp + value, 1);

	if (OnHpChange.IsBound())
	{
		OnHpChange.Broadcast(GetCurHp(), GetMaxHp());
	}
}

void UStatComponent::AddExtraAtk(float value)
{
	ExtraAtk += value;
}

void UStatComponent::AddExtraDef(float value)
{
	ExtraDef += value;
}