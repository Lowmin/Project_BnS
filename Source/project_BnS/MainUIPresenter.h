// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class PROJECT_BNS_API MainUIPresenter
{
public:
	MainUIPresenter(class UMainUi* ui, class ACharacterBase* character);
	~MainUIPresenter();

	// variable
	UMainUi* mainUI = nullptr;
	ACharacterBase* character = nullptr;

	// Status
public:
	void OnHpChange(float current, float max) const;
	void OnMpChange(float current) const;
	void OnStanimaChange(float current, float max) const;

	void OnLevelChange(int level) const;
	void OnExpChange(float current, float max) const;
};
