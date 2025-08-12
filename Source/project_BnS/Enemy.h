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

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UWorldFloatingUiComponent* HpBar = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* Nameplate = nullptr;

	// ITargetAble
	// FVector2D GetTargetCenter_Implementation();
	// FVector2D GetTargetSize_Implementation();
	virtual FVector GetWorldLocation() const override;
	virtual FVector2D GetTargetCenter() const override;
	virtual FVector2D GetTargetSize() const override;
};
