// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffPotion.h"

#include "../CharacterBase.h"
#include "../StatComponent.h"

void UBuffPotion::BuffTick(float deltaTime)
{
	if (!Target.IsValid()) return;

	ACharacterBase* character = Target.Get();
	if (!character || character->IsDead()) return;

	UStatComponent* stat = character->GetStatusComponent();
	if (!stat) return;

	const FBuffData& data = GetBuffData();
	LastTickTime += deltaTime;

	if (data.TickTime <= 0.0f) return;

	while (LastTickTime >= data.TickTime)
	{
		LastTickTime -= data.TickTime;

		stat->SetCurHp(stat->GetCurHp() + data.Value);

		if (character->IsDead())
		{
			RemainTime = 0.0f;
			LastTickTime = 0.0f;
			break;
		}
	}
}
