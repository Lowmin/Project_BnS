// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WorldFloatingUiComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UWorldFloatingUiComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;
	
public:
	UWorldFloatingUiComponent(); 
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
