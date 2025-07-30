// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystem.h"
#include "InputActionValue.h"
#include "MyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API AMyPlayer : public ASkillSystem
{
	GENERATED_BODY()
	
public:
	AMyPlayer();
	
	// ют╥б
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Movement;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
};
