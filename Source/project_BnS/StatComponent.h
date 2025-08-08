// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FDele_Single_I, int32);
DECLARE_DELEGATE_TwoParams(FDele_Single_FF, float, float);

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
	float CurHp = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float MaxHp = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 MaxMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float Atk = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float Def = 10.0f;

public:
	FDele_Single_FF OnHpChange = nullptr;
	FDele_Single_I OnMpChange = nullptr;
	FDele_Single_I OnLevelChange = nullptr;
	
	// HP
	float GetCurHp() const;
	void SetCurHp(float hp);
	float GetMaxHp() const;

	// MP
	int32 GetCurMp() const;
	void SetCurMp(int32 mp);
	int32 GetMaxMp() const;

	// level
	void SetLevel(int32 level);
	int32 GetLevel() const;

	// Atk
	float GetAtk() const;

	// def
	float GetDef() const;

};
