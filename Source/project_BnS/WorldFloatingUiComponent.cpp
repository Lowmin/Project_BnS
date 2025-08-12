// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldFloatingUiComponent.h"
#include "Kismet/KismetMathLibrary.h"

UWorldFloatingUiComponent::UWorldFloatingUiComponent()
{
	SetWidgetSpace(EWidgetSpace::World);
	
}

void UWorldFloatingUiComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void UWorldFloatingUiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CameraManager == nullptr)
		return;

	FVector camPos = CameraManager->GetCameraLocation();
	FRotator LookAtCam = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), camPos);

	SetWorldRotation(LookAtCam);
}
