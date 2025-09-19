// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECrowdControlType : uint8
{
	None,
	Down,
	Stun,
	Immune
};