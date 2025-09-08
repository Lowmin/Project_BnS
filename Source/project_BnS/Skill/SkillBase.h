// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillInterface.h"
#include "SkillData.h"
#include "SkillBase.generated.h"

class UAnimMontage;
class UAnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEnded, ESkillSlot, Slot, bool, bInterrupted);

UCLASS()
class PROJECT_BNS_API ASkillBase : public AActor, public ISkillInterface
{
	GENERATED_BODY()

public:
	ASkillBase();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void InitFromRow(const FSkillDataRow& InRow);

	// 타겟
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillTarget(AActor* InTarget);

	UFUNCTION(BlueprintPure, Category = "Skill")
	AActor* GetSkillTarget() const;


	// 실행 / 취소
	virtual void ExecuteSkill_Implementation() override;
	virtual void CancelSkill_Implementation() override;

	virtual void OnSkillNotify_Hit() {}
	virtual void OnSkillNotify_Custom(FName NotifyName) {}		// 노티파이 이름으로 세팅 (ex. Fire, Leap 등)

	// 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Skill Events")
	FOnSkillEnded    OnEnded;


protected:
	// 노티
	virtual void OnSkillMontageEnded(bool bInterrupted) {}		// 몽타주 종료 후 정리

	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	ACharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	UAnimInstance* GetOwnerAnimInstance() const;

	// 대미지
	virtual int32 CalculateDamage() const;
	void ApplyDamageToCharacter(ACharacter* DamagedCharacter) const;

	// 타입 데이터 꺼내기
	const FSkillType_Melee* GetTypeData_Melee() const { return SavedTypeData.GetPtr<FSkillType_Melee>(); }
	const FSkillType_Projectile* GetTypeData_Projectile() const { return SavedTypeData.GetPtr<FSkillType_Projectile>(); }
	const FSkillType_Area* GetTypeData_Area() const { return SavedTypeData.GetPtr<FSkillType_Area>(); }


protected:
	// 내부 상태 및 데이터
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	int32 MySkillID = -1;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	ESkillSlot MySlot = ESkillSlot::None;

	UPROPERTY()
	TWeakObjectPtr<AActor> MyTarget;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MyMontage = nullptr;

	UPROPERTY()
	bool bIsExecuting = false;

	UPROPERTY()
	float MyBaseDamage = 1.f;

	UPROPERTY()
	FInstancedStruct SavedTypeData;		// 스킬 타입 저장

	// 스킬 이펙트
	UPROPERTY()
	TObjectPtr<UParticleSystem> MyCastVFX;
	UPROPERTY()
	TObjectPtr<USoundBase> MyCastSound;
	UPROPERTY()
	TObjectPtr<UParticleSystem> MyHitVFX;
	UPROPERTY()
	TObjectPtr<USoundBase> MyHitSound;

private:
	// 베이스에서 바인딩/라우팅/종료
	UFUNCTION()
	void OnAnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	// 몽타주 끝났을 때
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
