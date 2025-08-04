// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SkillInformation.h"
#include "Castable.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT_BNS_API USkillBase : public UObject, public ICastable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDataTableRowHandle SkillInfoHandle;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	UPROPERTY()
	bool IsOnCooldown;

	void OnCooldownFinished();

public:
	USkillBase();

	virtual void RunSkill_Implementation(ACharacter* Instigator, AActor* Target, FVector TargetLocation) override;
	virtual bool CanRunSkill_Implementation(ACharacter* Instigator) override; 
	virtual float GetSkillCooldown_Implementation() override;
	virtual void SetSkillDataHandle_Implementation(const FDataTableRowHandle& InSkillInfoHandle) override;
	virtual FSkillInformation GetSkillInfo_Implementation() override;

};
