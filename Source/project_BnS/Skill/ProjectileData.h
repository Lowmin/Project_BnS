// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectileData.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileLifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool GravityAffects = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ExplosionOnImpact = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 0.f;

	// ºÎµúÇûÀ» ¶§ ÀÌÆåÆ®
	// ºÎµúÇûÀ» ¶§ »ç¿îµå
};