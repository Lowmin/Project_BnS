// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SkillInformation.h" 
#include "SkillController.generated.h"

class USkillBase;
class ACharacterBase;

UCLASS()
class PROJECT_BNS_API USkillController : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(TObjectPtr<UDataTable> InSkillDataTable, TObjectPtr<ACharacterBase> InOwnerCharacter);

	// 기본 공격 콤보용
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void RunBasicAttack();

	// 스킬
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void RunSkillID(int32 SkillID, AActor* Target = nullptr, FVector TargetLocation = FVector::ZeroVector);

protected:
	void ComboReset();

private:
	UPROPERTY(VisibleAnywhere) 
		TObjectPtr<UDataTable> SkillDataTable;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<ACharacterBase> OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
		int32 CurrentComboSkillID = 101;

	UPROPERTY(VisibleAnywhere) 
		FTimerHandle ComboTimerHandle;

};