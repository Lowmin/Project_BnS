// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"

void ABossEnemy::SetCCImmune(bool bImmune)
{
	bIsCCImmune = bImmune;
}

void ABossEnemy::CCApplied()
{
	if (bIsCCImmune) return;

	Super::CCApplied();
}

void ABossEnemy::CCRemoved()
{
	Super::CCRemoved();
}
