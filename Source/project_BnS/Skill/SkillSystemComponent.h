// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSystemComponent.generated.h"

class USkillController;
class UDataTable;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillIconChanged, int32, SlotIndex, UTexture2D*, NewIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillCooldownTick, int32, SlotIndex, float, RemainTime, float, CooldownDuration);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillSystemComponent();

	// 플레이어에서 호출
	void HandleBasicAttack();
	void UseProjectileSkill();

	// UI 연결
	UPROPERTY(BlueprintAssignable)
	FOnSkillIconChanged OnSkillIconChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSkillCooldownTick OnSkillCooldownTick;

protected:
	virtual void BeginPlay() override;

	// 데이터
	UPROPERTY(EditAnywhere, Category = "Skill UI")
	TArray<TObjectPtr<UTexture2D>> BasicAttackIcons;

	UPROPERTY(EditAnywhere, Category = "Skill UI")
	int32 BasicAttackSlotIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Skill Data")
	FName ProjectileRowName = TEXT("Fireball_Skill");

	UPROPERTY(EditAnywhere, Category = "Skill Data")
	int32 ProjectileSlotIndex = 1;

	UPROPERTY(EditAnywhere, Category = "Skill Data")
	TObjectPtr<UDataTable> DT_SkillCommon;
	UPROPERTY(EditAnywhere, Category = "Skill Data")
	TObjectPtr<UDataTable> DT_Melee;
	UPROPERTY(EditAnywhere, Category = "Skill Data")
	TObjectPtr<UDataTable> DT_Projectile;

private:
	UPROPERTY()
	TObjectPtr<USkillController> Controller;

	// 콤보 상태
	int32 ComboStep = 0;
	int32 ComboMax = 3;
	bool bCanInputNext = true;
	FTimerHandle ComboTimerHandle;
	float ComboTimeLimit = 1.0f;

	void StartComboWindow();
	void CloseComboWindow();

	// UI 쿨다운
	struct FCooldownInfo
	{
		float EndTime = 0.f;
		float Duration = 0.f;
	};

	TMap<int32, FCooldownInfo> CooldownInfo;
	FTimerHandle CooldownUITimerHandle;
	void TickCooldownUI();

	AActor* FindCurrentTarget() const;
	// UI 이벤트
	void BroadcastSkillUI(const FName& SkillRowName, int32 SlotIndex);
};

