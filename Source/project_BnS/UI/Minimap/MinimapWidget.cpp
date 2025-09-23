// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MinimapImage)
	{
		MinimapMaterialInst = MinimapImage->GetDynamicMaterial();
	}
}

void UMinimapWidget::SetMinimap(const FVector2D& ClampedMapCenterUV, float CurrentZoom)
{
	if (MinimapMaterialInst)
	{
		MinimapMaterialInst->SetVectorParameterValue(FName("PlayerPositionUV"), FLinearColor(ClampedMapCenterUV.X, ClampedMapCenterUV.Y, 0.f, 0.f));
		MinimapMaterialInst->SetScalarParameterValue(FName("Zoom"), CurrentZoom);
	}
}
