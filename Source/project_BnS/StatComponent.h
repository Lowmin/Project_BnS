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
	float curHp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float maxHp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int curMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int maxMp = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int level = 1;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float atk = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float def = 10.0f;

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
