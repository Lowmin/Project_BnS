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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Targetting
private:
	UPROPERTY()
	TArray<class ITargetAble*> TargetAbles{};
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetSensor = nullptr;

	// UPROPERTY(VisibleAnywhere)
	ITargetAble* Target = nullptr;

	// 현재 타겟 설정 
	void SetCurTarget();

	// 타겟 유효성 검사 
	void ValidateTarget();

public:
	bool IsTargetAble() const;
	ACharacterBase* GetTarget() const;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
