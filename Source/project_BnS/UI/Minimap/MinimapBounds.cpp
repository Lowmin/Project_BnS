// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapBounds.h"
#include "Components/BoxComponent.h"

AMinimapBounds::AMinimapBounds()
{
    PrimaryActorTick.bCanEverTick = false;

    BoundaryBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundaryBox"));
    RootComponent = BoundaryBox;
}

void AMinimapBounds::GetMapBounds(FVector2D& OutWorldOrigin, FVector2D& OutWorldSize) const
{
    if (!BoundaryBox) return;

    const FVector Center = GetActorLocation();
    const FVector Extent = BoundaryBox->GetScaledBoxExtent();

    // Origin : Box의 가장 왼쪽, 가장 아래쪽
    OutWorldOrigin.X = Center.X - Extent.X;
    OutWorldOrigin.Y = Center.Y - Extent.Y;

    // Size : Box의 전체 X,Y
    OutWorldSize.X = Extent.X * 2.0f;
    OutWorldSize.Y = Extent.Y * 2.0f;
}
