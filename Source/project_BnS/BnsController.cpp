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
		MainUiClass = res.Class;
	}
}

void ABnsController::BeginPlay()
{
	Super::BeginPlay();

	// UI 생성 
	if (MainUiClass != nullptr)
	{
		MainUi = CreateWidget<UMainUi>(this, MainUiClass);
		MainUi->AddToViewport();
	}
	
	AMyPlayer* player = GetPawn<AMyPlayer>();
	MainUIPresenter* presenter = new MainUIPresenter(MainUi, player);

	presenter->OnLevelChange(player->GetCharacterLevel());
	presenter->OnHpChange(player->GetCurHp(), player->GetMaxHp());
	presenter->OnMpChange(player->GetCurMp());
	presenter->OnNicknameChange(TEXT("BNSUser"));
	presenter->OnBattleChange(true);
}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}
