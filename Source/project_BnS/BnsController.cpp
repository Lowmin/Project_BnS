// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsController.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainUi.h"
#include "UI/MainUIPresenter.h"
#include "MyPlayer.h"

ABnsController::ABnsController()
{
	ConstructorHelpers::FClassFinder<UMainUi> res(TEXT("/Game/UI/WBP_Ingame.WBP_Ingame_C"));
	if (res.Succeeded())
	{
		MainUiClass = res.Class;
	}
	//UIPresenter = CreateDefaultSubobject<AMainUIPresenter>(TEXT("UiPresenter"));
	
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
	UIPresenter = GetWorld()->SpawnActor<AMainUIPresenter>(AMainUIPresenter::StaticClass());
	UIPresenter->SetMainUI(MainUi);
	UIPresenter->SetMyPlayer(player);
	UIPresenter->OnHpChange(player->GetCurHp(), player->GetMaxHp());
	UIPresenter->OnMpChange(player->GetCurMp());
	UIPresenter->OnStaminaChange(player->GetCurStamina(), player->GetMaxStamina());
	UIPresenter->OnLevelChange(player->GetCharacterLevel());
	UIPresenter->OnExpChange(player->GetCurExp(), player->GetMaxExp());
	UIPresenter->OnNicknameChange(player->GetCharacterName());
	UIPresenter->OnBattleChange(false);

}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}
