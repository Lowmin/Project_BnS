// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

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
	bool IsDead() const;
	

	// Stat Component
private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	class UStatComponent* status = nullptr;

	// skill Component
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
