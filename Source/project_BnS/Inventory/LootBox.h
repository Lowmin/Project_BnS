// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemLootData.h"
#include "InteractableInterface.h"
#include "LootBox.generated.h"

UCLASS()
class PROJECT_BNS_API ALootBox : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FDropItemInfo> ContainedItems;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
