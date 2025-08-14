// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIPresenter.h"

#include "MainUi.h"
#include "MyPlayer.h"
#include "StatComponent.h"

void AMainUIPresenter::SetMainUI(UMainUi* ui)
{
	MainUI = ui;
}


void AMainUIPresenter::SetMyPlayer(AMyPlayer* player)
{
	MyPlayer = player;
	
	Bind(MyPlayer->GetStatusComponent());
}

void AMainUIPresenter::Bind(UStatComponent* stat)
{
	MyPlayer->OnStaminaChange.BindUObject(this, &AMainUIPresenter::OnStaminaChange);
	MyPlayer->OnExpChange.BindUObject(this, &AMainUIPresenter::OnExpChange);
	
	stat->OnHpChange.AddUObject(this, &AMainUIPresenter::OnHpChange);
	stat->OnMpChange.AddUObject(this, &AMainUIPresenter::OnMpChange);
	stat->OnLevelChange.AddUObject(this, &AMainUIPresenter::OnLevelChange);
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

void AMainUIPresenter::OnTargetChange(bool isTarget, FVector2D center, FVector2D size) const
{
	if(MainUI == nullptr)
		return;

	MainUI->SetTarget(isTarget, center, size);
}

void AMainUIPresenter::SetSkillIcon(int index, UTexture2D* texture)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetSkillIcon(index, texture);
}

void AMainUIPresenter::OnCooldownChange(int index, float remain, float cooldown)
{
	if (MainUI == nullptr)
		return;

	MainUI->SetSkillCooldown(index, remain, cooldown);
}
