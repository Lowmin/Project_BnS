// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsController.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainUi.h"
#include "UI/MainUIPresenter.h"
#include "UI/Inventory/InventoryPopup.h"
#include "MyPlayer.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "UI/Inventory/InventoryPresenter.h"


ABnsController::ABnsController()
{
	static ConstructorHelpers::FClassFinder<UMainUi> res(TEXT("/Game/UI/WBP_Ingame.WBP_Ingame_C"));
	if (res.Succeeded())
	{
		MainUiClass = res.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> iaInventory(TEXT("/Game/Input/IA_Inventory.IA_Inventory"));
	if (iaInventory.Succeeded())
	{
		IA_Inventory = iaInventory.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> inventory(TEXT("/Game/UI/Inventory/WBP_InventoryPopup.WBP_InventoryPopup_C"));
	if (inventory.Succeeded())
	{
		InventoryClass = inventory.Class;
	}
}

void ABnsController::BeginPlay()
{
	Super::BeginPlay();
}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

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


	if (InventoryClass && IA_Inventory)
	{ 
		Inventory = CreateWidget<UInventoryPopup>(this, InventoryClass);
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Hidden);

		InventoryPresenter = NewObject<UInventoryPresenter>(this, TEXT("inventoryPresenter"));
		InventoryPresenter->SetPlayer(player);
		InventoryPresenter->SetInventoryPopup(Inventory);
	}

	UPopup::PopupCount = 0;
}

void ABnsController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedPlayerInputComponent != nullptr)
	{
		EnhancedPlayerInputComponent->BindAction(IA_Inventory, ETriggerEvent::Completed, this, &ABnsController::ShowInventory);
	}
}

void ABnsController::ShowInventory()
{
	if (Inventory == nullptr)
		return;

	Inventory->SetVisiblePopup(!Inventory->IsVisible());

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "popup cnt : " + FString::FromInt(UPopup::PopupCount));
}
