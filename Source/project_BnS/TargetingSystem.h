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
	FVector2D UiViewportSize = FVector2D::ZeroVector;
	FVector2D ViewportSize = FVector2D::ZeroVector;
	
	TArray<class ITargetAble*> TargetAbles{};
	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> TargetSensor = nullptr;

	// UPROPERTY(VisibleAnywhere)
	class ITargetAble* Target = nullptr;

	// 현재 타겟 설정 
	void SetCurTarget();

	// 타겟 유효성 검사 
	void ValidateTarget();

public:
	/// <summary>
	/// 현재 조준한 타겟 존재 여부 
	/// </summary>
	/// <returns> 타겟 존재 여부 </returns>
	bool IsTarget() const;
	/// <summary>
	/// 현재 조준한 타겟 
	/// </summary>
	/// <returns> 조준한 타겟  </returns>
	ACharacterBase* GetTarget() const;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
