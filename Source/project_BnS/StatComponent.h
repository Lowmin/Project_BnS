// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BNS_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float CurHp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float MaxHp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int CurMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int MaxMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int Level = 1;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float Atk = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float Def = 10.0f;

public:
	// HP
	float GetCurHp() const;
	void SetCurHp(float hp);
	float GetMaxHp() const;

	// MP
	int GetCurMp() const;
	void SetCurMp(float mp);
	int GetMaxMp() const;

	// level
	int GetLevel() const;

	// Atk
	float GetAtk() const;

	// def
	float GetDef() const;

};
