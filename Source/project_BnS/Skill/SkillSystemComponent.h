// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSystemComponent.generated.h"

class USkillController;
class UDataTable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USkillController* Controller = nullptr;


	// Combo
	UPROPERTY(EditAnywhere)
	int32 ComboMax = 3;

	int32 ComboStep = 0;
	FTimerHandle ComboTimer;
	bool CanInputNext = true;

	UPROPERTY(EditAnywhere)
	float ComboTimeLimit = 1.0f;

	void StartComboWindow(float TimeSec);
	void CloseComboWindow();

	// Target
	AActor* FindCurrentTarget() const;

public:
	USkillSystemComponent();
	// 실행 테스트
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

	// Input
	void HandleBasicAttack();
	UPROPERTY(EditAnywhere)
	FName ProjectileRowName = TEXT("Fireball_Skill");
	void UseProjectileSkill();

	void BlockNextInput() { CanInputNext = false; }
	void AllowNextInput(float Seconds) { StartComboWindow(Seconds); }

public:
	UPROPERTY(EditAnywhere)
	UDataTable* DT_SkillCommon = nullptr;

	UPROPERTY(EditAnywhere)
	UDataTable* DT_Melee = nullptr;

	UPROPERTY(EditAnywhere)
	UDataTable* DT_Projectile = nullptr;


};
