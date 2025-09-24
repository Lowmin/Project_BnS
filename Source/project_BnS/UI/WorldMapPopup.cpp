// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMapPopup.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
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
		BoundsTransform = Bounds->GetActorTransform();

		if (const UBoxComponent* Box = Bounds->FindComponentByClass<UBoxComponent>())
		{
			const FVector Extent = Box->GetScaledBoxExtent();
			BoundsHalf = FVector2D(Extent.X, Extent.Y);
		}
	}
}

void UWorldMapPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapImage && MapMaterial)
	{
		MapMaterialInst = UMaterialInstanceDynamic::Create(MapMaterial, this);
		MapImage->SetBrushFromMaterial(MapMaterialInst);

		if (MapMaterialInst && WorldMapTex)
		{
			MapMaterialInst->SetTextureParameterValue(FName("Texture"), WorldMapTex);
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &UWorldMapPopup::UpdatePlayerMarker, 0.1f, true);
	}
}

FVector2D UWorldMapPopup::WorldToUV(const FVector& World) const
{
	if (!Bounds.IsValid() || BoundsHalf.X <= KINDA_SMALL_NUMBER || BoundsHalf.Y <= KINDA_SMALL_NUMBER)
	{
		return FVector2D(0.5f, 0.5f);
	}

	const FVector Local = BoundsTransform.InverseTransformPosition(World);

	float U = (Local.X / (BoundsHalf.X * 2.f)) + 0.5f;
	float V = (Local.Y / (BoundsHalf.Y * 2.f)) + 0.5f;
	if (IsFlipV) V = 1.f - V;

	U = FMath::Clamp(U, 0.f, 1.f);
	V = FMath::Clamp(V, 0.f, 1.f);
	return FVector2D(U, V);
}

void UWorldMapPopup::UpdatePlayerMarker()
{
	if (!PlayerIcon || !MyPlayer.IsValid() || !MapImage) return;

	const FVector2D PlayerUV = WorldToUV(MyPlayer->GetActorLocation());
	const FVector2D MapWidgetSize = MapImage->GetCachedGeometry().GetLocalSize();
	const FVector2D IconAbsolutePosition = PlayerUV * MapWidgetSize;

	FWidgetTransform Transform = PlayerIcon->GetRenderTransform();
	Transform.Translation = IconAbsolutePosition + IconPositionOffset;
	PlayerIcon->SetRenderTransform(Transform);
}
