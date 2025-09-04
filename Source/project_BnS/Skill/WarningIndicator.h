// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarningIndicator.generated.h"

UCLASS()
class PROJECT_BNS_API AWarningIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWarningIndicator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetDecalSize(float Radius);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDecalComponent> DecalComponent;
};
