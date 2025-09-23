// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterBase.h"
#include "TargetingSystem.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_Single_TargetBox, const FVector2D&, const FVector2D&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Single_TargetChanged, class ACharacterBase*, Target);


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
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;
	TObjectPtr<class ABnsController> BnsController;
	
	TArray<TObjectPtr<AActor>> TargetAbles{};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> TargetSensor = nullptr;

	// UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Target = nullptr;

	void OnViewportResized(FViewport* viewport, uint32 params);

	void SetTargetBox();

	// 타겟 해제
	void RemoveCurrentTarget();

	// 현재 타겟 설정 
	void SetCurTarget();

	// 타겟 유효성 검사 
	void ValidateTarget();

	// 대상과 캐릭터 사이 벽 검사 
	bool IsTargetBlocked(FVector targetPos);

public:
	FDele_Single_TargetBox OnTargetBoxChange;
	FDele_Single_TargetChanged OnTargetChanged;

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
