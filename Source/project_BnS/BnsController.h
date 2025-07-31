// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BnsController.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_BNS_API ABnsController : public APlayerController
{
	GENERATED_BODY()


public:
	ABnsController();

private:
	virtual void BeginPlay() override;

	// UI
private:
	TSubclassOf<class UMainUi> mainUiClass = nullptr;
	UMainUi* mainUi = nullptr;

	// 빙의 시 호출 
	virtual void OnPossess(APawn* pawn) override;
};
