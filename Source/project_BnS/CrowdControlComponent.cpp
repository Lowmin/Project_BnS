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

	// 합격기 스택 활성화 동시에 초기화
	CurrentType = ECrowdControlType::None;
	Stack = 0;
}

bool UCrowdControlComponent::IsEffect()
{
	return Stack >= ActivateStackCount && Duration > 0.f;
}

ECrowdControlType UCrowdControlComponent::GetCrowdControlType() const
{
	return CurrentType;
}

void UCrowdControlComponent::ApplyCrowdControl(ECrowdControlType type, float duration)
{
	// Boss의 패턴에서 SetAcivateStackCount 호출 후 StackCount를 초기화
	if (ActivateStackCount > 0)
	{
		if (Stack == 0)
		{
			CurrentType = type;
			Stack = 1;
			UE_LOG(LogTemp, Log, TEXT(" -> Logic: Starting new stack."));
		}
		else if (CurrentType == type)
		{
			Stack++;
			UE_LOG(LogTemp, Log, TEXT(" -> Logic: Incrementing stack."));
		}
		else
		{
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Stack is now: %d"), Stack);

		if (Stack >= ActivateStackCount)
		{
			RemoveCrowdControl();
			SetActivateStackCount(2);
			UE_LOG(LogTemp, Warning, TEXT("Stack is now: %d"), Stack);
			Duration = duration;
			OnAppliedCrowdControl.Broadcast();
		}
	}
	else 
	{
		CurrentType = type;
		Duration = duration;

		if (OnAppliedCrowdControl.IsBound())
		{
			OnAppliedCrowdControl.Broadcast();
		}
	}

	if (Duration > 0.f)
	{
		//RemoveCrowdControl();
	}

}

void UCrowdControlComponent::RemoveCrowdControl()
{
	// 초기화
	CurrentType = ECrowdControlType::None;
	Stack = 0;

	if (OnRemovedCrowdControl.IsBound())
	{
		OnRemovedCrowdControl.Broadcast();
	}
}

