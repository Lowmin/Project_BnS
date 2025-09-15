// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MinimapPresenter.generated.h"


class AMyPlayer;
class UTextureRenderTarget2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapPositionUpdated, const FVector2D&, MapCenterUV);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRotationUpdated, float, PlayerRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenFullMap);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UMinimapPresenter : public UObject
{
	GENERATED_BODY()

public:
	UMinimapPresenter();

	void Init(AMyPlayer* InPlayer, const FVector2D& InMapWorldOrigin, const FVector2D& InMapWorldSize);
	void SetMinimap();

	UPROPERTY(BlueprintAssignable)
	FOnMapPositionUpdated OnMapPositionUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerRotationUpdated OnPlayerRotationUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnOpenFullMap OnOpenMap;

	UFUNCTION(BlueprintCallable)
	void ClickMinimap();

protected:
	UPROPERTY()
	TWeakObjectPtr<AMyPlayer> MyPlayer;

private:
	FVector2D ConvertWorldToMinimap(const FVector& WorldLocation) const;
	void SetMinimapData();

	FVector2D MapWorldOrigin; // 맵 텍스처의 좌상단에 해당하는 월드 좌표 (X, Y)
	FVector2D MapWorldSize;   // 맵 텍스처의 전체 월드 크기 (가로, 세로)
};
