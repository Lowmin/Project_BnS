// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatComponent.h"
#include "CharacterBase.generated.h"

enum class ECrowdControlType : uint8;
class USkillSystemComponent;
class USoundBase;

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
	
	// Status Component
protected:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UStatComponent> Status = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<class USoundBase> HitSound = nullptr;

public:
	UStatComponent* GetStatusComponent() const;
	const FString& GetCharacterName() const;
	float GetCurHp() const;
	float GetMaxHp() const;
	int GetCurMp() const;
	int GetCharacterLevel() const;

	// CrowdControl Component
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

	UFUNCTION(BlueprintCallable, Category = "Skills")
	USkillSystemComponent* GetSkillSystemComponent() const { return SkillSystem; }

	// Buff Component
private:
	UPROPERTY(VisibleAnywhere, Category = "Buff")
	TObjectPtr<class UBuffComponent> BuffComponent = nullptr;

public:
	/// <summary>
	/// 버프 관리 컴포넌트 
	/// </summary>
	/// <returns> 버프 관리 컴포넌트  </returns>
	UBuffComponent* GetBuffComponent() const;
};
