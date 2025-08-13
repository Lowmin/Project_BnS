// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdControlComponent.h"

// Sets default values for this component's properties
UCrowdControlComponent::UCrowdControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCrowdControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Duration <= 0.f)
		return;

	Duration -= DeltaTime;

	if (Duration <= 0.f)
		RemoveCrowdControl();
}

void UCrowdControlComponent::SetActivateStackCount(int32 count)
{
	ActivateStackCount = count;
}

bool UCrowdControlComponent::IsEffect()
{
	return Stack >= ActivateStackCount && Duration > 0.f;
}

ECrowdControlType UCrowdControlComponent::GetCrowdControlType()
{
	return CurrentType;
}

void UCrowdControlComponent::ApplyCrowdControl(ECrowdControlType type, float duration)
{
	if (Duration > 0.f)
	{
		RemoveCrowdControl();
	}

	CurrentType = type;
	Duration = duration;

	if (OnAppliedCrowdControl.IsBound())
	{
		OnAppliedCrowdControl.Broadcast();
	}
}

void UCrowdControlComponent::RemoveCrowdControl()
{
	if (OnRemovedCrowdControl.IsBound())
	{
		OnRemovedCrowdControl.Broadcast();
	}
}

