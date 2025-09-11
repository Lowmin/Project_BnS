// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SkillBase.h"
#include "../WarningIndicator.h"
#include "GruxBoss_Rush.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API AGruxBoss_Rush : public ASkillBase
{
	GENERATED_BODY()

public:
	AGruxBoss_Rush();
	virtual void ExecuteSkill_Implementation() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void PerformDash();
	void EndDash();

	UFUNCTION()
	void OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CancelSkill();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UAnimMontage> RushMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AWarningIndicator> WarningIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float DashSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float WarningDuration = 3.0f;

	UPROPERTY()
	TObjectPtr<AWarningIndicator> SpawndIndicator;

	FVector DashDirection;

	UPROPERTY()
	TSet<AActor*> HitActors;
};
