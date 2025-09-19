// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdControlType.h"
#include "CrowdControlComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_Multi);
DECLARE_DELEGATE_TwoParams(FDele_Single_CC_Change, ECrowdControlType type, int32 count)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BNS_API UCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrowdControlComponent();

private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float Duration = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 Stack = 0;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 ActivateStackCount = 0;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	ECrowdControlType CurrentType = ECrowdControlType::None;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetActivateStackCount(int32 count);
	bool IsEffect() const;
	ECrowdControlType GetCrowdControlType() const;
	void ApplyCrowdControl(ECrowdControlType type, float duration);
	void RemoveCrowdControl();
	FDele_Multi OnAppliedCrowdControl;
	FDele_Multi OnRemovedCrowdControl;

	FDele_Single_CC_Change OnCCInfoChange;
	UFUNCTION(BlueprintPure, Category = "CrowdControl")
	int32 GetActivateStackCount() const;
	UFUNCTION(BlueprintPure, Category = "CrowdControl")
	int32 GetCurrentStack() const;
};
