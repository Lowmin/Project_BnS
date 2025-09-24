// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CrowdControlType.h"
#include "project_BnS/Buff/BuffData.h"
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
	TObjectPtr<UProgressBar> HpBg = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> MpSlot = nullptr;

	TArray<TObjectPtr<class UMpIcon>> MpList;

	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StaminaText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UHoverProgressBar> ExpBar = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> NicknameText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> Target = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCanvasPanelSlot> TargetSlot = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<USkillIcon>> SkillIcons;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USkillIcon> Skill_0 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USkillIcon> Skill_1 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USkillIcon> Skill_2 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USkillIcon> Skill_3 = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USkillIcon> Skill_4 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UBossInfo> BossInfo = nullptr;

	// Minimap
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UMinimapWidget> MinimapView;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPlayerIconWidget> PlayerIconView;

	// buff
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBuffSlot> BuffSlot = nullptr;

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetHp(float current, float max);

	void HpEffect(float InDeltaTime);

	UFUNCTION(BlueprintCallable)
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
	void SetLevel(int32 level);

	UFUNCTION(BlueprintCallable)
	void SetTargetBox(FVector2D center, FVector2D size);

	UFUNCTION()
	void ChangeSkillIcon(int32 index, UTexture2D* prevTexture, UTexture2D* texture);
	
    UFUNCTION()
    void ChangeSkillIconChain(int32 index, UTexture2D* prevTexture, UTexture2D* texture);

	UFUNCTION()
	void SetSkillIcon(int32 index, UTexture2D* texture);
	/// <summary>
	/// 스킬 쿨타임 표시 <br/>
	/// </summary>
	/// <param name="index"> 스킬 인덱스 <para/>
	/// ㄴ0 : 기본공격  <para/>
	/// ㄴ1 : 1번스킬
	/// </param>
	/// <param name="remain"> 남은 시간 </param>
	/// <param name="cooldown"> 총 쿨타임 시간 </param>
	/// <param name="isVisibleNum"> 쿨타임 숫자 노출 여부  </param>
	UFUNCTION()
	void SetSkillCooldown(int32 index, float remain, float cooldown, bool isVisibleNum);
	
	UFUNCTION()
	void SetSkillUsable(int32 index, bool isUsable);

	// Boss Info
	void SetBossInfo(const class UStatComponent* status, int32 activateCCCount, float distance);
	void SetBossHp(float current, float max);
	void SetBossCrowdControlCount(int current, int max);
	void SetBossCCInfo(ECrowdControlType type, int32 count);
	void SetBossDistance(float distance);

	// Minimap
	UMinimapWidget* GetMinimapWidget() const;
	UPlayerIconWidget* GetPlayerIconWidget() const;

	// BUff
	void BuffStart(const FBuffData& data);
	void BuffFinish(const FBuffData& data);
	
};
