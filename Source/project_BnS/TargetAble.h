// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetAble.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetAble : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_BNS_API ITargetAble
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// <summary>
	/// 현재 타겟 가능 여부 
	/// </summary>
	/// <returns> 현재 타겟 가능 여부 </returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsActiveTarget() const;

	/// <summary>
	/// 월드 좌표 
	/// </summary>
	/// <returns> 월드 좌표 </returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetWorldLocation() const;

	/// <summary>
	/// 타겟 상자 중심 오프셋 
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector2D GetTargetCenter() const;

	/// <summary>
	/// 타겟 상자 크기 
	/// </summary>
	/// <returns> 타겟 상자 크기 </returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector2D GetTargetBoxSize() const;

	/// <summary>
	/// 타겟 여부 설정 
	/// </summary>
	/// <param name="isTarget"></param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnTargeted(bool isTarget);
};
