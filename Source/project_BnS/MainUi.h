// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUi.generated.h"


class UProgressBar;
class UTextBlock;
class UCanvasPanel;
class UCanvasPanelSlot;
class USkillIcon;

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UMainUi : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HpBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HpText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* StaminaBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* StaminaText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ExpBar = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* NicknameText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LevelText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* Target = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UCanvasPanelSlot* TargetSlot = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillIcon* Skill_0 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillIcon* Skill_1 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillIcon* Skill_2 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillIcon* Skill_3 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillIcon* Skill_4 = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void SetHp(float current, float max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMp(int32 current);

	UFUNCTION(BlueprintCallable)
	void SetStamina(float current, float max);

	UFUNCTION(BlueprintCallable)
	void SetStaminaEnable(bool enable);

	UFUNCTION(BlueprintCallable)
	void SetExp(float current, float max);

	UFUNCTION(BlueprintCallable)
	void SetNickname(const FString& nickname);

	UFUNCTION(BlueprintCallable)
	void SetLevel(int level);

	UFUNCTION(BlueprintCallable)
	void SetTarget(bool isTarget, FVector2D center, FVector2D size);

	/// <summary>
	/// 스킬 쿨타임 표시 <br/>
	/// </summary>
	/// <param name="index"> 스킬 인덱스 <para/>
	/// ㄴ0 : 기본공격  <para/>
	/// ㄴ1 : 1번스킬
	/// </param>
	/// <param name="remain"> 남은 시간 </param>
	/// <param name="cooldown"> 총 쿨타임 시간 </param>
	UFUNCTION()
	void SetSkillCooldown(int index, float remain, float cooldown);
};
