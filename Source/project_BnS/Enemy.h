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

protected:
	UPROPERTY(EditAnywhere, Category = "AI Data")
	TObjectPtr<class UDataTable> EnemyDataTable;

	UPROPERTY(EditAnywhere, Category = "AI Data")
	FName EnemyRowName;

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
