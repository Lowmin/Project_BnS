// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIPresenter.h"

#include "MainUi.h"
#include "MyPlayer.h"

MainUIPresenter::MainUIPresenter(UMainUi* ui, AMyPlayer* player)
{
	MainUI = ui;
	this->MyPlayer = player;
}

MainUIPresenter::~MainUIPresenter()
{
}

void MainUIPresenter::OnHpChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetHp(current, max);
}

void MainUIPresenter::OnMpChange(float current) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetMp(current);
}

void MainUIPresenter::OnStanimaChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetStamina(current, max);
}

void MainUIPresenter::OnBattleChange(bool battle) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetStaminaEnable(!battle);
}

void MainUIPresenter::OnNicknameChange(const FString& nickname) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetNickname(nickname);
}

void MainUIPresenter::OnLevelChange(int level) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetLevel(level);
}

void MainUIPresenter::OnExpChange(float current, float max) const
{
	if (MainUI == nullptr)
		return;

	MainUI->SetExp(current, max);
}
