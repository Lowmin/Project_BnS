// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../CrowdControlType.h"

#include "MainUIPresenter.generated.h"

/**
 * 
 */
 UCLASS()
class PROJECT_BNS_API AMainUIPresenter : public AActor
{
	GENERATED_BODY()

public:
	void SetMainUI(class UMainUi* ui);
 	void SetMyPlayer(class AMyPlayer* player);

 private:
 	void BindUI();
 	
	// variable
	UPROPERTY()
	UMainUi* MainUI = nullptr;
	UPROPERTY()
	AMyPlayer* MyPlayer = nullptr;

	// Status
public:
 	UFUNCTION()
	void OnHpChange(float current, float max) const;
 	UFUNCTION()
	void OnMpChange(int32 current) const;
 	UFUNCTION()
	void OnStaminaChange(float current, float max) const;
	void OnBattleChange(bool battle) const;

	void OnNicknameChange(const FString& nickname) const;
 	UFUNCTION()
	void OnLevelChange(int level) const;
 	UFUNCTION()
	void OnExpChange(float current, float max) const;

	void OnTargetChange(const FVector2D& center, const FVector2D& size) const;

	// Skill 

 	UFUNCTION()
 	void ChangeSkillIcon(int index, UTexture2D* prevTexture, UTexture2D* texture);
	UFUNCTION()
 	void ChangeSkillIconChain(int index, UTexture2D* prevTexture, UTexture2D* texture);
 	
	/// <summary>
	/// 스킬 아이콘 변경 
	/// </summary>
	/// <param name="index"> 스킬 인덱스 <para/>
	/// ㄴ0 : 기본공격  <para/>
	/// ㄴ1 : 1번스킬</param>
	/// <param name="texture"> 스킬 아이콘 텍스쳐 </param>
	UFUNCTION()
	void SetSkillIcon(int index, UTexture2D* texture);
	/// <summary>
	/// 스킬 쿨타임 표시
	/// </summary>
	/// <param name="index"> 스킬 인덱스 <para/>
	/// ㄴ0 : 기본공격  <para/>
	/// ㄴ1 : 1번스킬
	/// </param>
	/// <param name="remain"> 남은 시간 </param>
	/// <param name="cooldown"> 총 쿨타임 시간 </param>
	/// <param name="isVisibleNum"> 쿨타임 숫자 노출 여부 </param>
	UFUNCTION()
	void OnCooldownChange(int index, float remain, float cooldown, bool isVisibleNum);


	// BossInfo
	void OnBossInfoChange(class UStatComponent* status, int32 activateCCCount, float distance);
	void OnBossHpChange(float current, float max);
	void OnBossCCInfoChange(ECrowdControlType type, int32 count);
	void OnBossDistanceChange(float distance);
};
