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

#include "Kismet/GameplayStatics.h"
#include "UI/Minimap/MinimapPresenter.h"
#include "UI/Minimap/MinimapWidget.h"
#include "UI/Minimap/PlayerIconWidget.h"
#include "UI/Minimap/MinimapBounds.h"

#include "UI/PopupManager.h"
#include "UI/WorldMapPopup.h"


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

	static ConstructorHelpers::FClassFinder<UInventoryPopup> inventory(TEXT("/Game/UI/Inventory/WBP_InventoryPopup.WBP_InventoryPopup_C"));
	if (inventory.Succeeded())
	{
		InventoryPopupClass = inventory.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> iaCloseAll(TEXT("/Game/Input/IA_CloseAllPopups.IA_CloseAllPopups"));
	if (iaCloseAll.Succeeded())
	{
		IA_CloseAllPopups = iaCloseAll.Object;
	}

	static ConstructorHelpers::FClassFinder<UWorldMapPopup> worldmap(TEXT("/Game/UI/WBP_WorldMapPopup.WBP_WorldMapPopup_C"));
	if (worldmap.Succeeded())
	{
		WorldMapPopupClass = worldmap.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> iaWorldMap(TEXT("/Game/Input/IA_WorldMap.IA_WorldMap"));
	if (iaWorldMap.Succeeded())
	{
		IA_WorldMap = iaWorldMap.Object;
	}
}

void ABnsController::BeginPlay()
{
	Super::BeginPlay();
}

void ABnsController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	AMyPlayer* MyPlayer = GetPawn<AMyPlayer>();

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

	PopupManager = NewObject<UPopupManager>(this, TEXT("PopupManager"));
	PopupManager->Initialize(this);

	MyPlayer->OnMovementInput.BindUObject(this, &ABnsController::OnMovementInputReceived);

	if (InventoryPopupClass && IA_Inventory)
	{
		UInventoryPopup* InventoryPopup = Cast<UInventoryPopup>(PopupManager->RegisterPopup(EPopupType::Inventory, InventoryPopupClass));

		if (InventoryPopup)
		{
			InventoryPresenter = NewObject<UInventoryPresenter>(this, TEXT("inventoryPresenter"));
			InventoryPresenter->SetPlayer(Cast<AMyPlayer>(pawn));
			InventoryPresenter->SetInventoryPopup(InventoryPopup);
		}
	}


	if (player && MainUi && MainUi->GetMinimapWidget() && MainUi->GetPlayerIconWidget())
	{
		AMinimapBounds* MapBoundary = Cast<AMinimapBounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AMinimapBounds::StaticClass()));

		if (MapBoundary)
		{
			MinimapPresenter = NewObject<UMinimapPresenter>(this);

			UMinimapWidget* MinimapView = MainUi->GetMinimapWidget();
			UPlayerIconWidget* PlayerIcon = MainUi->GetPlayerIconWidget();

			MinimapPresenter->OnMapPositionUpdated.AddDynamic(MinimapView, &UMinimapWidget::SetMapPosition);
			MinimapPresenter->OnPlayerRotationUpdated.AddDynamic(PlayerIcon, &UPlayerIconWidget::UpdateRotation);

			FVector2D MapWorldOrigin;
			FVector2D MapWorldSize;
			MapBoundary->GetMapBounds(MapWorldOrigin, MapWorldSize);

			MinimapPresenter->Init(player, MapWorldOrigin, MapWorldSize);
		}
	}

	// 월드맵 팝업 등록
	if (WorldMapPopupClass)
	{
		PopupManager->RegisterPopup(EPopupType::WorldMap, WorldMapPopupClass);
	}
}

void ABnsController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 인벤토리 토글 바인딩
		EnhancedPlayerInputComponent->BindAction(IA_Inventory, ETriggerEvent::Completed, this, &ABnsController::ToggleInventory);
		EnhancedPlayerInputComponent->BindAction(IA_WorldMap, ETriggerEvent::Completed, this, &ABnsController::ToggleWorldMap);

		// ESC 바인딩
		EnhancedPlayerInputComponent->BindAction(IA_CloseAllPopups, ETriggerEvent::Completed, this, &ABnsController::CloseAllPopup);

		if (AMyPlayer* MyPlayer = GetPawn<AMyPlayer>())
		{
			EnhancedPlayerInputComponent->BindAction(MyPlayer->IA_Movement, ETriggerEvent::Completed, this, &ABnsController::OnMovementStopped);
			EnhancedPlayerInputComponent->BindAction(MyPlayer->IA_Jump, ETriggerEvent::Completed, this, &ABnsController::OnMovementStopped);

		}
	}
}

void ABnsController::OnMovementInputReceived()
{
	if (bPopupDuringMovement)
	{
		bPopupDuringMovement = false;
		return;
	}

	if (PopupManager)
	{
		PopupManager->ClosePopup();
	}
}

void ABnsController::ToggleInventory()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn && ControlledPawn->GetVelocity().SizeSquared() > 0.f)
	{
		bPopupDuringMovement = true;
	}

	if (PopupManager)
	{
		PopupManager->TogglePopup(EPopupType::Inventory);
	}
}

void ABnsController::ToggleWorldMap()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn && ControlledPawn->GetVelocity().SizeSquared() > 0.f)
	{
		bPopupDuringMovement = true;
	}

	if (PopupManager)
	{
		PopupManager->TogglePopup(EPopupType::WorldMap);
	}
}

void ABnsController::OnMovementStopped()
{
	bPopupDuringMovement = false;
}

void ABnsController::CloseAllPopup()
{
	if (PopupManager)
	{
		PopupManager->ClosePopup();
	}
}

void ABnsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MinimapPresenter)
	{
		// Presenter 업데이트 -> 플레이어 정보 델리게이트 방송
		MinimapPresenter->SetMinimap();
	}
}
