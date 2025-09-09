// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class E_AIState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Attacking	UMETA(DisplayName = "Attack")
};
