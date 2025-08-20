// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "TargetAble.h"
#include "Enemy.generated.h"


/**
 * 
 */
class USphereComponent;

UCLASS()
class PROJECT_BNS_API AEnemy : public ACharacterBase, public ITargetAble
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void BeginPlay() override;

	virtual void OnDamaged(int32 damage) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI Data")
	TObjectPtr<class UDataTable> EnemyDataTable;

	UPROPERTY(EditAnywhere, Category = "AI Data")
	FName EnemyRowName;

	UPROPERTY(EditAnywhere, Category = "AI Data")
	TObjectPtr<USphereComponent> AttackRangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> HitReactMontage;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWorldFloatingUiComponent> HpBar = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> Nameplate = nullptr;
	FText Name;

	// ITargetAble
	bool IsActiveTarget_Implementation() const;
	FVector GetWorldLocation_Implementation() const;
	FVector2D GetTargetCenter_Implementation() const;
	FVector2D GetTargetBoxSize_Implementation() const;
	void OnTargeted_Implementation(bool isTarget);
};
