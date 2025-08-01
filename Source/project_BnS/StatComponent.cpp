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
	CurHp = hp;
}

float UStatComponent::GetMaxHp() const 
{
	return MaxHp;
}

int UStatComponent::GetCurMp() const
{
	return CurMp;
}

void UStatComponent::SetCurMp(float mp)
{
	CurMp = mp;
}

int UStatComponent::GetMaxMp() const
{
	return MaxMp;
}

int UStatComponent::GetLevel() const
{
	return Level;
}

float UStatComponent::GetAtk() const
{
	return Atk;
}

float UStatComponent::GetDef() const
{
	return Def;
}
