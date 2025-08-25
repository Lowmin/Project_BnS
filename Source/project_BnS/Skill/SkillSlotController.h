// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillData.h"
#include "SkillSlotController.generated.h"

class UDataTable;
class UTexture2D;
class AActor;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotIconChanged, ESkillSlot, SkillSlot, UTexture2D*, NewIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotPairChanged, ESkillSlot, SkillSlot, UTexture2D*, CurIcon, UTexture2D*, NextIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotCooldownTick, ESkillSlot, SkillSlot, float, Remain, float, Total);

UCLASS()
class PROJECT_BNS_API USkillSlotController : public UObject
{
	GENERATED_BODY()

public:
	USkillSlotController();

	void Initialize(UDataTable* InTable, AActor* InOwner, UWorld* InWorld);

	// 베이스 스킬 (기본 아이콘 세팅)
	UFUNCTION(BlueprintCallable, Category = "Skill Slot")
	bool EquipBaseSkill(ESkillSlot SkillSlot, int32 SkillID);
	UFUNCTION(BlueprintCallable, Category = "Skill Slot")
	void ResetToBase(ESkillSlot SkillSlot);

	// 컴포넌트로부터 받은 지시 (해당 ID / 현재, 다음 ID)
	UFUNCTION(BlueprintCallable, Category = "Skill Slot")
	bool ShowSkill(ESkillSlot SkillSlot, int32 SkillID);
	UFUNCTION(BlueprintCallable, Category = "Skill Slot")
	bool ShowPairSkill(ESkillSlot SkillSlot, int32 CurSkillID, int32 NextSkillID);

	// 현재 표시중인 ID
	UFUNCTION(BlueprintCallable, Category = "Skill Slot")
	int32 GetCurrentSkillID(ESkillSlot SkillSlot) const;

	// UI 쿨타임
	void PlayCooldownShow(ESkillSlot SkillSlot, int32 SkillID, float EndAt, float TotalSec);
	void StopCooldownShow(ESkillSlot SkillSlot);
	void PlayCooldownShow_All(float EndAt, float TotalSec, bool bSkipSlotCool = true);	// GCD

	// UI 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnSlotIconChanged OnSlotIconChanged;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnSlotPairChanged OnSlotPairChanged;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnSlotCooldownTick  OnSlotCooldownTick;

private:
	const FSkillDataRow* FindRowByID(int32 SkillID) const;
	void OnCooldownShowTick(ESkillSlot SkillSlot);
	UWorld* GetWorldSafe() const;

	UPROPERTY()
	TObjectPtr<UDataTable> Table = nullptr;
	TWeakObjectPtr<AActor> Owner;

	// 슬롯 기억용
	UPROPERTY(Transient)
	TMap<int32, FName> RowNameByID;				// SkillID → RowName
	UPROPERTY(Transient)
	TMap<ESkillSlot, int32> BaseIDBySlot;		// 슬롯 → SkillID	(기본 슬롯)
	UPROPERTY(Transient)
	TMap<ESkillSlot, int32> CurIDBySlot;		// 슬롯 → SkillID	(현재 표시할 슬롯)

	// 쿨다운 (타이머와 목표 시각)
	UPROPERTY(Transient)
	TMap<ESkillSlot, FTimerHandle> ShowTickTimer;
	UPROPERTY(Transient)
	TMap<ESkillSlot, float> ShowEndAt;
	UPROPERTY(Transient)
	TMap<ESkillSlot, float> ShowTotal;

};
