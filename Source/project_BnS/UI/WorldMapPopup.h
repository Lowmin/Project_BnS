// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Popup.h"
#include "WorldMapPopup.generated.h"

class UImage;
class UOverlay;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class AMinimapBounds;
class AMyPlayer;

UCLASS()
class PROJECT_BNS_API UWorldMapPopup : public UPopup
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWorldMap(AMinimapBounds* InBounds, AMyPlayer* InPlayer, UTexture2D* InWorldMapTex);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>   MapImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> MarkerLayer = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>   PlayerIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TObjectPtr<UMaterialInterface> MapMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Map")
	bool IsFlipV = true;

	UPROPERTY(EditAnywhere, Category = "Map")
	FVector2D IconPositionOffset = FVector2D::ZeroVector;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MapMaterialInst = nullptr;
	UPROPERTY()
	TWeakObjectPtr<AMinimapBounds> Bounds;
	UPROPERTY()
	TWeakObjectPtr<AMyPlayer> MyPlayer;
	UPROPERTY()
	TObjectPtr<UTexture2D> WorldMapTex = nullptr;

	FTransform BoundsTransform;
	FVector2D  BoundsHalf;

	FTimerHandle UpdateTimer;

	FVector2D WorldToUV(const FVector& World) const;

	void UpdatePlayerMarker();
};
