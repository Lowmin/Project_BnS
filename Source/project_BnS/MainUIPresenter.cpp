// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIPresenter.h"

#include "MainUi.h"
#include "MyPlayer.h"

MainUIPresenter::MainUIPresenter(UMainUi* ui, AMyPlayer* character)
{
	mainUI = ui;
	this->character = character;
}

MainUIPresenter::~MainUIPresenter()
{
}

void MainUIPresenter::OnHpChange(float current, float max) const
{
	if (mainUI == nullptr)
		return;
	mainUI->SetHp(current, max);
}

void MainUIPresenter::OnMpChange(float current) const
{
	if (mainUI == nullptr)
		return;

	mainUI->SetMp(current);
}

void MainUIPresenter::OnStanimaChange(float current, float max) const
{
	if (mainUI == nullptr)
		return;

	mainUI->SetStamina(current, max);
}

void MainUIPresenter::OnNicknameChange(const FString& nickname) const
{
	if (mainUI == nullptr)
		return;

	mainUI->SetNickname(nickname);
}

void MainUIPresenter::OnLevelChange(int level) const
{
	if (mainUI == nullptr)
		return;

	mainUI->SetLevel(level);
}

void MainUIPresenter::OnExpChange(float current, float max) const
{
	if (mainUI == nullptr)
		return;

	mainUI->SetExp(current, max);
}
