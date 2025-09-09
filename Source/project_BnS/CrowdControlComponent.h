// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdControlComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_Multi);

UENUM(BlueprintType)
enum class ECrowdControlType : uint8
{
	None,
	Down,
	Stun
};

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
	int32 ActivateStackCount = 1;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	ECrowdControlType CurrentType = ECrowdControlType::None;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetActivateStackCount(int32 count);
	bool IsEffect();
	ECrowdControlType GetCrowdControlType();
	void ApplyCrowdControl(ECrowdControlType type, float duration);
	void RemoveCrowdControl();
	FDele_Multi OnAppliedCrowdControl;
	FDele_Multi OnRemovedCrowdControl;
};
