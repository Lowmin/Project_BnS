// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatComponent.h"
#include "CharacterBase.generated.h"

enum class ECrowdControlType : uint8;
class USkillSystemComponent;

UCLASS()
class PROJECT_BNS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	void Attack();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	virtual void OnDamaged(int32 damage);
	virtual void Die();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UStatComponent> Status = nullptr;
public:
	UStatComponent* GetStatusComponent() const;

	// CrowdControl
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCrowdControlComponent> CrowdControl = nullptr;

public:
	UCrowdControlComponent* GetCrowdControlComponent();
	void SetCrowdControl(ECrowdControlType type, float duration);
	ECrowdControlType GetCrowdControlType();

	// skill Component
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkillSystemComponent* SkillSystem = nullptr;

public:
	float GetCurHp() const;
	float GetMaxHp() const;
	int GetCurMp() const;
	int GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	USkillSystemComponent* GetSkillSystemComponent() const { return SkillSystem; }

};
