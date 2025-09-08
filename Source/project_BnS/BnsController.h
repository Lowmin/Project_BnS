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
	TObjectPtr<class UInventory> Inventory = nullptr;


public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AMainUIPresenter> UIPresenter = nullptr;

	// 빙의 시 호출 
	virtual void OnPossess(APawn* pawn) override;

	virtual void SetupInputComponent() override;

	// Input
private:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Inventory = nullptr;

	void ShowInventory();
};
