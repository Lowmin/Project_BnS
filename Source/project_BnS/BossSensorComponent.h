// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BossSensorComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_Single_BossHpCHange, float, float);
DECLARE_DELEGATE_TwoParams(FDele_Single_BossInfo, class UStatComponent*, float);
DECLARE_DELEGATE_OneParam(FDele_Single_BossDistance, float);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBossSensorComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UBossSensorComponent();

	// Boss Info
private:
	TWeakObjectPtr<class ABossEnemy> Target;
	TSet<class ABossEnemy*> BossList;

	void RemoveTargetBoss();
	void SetTargetBoss();
	void BossHpChange(float current, float max);

public:
	FDele_Single_BossInfo OnBossInfoChange;
	FDele_Single_BossHpCHange OnBossHpChange;
	FDele_Single_BossDistance OnBossDistanceChange;

	UFUNCTION()
	void OnBossOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBossOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
