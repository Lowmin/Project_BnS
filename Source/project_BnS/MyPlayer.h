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

	// 입력
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Movement;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Look;

	// 새로 추가할 달리기 Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Run; // IA_Run 추가

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void BeginWalking();
	void StopWalking();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RunSpeed;
};