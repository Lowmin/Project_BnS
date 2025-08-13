// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ProjectileData.h"
#include "ProjectileBall.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PROJECT_BNS_API AProjectileBall : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBall();
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* HitSphere = nullptr;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MoveSphere = nullptr;

	UPROPERTY(Transient)
	FProjectileData CurrentData;

	UFUNCTION()
	void DoExplore(const FVector& HitPos);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit);


public:
	void SetupFromData(const FProjectileData& InData, AActor* InOwner);

};
