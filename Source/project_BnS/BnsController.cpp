// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsController.h"

#include "Blueprint/UserWidget.h"
#include "MainUi.h"
#include "MainUIPresenter.h"
#include "MyPlayer.h"

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
	
	auto character = GetPawn<AMyPlayer>();
	auto presenter = new MainUIPresenter(mainUi, character);

	presenter->OnLevelChange(character->GetCharacterLevel());
	presenter->OnHpChange(character->GetCurHp(), character->GetMaxHp());
	presenter->OnMpChange(character->GetCurMp());
	presenter->OnNicknameChange(TEXT("BNSUser"));
	presenter->OnBattleChange(true);
}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}
