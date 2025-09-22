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
	UPROPERTY()
	TSubclassOf<class UMainUi> MainUiClass = nullptr;
	UPROPERTY()
	TObjectPtr<class UMainUi> MainUi = nullptr;

	UPROPERTY()
	TSubclassOf<class UUserWidget> InventoryClass = nullptr;

	UPROPERTY()
	TSubclassOf<class UInventoryPopup> InventoryPopupClass = nullptr;

	// 월드맵
	UPROPERTY()
	TSubclassOf<class UWorldMapPopup> WorldMapPopupClass;

	// 미니맵
	UPROPERTY()
	TObjectPtr<class UMinimapPresenter> MinimapPresenter = nullptr;

	UPROPERTY()
	TSubclassOf<class UCharacterInfoPopup> CharacterInfoPopupClass;


public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AMainUIPresenter> UIPresenter = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInventoryPresenter> InventoryPresenter = nullptr;

	// 팝업
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPopupManager> PopupManager = nullptr;

	// 빙의 시 호출 
	virtual void OnPossess(APawn* pawn) override;

	virtual void SetupInputComponent() override;

	void OnMovementInputReceived();

	bool IsPopupVisible();

	void EnterLevelUp();

	// Input
private:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Inventory = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CloseAllPopups = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_WorldMap = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharacterInfo = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_LevelUp = nullptr;

	void ToggleInventory();
	void ToggleWorldMap();
	void ToggleCharacterInfo();
	void OnMovementStopped();
	void CloseAllPopup();

	bool bPopupDuringMovement = false;

protected:
	virtual void Tick(float DeltaSeconds) override;
};
