// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UProjectileSkill : public USkillBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileSkill")
	float MaxRange = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileSkill")
	float SweepRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileSkill")
	float HitDamage = 10.f;

	virtual bool CanExecuteSkill_Implementation(ACharacter* Caster) override;
	virtual void ExecuteSkill_Implementation(ACharacter* Caster, AActor* Target, FVector TargetPos) override;

private:
	AActor* FindFirstEnemy(ACharacter* Caster, FHitResult& OutHit) const;
};
