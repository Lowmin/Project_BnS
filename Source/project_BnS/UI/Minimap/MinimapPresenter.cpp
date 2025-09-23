// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapPresenter.h"
#include "../../MyPlayer.h"
#include "GameFramework/Controller.h"

UMinimapPresenter::UMinimapPresenter()
{
}

void UMinimapPresenter::Init(AMyPlayer* InPlayer, const FVector2D& InMapWorldOrigin, const FVector2D& InMapWorldSize, const FVector2D& InMinimapScreenSize)
{
	MyPlayer = InPlayer;
	MapWorldOrigin = InMapWorldOrigin;
	MapWorldSize = InMapWorldSize;
	MinimapScreenSize = InMinimapScreenSize;
}

void UMinimapPresenter::SetMinimap()
{
	if (!MyPlayer.IsValid()) return;
	SetMinimapData();
}

void UMinimapPresenter::SetMinimapZoom(float InZoom)
{
	MapZoom = FMath::Max(1.0f, InZoom);
}

FVector2D UMinimapPresenter::ConvertWorldToMinimap(const FVector& WorldLocation) const
{
	if (MapWorldSize.X <= KINDA_SMALL_NUMBER || MapWorldSize.Y <= KINDA_SMALL_NUMBER)
	{
		return FVector2D(0.5f, 0.5f);
	}

	const float RelativeX = WorldLocation.X - MapWorldOrigin.X;
	const float RelativeY = WorldLocation.Y - MapWorldOrigin.Y;

	float U = 1.0f - (RelativeX / MapWorldSize.X);
	float V = 1.0f - (RelativeY / MapWorldSize.Y);

	U = FMath::Clamp(U, 0.0f, 1.0f);
	V = FMath::Clamp(V, 0.0f, 1.0f);

	return FVector2D(U, V);
}

void UMinimapPresenter::SetMinimapData()
{
	if (AMyPlayer* Player = MyPlayer.Get())
	{
		const FVector2D PlayerUV = ConvertWorldToMinimap(Player->GetActorLocation());
		const FVector2D ClampUV = ClampMinimapZoom(PlayerUV, MapZoom);

		const float PlayerYaw = Player->GetActorRotation().Yaw;

		const FVector2D UVOffset = PlayerUV - ClampUV;
		const FVector2D ScreenOffset = UVOffset * MapZoom * MinimapScreenSize;

		OnMapPositionUpdated.Broadcast(ClampUV, MapZoom);
		OnPlayerRotationUpdated.Broadcast(PlayerYaw);
		OnPlayerIconOffsetUpdated.Broadcast(ScreenOffset);
	}
}

FVector2D UMinimapPresenter::ClampMinimapZoom(const FVector2D& InUV, float InZoom)
{
	const float HalfView = 0.5f / FMath::Max(1.f, InZoom);
	return FVector2D(FMath::Clamp(InUV.X, HalfView, 1.f - HalfView), FMath::Clamp(InUV.Y, HalfView, 1.f - HalfView));
}

void UMinimapPresenter::ClickMinimap()
{
	OnOpenMap.Broadcast();
}