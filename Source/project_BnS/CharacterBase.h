// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterBase.generated.h"

UCLASS()
class PROJECT_BNS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	// 카메라 관련 //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyTPS_Cam, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = myTPS_Cam, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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

};
