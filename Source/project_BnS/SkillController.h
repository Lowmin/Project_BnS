// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SkillBase.h"
#include "SkillInformation.h" 
#include "SkillController.generated.h"

UCLASS()
class PROJECT_BNS_API USkillController : public UObject
{
	GENERATED_BODY()

public:
	USkillController();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void Initialize(ACharacter* InInstigator, const FDataTableRowHandle& InDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryExecuteSkill(AActor* Target, FVector TargetLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USkillBase> SkillBaseClass;
	UPROPERTY()
	USkillBase* SkillBaseInstance;
	UPROPERTY()
	ACharacter* OwnerInstigator;

	float CooldownEndTime;

	void StartCooldown();
	bool IsCooldown() const;
	bool IsEnoughMP() const;

	FSkillInformation GetSkillInformation() const;
};