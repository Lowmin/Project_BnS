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

	// --- 초기화 ---
	// Controller가 데이터 핸들과 '미리 로드된' 에셋을 전달하도록 변경
	void InitializeSkill(
		const FDataTableRowHandle& InCommonHandle,
		const FDataTableRowHandle& InTypeHandle,
		UAnimMontage* PreloadedMontage,
		UCurveFloat* PreloadedDamageCurve
	);

	// --- 인터페이스 구현 ---
	virtual void ExecuteSkill_Implementation() override;
	virtual void CancelSkill_Implementation() override;

	// --- Public API ---
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

	// GetRow의 성공 여부 확인
	const FSkillCommonData* GetCommonData() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	ACharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 대미지 계산 및 적용
	virtual int32 CalculateDamage() const;
	// 함수 이름을 더 명확하게 변경
	void ApplyDamageToCharacter(ACharacter* DamagedCharacter) const;
};
