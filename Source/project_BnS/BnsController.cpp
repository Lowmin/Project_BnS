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
#include "UI/CharacterInfo/CharacterInfoPopup.h"


ABnsController::ABnsController()
{
	static ConstructorHelpers::FClassFinder<UMainUi> res(TEXT("/Game/UI/WBP_Ingame.WBP_Ingame_C"));
	if (res.Succeeded())
	{
		MainUiClass = res.Class;
	}

	static ConstructorHelpers::FClassFinder<UPopupManager> popupManagerClass(TEXT("/Game/UI/WBP_PopupManager.WBP_PopupManager_C"));
	if (popupManagerClass.Succeeded())
	{
		PopupManagerClass = popupManagerClass.Class;
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

	static ConstructorHelpers::FClassFinder<UWorldMapPopup> worldmap(TEXT("/Game/UI/Minimap/WBP_WorldMapPopup.WBP_WorldMapPopup_C"));
	if (worldmap.Succeeded())
	{
		WorldMapPopupClass = worldmap.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> iaWorldMap(TEXT("/Game/Input/IA_WorldMap.IA_WorldMap"));
	if (iaWorldMap.Succeeded())
	{
		IA_WorldMap = iaWorldMap.Object;
	}

	static ConstructorHelpers::FClassFinder<UCharacterInfoPopup> characterInfo(TEXT("/Game/UI/CharacterInfo/WBP_CharacterInfoPopup.WBP_CharacterInfoPopup_C"));
	if (characterInfo.Succeeded())
	{
		CharacterInfoPopupClass = characterInfo.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> iaCharacterInfo(TEXT("/Game/Input/IA_CharacterInfo.IA_CharacterInfo"));
	if (iaCharacterInfo.Succeeded())
	{
		IA_CharacterInfo = iaCharacterInfo.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LevelUpAction(TEXT("/Game/Input/IA_LevelUp.IA_LevelUp"));
	if (LevelUpAction.Succeeded())
	{
		IA_LevelUp = LevelUpAction.Object;
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

	if (PopupManagerClass != nullptr)
	{
		PopupManager = CreateWidget<UPopupManager>(this, PopupManagerClass);
		PopupManager->AddToViewport();
		PopupManager->Initialize(this);
	}

	MyPlayer->OnMovementInput.BindUObject(this, &ABnsController::OnMovementInputReceived);
	MyPlayer->IsUIVisibleDelegate.BindUObject(this, &ABnsController::IsPopupVisible);

	if (InventoryPopupClass && IA_Inventory)
	{
		UInventoryPopup* InventoryPopup = Cast<UInventoryPopup>(PopupManager->RegisterPopup(EPopupType::Inventory, InventoryPopupClass, FVector2D(700.0f, 150.0f), FVector2D(958.0f, 782.0f), true));

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

			MinimapPresenter->OnMapPositionUpdated.AddDynamic(MinimapView, &UMinimapWidget::SetMinimap);
			MinimapPresenter->OnPlayerRotationUpdated.AddDynamic(PlayerIcon, &UPlayerIconWidget::SetPlayerIconRotation);
			MinimapPresenter->OnPlayerIconOffsetUpdated.AddDynamic(PlayerIcon, &UPlayerIconWidget::SetPlayerIconPosition);

			FVector2D MapWorldOrigin;
			FVector2D MapWorldSize;
			MapBoundary->GetMapBounds(MapWorldOrigin, MapWorldSize);

			// 미니맵 위젯 크기
			const FVector2D MinimapScreenSize(300.0f, 300.0f);

			MinimapPresenter->Init(player, MapWorldOrigin, MapWorldSize, MinimapScreenSize);
		}
	}

	// 월드맵 팝업 등록
	if (WorldMapPopupClass)
	{
		UWorldMapPopup* WorldMapPopup = Cast<UWorldMapPopup>(PopupManager->RegisterPopup(EPopupType::WorldMap, WorldMapPopupClass, FVector2D(460.0f, 90.0f), FVector2D(1000.0f, 900.0f), true));
		if (WorldMapPopup)
		{
			AMinimapBounds* MapBoundary = Cast<AMinimapBounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AMinimapBounds::StaticClass()));

			UTexture2D* WorldMapTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Minimap/Minimap.Minimap"));

			if (MapBoundary && WorldMapTexture)
			{
				WorldMapPopup->SetWorldMap(MapBoundary, player, WorldMapTexture);
			}
		}
	}

	if (CharacterInfoPopupClass)
	{
		PopupManager->RegisterPopup(EPopupType::CharacterInfo, CharacterInfoPopupClass, FVector2D(944.0f, 92.0f), FVector2D(500.0f, 800.0f), false);
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
		EnhancedPlayerInputComponent->BindAction(IA_CharacterInfo, ETriggerEvent::Completed, this, &ABnsController::ToggleCharacterInfo);

		// ESC 바인딩
		EnhancedPlayerInputComponent->BindAction(IA_CloseAllPopups, ETriggerEvent::Completed, this, &ABnsController::CloseAllPopup);

		if (AMyPlayer* MyPlayer = GetPawn<AMyPlayer>())
		{
			EnhancedPlayerInputComponent->BindAction(MyPlayer->IA_Movement, ETriggerEvent::Completed, this, &ABnsController::OnMovementStopped);
			EnhancedPlayerInputComponent->BindAction(MyPlayer->IA_Jump, ETriggerEvent::Completed, this, &ABnsController::OnMovementStopped);

		}

		if (IA_LevelUp)
		{
			EnhancedPlayerInputComponent->BindAction(IA_LevelUp, ETriggerEvent::Started, this, &ABnsController::EnterLevelUp);
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

bool ABnsController::IsPopupVisible()
{
	if (PopupManager)
	{
		return PopupManager->IsPopupVisible();
	}
	return false;
}

void ABnsController::EnterLevelUp()
{
	AMyPlayer* MyPlayer = GetPawn<AMyPlayer>();
	if (MyPlayer)
	{
		const float ExpToLevelUp = (MyPlayer->GetMaxExp() - MyPlayer->GetCurExp()) + 1.f;
		MyPlayer->AddExp(ExpToLevelUp);

		UE_LOG(LogTemp, Warning, TEXT("Forced Level Up!"));
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

void ABnsController::ToggleCharacterInfo()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn && ControlledPawn->GetVelocity().SizeSquared() > 0.f)
	{
		bPopupDuringMovement = true;
	}

	if (PopupManager)
	{
		PopupManager->TogglePopup(EPopupType::CharacterInfo);
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
