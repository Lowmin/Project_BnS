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

void UMinimapWidget::SetMapPosition(const FVector2D& MapCenterUV)
{
	if (MinimapMaterialInst)
	{
		MinimapMaterialInst->SetVectorParameterValue(FName("PlayerPositionUV"), FLinearColor(MapCenterUV.X, MapCenterUV.Y, 0.f, 0.f));
	}
}