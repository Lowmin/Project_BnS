// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsController.h"

#include "Blueprint/UserWidget.h"
#include "MainUi.h"
#include "MainUIPresenter.h"
#include "CharacterBase.h"

ABnsController::ABnsController()
{
	ConstructorHelpers::FClassFinder<UMainUi> res(TEXT("/Game/UI/WBP_Ingame.WBP_Ingame_C"));
	if (res.Succeeded())
	{
		mainUiClass = res.Class;
	}
}

void ABnsController::BeginPlay()
{
	Super::BeginPlay();

	// UI 생성 
	if (mainUiClass != nullptr)
	{
		mainUi = CreateWidget<UMainUi>(this, mainUiClass);
		mainUi->AddToViewport();
	}
	
	//auto presenter = new MainUIPresenter(mainUi, GetPawn<ACharacterBase>());
}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}
