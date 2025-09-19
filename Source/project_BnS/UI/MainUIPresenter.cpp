// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIPresenter.h"

#include "MainUi.h"
#include "../MyPlayer.h"
#include "../StatComponent.h"
#include "../Skill/SkillSystemComponent.h"
#include "../BossSensorComponent.h"

void AMainUIPresenter::SetMainUI(UMainUi* ui)
{
	MainUI = ui;
}


void AMainUIPresenter::SetMyPlayer(AMyPlayer* player)
{
	MyPlayer = player;
	
	BindUI();
}

void AMainUIPresenter::BindUI()
{
	if (MyPlayer == nullptr)
		return;

	// 스킬 바인드 
	if (USkillSystemComponent* Skill = MyPlayer->GetSkillSystemComponent())
	{
		Skill->UI_OnSetIcon.AddDynamic(this, &AMainUIPresenter::SetSkillIcon);
		Skill->UI_OnSetIconStep.AddDynamic(this, &AMainUIPresenter::ChangeSkillIconChain);
		Skill->UI_OnCooldownTick.AddDynamic(this, &AMainUIPresenter::OnCooldownChange);
		Skill->UI_OnAnimatedSetIcon.AddDynamic(this, &AMainUIPresenter::ChangeSkillIcon);
	}

	// 보스 UI 바인드 
	if (UBossSensorComponent* BossSensor = MyPlayer->GetBossSensorComponent())
	{
		BossSensor->OnBossInfoChange.BindUObject(this, &AMainUIPresenter::OnBossInfoChange);
		BossSensor->OnBossHpChange.BindUObject(this, &AMainUIPresenter::OnBossHpChange);
		BossSensor->OnBossCCInfoChange.BindUObject(this, &AMainUIPresenter::OnBossCCInfoChange);
		BossSensor->OnBossDistanceChange.BindUObject(this, &AMainUIPresenter::OnBossDistanceChange);
	}

	// 타겟 박스 바인드 
	MyPlayer->OnTargetBoxChange.BindUObject(this, &AMainUIPresenter::OnTargetChange);

	// 스테이터스 바인드 
	if (UStatComponent* stat = MyPlayer->GetStatusComponent())
	{
		stat->OnHpChange.AddUObject(this, &AMainUIPresenter::OnHpChange);
		stat->OnMpChange.AddUObject(this, &AMainUIPresenter::OnMpChange);
		stat->OnLevelChange.AddUObject(this, &AMainUIPresenter::OnLevelChange);
	}
	MyPlayer->OnStaminaChange.BindUObject(this, &AMainUIPresenter::OnStaminaChange);
	MyPlayer->OnExpChange.BindUObject(this, &AMainUIPresenter::OnExpChange);
}

void AMainUIPresenter::OnHpChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetHp(current, max);
}

void AMainUIPresenter::OnMpChange(int32 current) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetMp(current);
}

void AMainUIPresenter::OnStaminaChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetStamina(current, max);
}

void AMainUIPresenter::OnBattleChange(bool battle) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetStaminaEnable(!battle);
}

void AMainUIPresenter::OnNicknameChange(const FString& nickname) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetNickname(nickname);
}

void AMainUIPresenter::OnLevelChange(int level) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetLevel(level);
}

void AMainUIPresenter::OnExpChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetExp(current, max);
}

void AMainUIPresenter::OnTargetChange(const FVector2D& center, const FVector2D& size) const
{
	if(MainUI == nullptr)
		return;

	MainUI->SetTargetBox(center, size);
}

void AMainUIPresenter::ChangeSkillIcon(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
	if(MainUI == nullptr)
		return;

	MainUI->ChangeSkillIcon(index, prevTexture, texture);
}

void AMainUIPresenter::ChangeSkillIconChain(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
	if(MainUI == nullptr)
		return;

	MainUI->ChangeSkillIconChain(index, prevTexture, texture);
}

void AMainUIPresenter::SetSkillIcon(int index, UTexture2D* texture)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetSkillIcon(index, texture);
}

void AMainUIPresenter::OnCooldownChange(int index, float remain, float cooldown, bool isVisibleNum)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetSkillCooldown(index, remain, cooldown, isVisibleNum);
}

void AMainUIPresenter::OnBossInfoChange(UStatComponent* status, int32 activateCCCount, float distance)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetBossInfo(status, activateCCCount, distance);
}

void AMainUIPresenter::OnBossHpChange(float current, float max)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetBossHp(current, max);
}

void AMainUIPresenter::OnBossCCInfoChange(ECrowdControlType type, int32 count)
{
	if (MainUI == nullptr)
		return;
	MainUI->SetBossCCInfo(type, count);
}

void AMainUIPresenter::OnBossDistanceChange(float distance)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetBossDistance(distance);
}
