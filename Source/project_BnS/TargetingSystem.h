// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "TargetingSystem.generated.h"


class ACharacterBase;

UCLASS()
class PROJECT_BNS_API UTargetingSystem : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingSystem();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Targetting
private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetSensor = nullptr;

	UPROPERTY(VisibleAnywhere)
	ACharacterBase* Target = nullptr;

public:
	ACharacterBase* GetTarget() const;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
