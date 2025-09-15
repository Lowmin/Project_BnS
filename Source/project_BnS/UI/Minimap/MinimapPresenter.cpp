// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapPresenter.h"
#include "../../MyPlayer.h"
#include "GameFramework/Controller.h"

UMinimapPresenter::UMinimapPresenter()
{
}

void UMinimapPresenter::Init(AMyPlayer* InPlayer, const FVector2D& InMapWorldOrigin, const FVector2D& InMapWorldSize)
{
	MyPlayer = InPlayer;
	MapWorldOrigin = InMapWorldOrigin;
	MapWorldSize = InMapWorldSize;
}

void UMinimapPresenter::SetMinimap()
{
	if (!MyPlayer.IsValid()) return;
	SetMinimapData();
}

FVector2D UMinimapPresenter::ConvertWorldToMinimap(const FVector& WorldLocation) const
{
	if (MapWorldSize.X <= KINDA_SMALL_NUMBER || MapWorldSize.Y <= KINDA_SMALL_NUMBER)
	{
		return FVector2D(0.5f, 0.5f);
	}

	const float RelativeX = WorldLocation.X - MapWorldOrigin.X;
	const float RelativeY = WorldLocation.Y - MapWorldOrigin.Y;

	float U = (RelativeX / MapWorldSize.X);
	float V = (RelativeY / MapWorldSize.Y);

	U = FMath::Clamp(U, 0.0f, 1.0f);
	V = FMath::Clamp(V, 0.0f, 1.0f);

	return FVector2D(U, V);
}

void UMinimapPresenter::SetMinimapData()
{
	if (AMyPlayer* Player = MyPlayer.Get())
	{
		const FVector2D PlayerUV = ConvertWorldToMinimap(Player->GetActorLocation());
		const float PlayerYaw = Player->GetActorRotation().Yaw;

		OnMapPositionUpdated.Broadcast(PlayerUV);
		OnPlayerRotationUpdated.Broadcast(PlayerYaw);
	}
}

void UMinimapPresenter::ClickMinimap()
{
	OnOpenMap.Broadcast();
}