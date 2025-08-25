// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ProjectileSkill.generated.h"

class AProjectileBall;

UCLASS()
class PROJECT_BNS_API AProjectileSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteSkill_Implementation() override;
	virtual void OnSkillNotify_Custom(FName NotifyName) override;

private:
	void CalcSpawnTransform(FVector& OutPos, FRotator& OutRot) const;
	void SpawnProjectile(const struct FSkillType_Projectile& Type);

private:
	float OffsetForward = 50.f;
	float OffsetUp = 40.f;
};