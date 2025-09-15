// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIconWidget.h"
#include "Components/Image.h"

void UPlayerIconWidget::UpdateRotation(float PlayerTargetAngle)
{
	TargetRotationAngle = PlayerTargetAngle - 90.0f;
}

void UPlayerIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn)
	{
		CurrentRotationAngle = PlayerPawn->GetControlRotation().Yaw;
		TargetRotationAngle = CurrentRotationAngle;
		if (PlayerIconImage)
		{
			PlayerIconImage->SetRenderTransformAngle(CurrentRotationAngle);
		}
	}
}

void UPlayerIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (FMath::IsNearlyEqual(CurrentRotationAngle, TargetRotationAngle, 0.5f)) return;

	const float AngleDifference = FMath::FindDeltaAngleDegrees(CurrentRotationAngle, TargetRotationAngle);

	float RotationStep = AngleDifference * RotationSpeed * InDeltaTime;
	CurrentRotationAngle += RotationStep;

	if (CurrentRotationAngle >= 360.0f)
	{
		CurrentRotationAngle -= 360.0f;
	}
	else if (CurrentRotationAngle < 0.0f)
	{
		CurrentRotationAngle += 360.0f;
	}

	if (PlayerIconImage)
	{
		PlayerIconImage->SetRenderTransformAngle(CurrentRotationAngle);
	}
}