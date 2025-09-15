// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapBounds.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_BNS_API AMinimapBounds : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinimapBounds();

	UFUNCTION(BlueprintPure, Category = "Minimap")
	void GetMapBounds(FVector2D& OutWorldOrigin, FVector2D& OutWorldSize) const;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoundaryBox;
};
