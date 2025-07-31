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
	return curHp;
}

void UStatComponent::SetCurHp(float hp)
{
	curHp = hp;
}

float UStatComponent::GetMaxHp() const 
{
	return maxHp;
}

int UStatComponent::GetCurMp() const
{
	return curMp;
}

void UStatComponent::SetCurMp(float mp)
{
	curMp = mp;
}

int UStatComponent::GetMaxMp() const
{
	return maxMp;
}

int UStatComponent::GetLevel() const
{
	return level;
}

float UStatComponent::GetAtk() const
{
	return atk;
}

float UStatComponent::GetDef() const
{
	return def;
}
