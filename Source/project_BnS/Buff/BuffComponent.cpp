// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"

#include "Buff.h"
#include "BuffTick.h"

// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BuffTick(DeltaTime);
}

void UBuffComponent::BuffTick(float deltaTime)
{
	for (IBuffTick* buff : BuffTickList)
	{

	}
}

void UBuffComponent::AddBuff(int32 buffIdx)
{
	UBuff* buff = NewObject<UBuff>(this);
	BuffList.Add(buff);

	IBuffTick* buffTick = Cast<IBuffTick>(buff);
	if (buffTick)
	{
		BuffTickList.push_back(buffTick);
	}
}

bool UBuffComponent::IsBuff(int32 buffIdx)
{
	for (UBuff* buff : BuffList)
	{
		if (buff->GetBuffData().Idx == buffIdx)
			return true;
	}

	return false;
}

