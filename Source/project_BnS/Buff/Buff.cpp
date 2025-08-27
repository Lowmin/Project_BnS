// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"

#include "../CharacterBase.h"

void UBuff::SetBuffData(const FBuffData* data)
{
	Data = data;
	RemainTime = data->Duration;
}

void UBuff::SetTarget(ACharacterBase* target)
{
	Target = target;
}

const FBuffData& UBuff::GetBuffData()const
{
	return *Data;
}
