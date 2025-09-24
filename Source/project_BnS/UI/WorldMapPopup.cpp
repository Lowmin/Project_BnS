// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMapPopup.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Minimap/MinimapBounds.h"
#include "Components/BoxComponent.h"
#include "../MyPlayer.h"

void UWorldMapPopup::SetWorldMap(AMinimapBounds* InBounds, AMyPlayer* InPlayer, UTexture2D* InWorldMapTex)
{
	Bounds = InBounds;
	MyPlayer = InPlayer;
	WorldMapTex = InWorldMapTex;

	if (Bounds.IsValid())
	{
		Bounds->GetMapBounds(MapWorldOrigin, MapWorldSize);
	}
}

void UWorldMapPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapImage && MapMaterial)
	{
		MapMID = UMaterialInstanceDynamic::Create(MapMaterial, this);
		MapImage->SetBrushFromMaterial(MapMID);

		if (MapMID && WorldMapTex)
		{
			MapMID->SetTextureParameterValue(FName("Texture"), WorldMapTex);
		}
		ApplyMapParametersToMaterial();
	}
}

void UWorldMapPopup::SetVisiblePopup(bool isVisible)
{
	Super::SetVisiblePopup(isVisible);

	if (isVisible)
	{
		SetMapAndMarkers();

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &UWorldMapPopup::SetMapAndMarkers, 0.1f, true);
		}
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(UpdateTimer);
		}
	}
}

FVector2D UWorldMapPopup::WorldToUV(const FVector& World) const
{
	if (MapWorldSize.X <= KINDA_SMALL_NUMBER || MapWorldSize.Y <= KINDA_SMALL_NUMBER)
	{
		return FVector2D(0.5f, 0.5f);
	}

	const float RelativeX = World.X - MapWorldOrigin.X;
	const float RelativeY = World.Y - MapWorldOrigin.Y;

	float U = 1.0f - (RelativeX / MapWorldSize.X);
	float V = 1.0f - (RelativeY / MapWorldSize.Y);

	U = FMath::Clamp(U, 0.0f, 1.0f);
	V = FMath::Clamp(V, 0.0f, 1.0f);

	return FVector2D(U, V);
}

void UWorldMapPopup::SetMapAndMarkers()
{
	if (!MyPlayer.IsValid() || !PlayerIcon) return;

	const float yaw = MyPlayer->GetActorRotation().Yaw - 90.f;
	PlayerIcon->SetRenderTransformAngle(yaw);

	ApplyMapParametersToMaterial();
}

FReply UWorldMapPopup::NativeOnMouseWheel(const FGeometry& InGeo, const FPointerEvent& InMouseEvent)
{
	const float delta = InMouseEvent.GetWheelDelta();
	Zoom = FMath::Clamp(Zoom + delta * 0.25f, MinZoom, MaxZoom);

	ApplyMapParametersToMaterial();

	return FReply::Handled();
}

void UWorldMapPopup::ApplyMapParametersToMaterial()
{
	if (MapMID && MyPlayer.IsValid())
	{
		MapMID->SetScalarParameterValue(FName("Zoom"), Zoom);

		const FVector2D PlayerUV = WorldToUV(MyPlayer->GetActorLocation());
		MapMID->SetVectorParameterValue(FName("PlayerPositionUV"), FLinearColor(PlayerUV.X, PlayerUV.Y, 0.f));
	}
}