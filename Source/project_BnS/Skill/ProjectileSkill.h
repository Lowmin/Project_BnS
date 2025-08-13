// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ProjectileSkill.generated.h"

struct FProjectileData;
class AProjectileBall;

UCLASS()
class PROJECT_BNS_API AProjectileSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteSkill_Implementation() override;

private:
	bool LoadProjectileData(FProjectileData& OutData);
	void SpawnProjectile(const FProjectileData& Data);

	float OffsetForward = 50.f;
	float OffsetUp = 40.f;

};
