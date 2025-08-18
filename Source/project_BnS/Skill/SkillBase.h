// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "SkillInterface.h"
#include "SkillBase.generated.h"

class UAnimMontage;
class UCurveFloat;
class ACharacter;
class UAnimInstance;
struct FSkillCommonData;

UCLASS()
class PROJECT_BNS_API ASkillBase : public AActor, public ISkillInterface
{
	GENERATED_BODY()

public:
	ASkillBase();

	void InitializeSkill(
		const FDataTableRowHandle& InCommonHandle,
		const FDataTableRowHandle& InTypeHandle,
		UAnimMontage* PreloadedMontage,
		UCurveFloat* PreloadedDamageCurve
	);

	virtual void ExecuteSkill_Implementation() override;
	virtual void CancelSkill_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetSkillTarget(AActor* InTarget);

	UFUNCTION(BlueprintPure)
	AActor* GetSkillTarget() const;

protected:
	// 내부 상태 및 데이터
	UPROPERTY()
	FDataTableRowHandle CommonHandle;

	UPROPERTY()
	FDataTableRowHandle TypeHandle;

	UPROPERTY()
	bool bIsExecuting = false;

	UPROPERTY()
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DamageCurve;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	const FSkillCommonData* GetCommonData() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	ACharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual int32 CalculateDamage() const;
	void ApplyDamageToCharacter(ACharacter* DamagedCharacter) const;
};
