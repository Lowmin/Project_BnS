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

protected:
	void PerformDash();
	void EndDash();

	//UFUNCTION()
	//void OnDashHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FResult& Hit);


private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AWarningIndicator> WarningIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float DashSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float WarningDuration = 3.0f;

	UPROPERTY()
	TObjectPtr<AWarningIndicator> SpawndIndicator;
};
