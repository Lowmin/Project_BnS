// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
 	void Bind(class UStatComponent* stat);
 	
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

	void OnTargetChange(bool isTarget, FVector2D center, FVector2D size) const;

	/// <summary>
	/// 스킬 쿨타임 표시 <br/>
	/// </summary>
	/// <param name="index"> 스킬 인덱스 <para/>
	/// ㄴ0 : 기본공격  <para/>
	/// ㄴ1 : 1번스킬
	/// </param>
	/// <param name="remain"> 남은 시간 </param>
	/// <param name="cooldown"> 총 쿨타임 시간 </param>
	void OnCooldownChange(int index, float remain, float cooldown);
};
