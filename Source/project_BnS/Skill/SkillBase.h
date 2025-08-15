// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "SkillInterface.h"
#include "Curves/CurveFloat.h"
#include "SkillBase.generated.h"

struct FSkillCommonData;
struct FMeleeData;
class UAnimMontage;
class UAnimInstance;
class ACharacter;

UCLASS()
class PROJECT_BNS_API ASkillBase : public AActor, public ISkillInterface
{
	GENERATED_BODY()

public:
	ASkillBase();

	void InitSkill(const FDataTableRowHandle& InCommonHandle, const FDataTableRowHandle& InTypeHandle);

	virtual void InitSkillExecute_Implementation() override;
	virtual void ExecuteSkill_Implementation() override;
	virtual void CancelSkill_Implementation() override;

protected:
	UPROPERTY()
	FDataTableRowHandle CommonHandle;

	UPROPERTY()
	FDataTableRowHandle TypeHandle;

	UPROPERTY()
	bool bExecuting = false;

	const FSkillCommonData* GetCommonRow() const;
	UAnimMontage* LoadMontage() const;
	UFUNCTION(BlueprintCallable)
	ACharacter* GetOwnerCharacter() const;
	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY()
	UCurveFloat* DamageCurveCached = nullptr;

	int32 GetDamageAmount() const;
	void ApplyDamageToActor(AActor* Enemy) const;

	void ReadyCommonAsset();

public:
	UFUNCTION(BlueprintCallable)
	void SetSkillTarget(AActor* InTarget);

	UFUNCTION(BlueprintPure)
	AActor* GetSkillTarget() const;
};
