// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterBase.h"
#include "TargetingSystem.generated.h"


UCLASS()
class PROJECT_BNS_API ATargetingSystem : public ACharacterBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ATargetingSystem();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Targetting
private:
	FVector2D ViewportSize = FVector2D::ZeroVector;
	float DpScale = 1.0f;
	/*
	UPROPERTY(VisibleAnywhere)
	TArray<class ITargetAble*> TargetAbles{};
	*/

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* TargetSensor = nullptr;

	// UPROPERTY(VisibleAnywhere)
	class ITargetAble* Target = nullptr;

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
