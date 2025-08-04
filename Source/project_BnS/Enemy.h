// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "TargetAble.h"
#include "Enemy.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_BNS_API AEnemy : public ACharacterBase, public ITargetAble
{
	GENERATED_BODY()

public:
	AEnemy();

private:

	// ITargetAble
	FVector2D GetTargetCenter_Implementation();
	FVector2D GetTargetSize_Implementation();
};
