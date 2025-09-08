// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ProjectileData.h"
#include "ProjectileBall.generated.h"

class UParticleSystem;
class USoundBase;
class UProjectileMovementComponent;
class ACharacterBase;
class UParticleSystemComponent;
class UAudioComponent;

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
	void ProjectileDamage(AActor* Enemy);

	UPROPERTY()
	int32 DamageValue = 0;

	UPROPERTY()
	bool IsDamage = false;

	// ÀÌÆåÆ®
	UPROPERTY()
	TObjectPtr<UParticleSystem> HitVFX = nullptr;

	UPROPERTY()
	TObjectPtr<USoundBase> HitSound = nullptr;

	// ±ËÀû È¿°ú
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> TrailVFXComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> MovingSoundComponent;

public:
	void SetupProjectileData(const FProjectileData& InData, AActor* InOwner, int32 InDamage = INDEX_NONE, 
		UParticleSystem* InHitVFX = nullptr, USoundBase* InHitSound = nullptr,
		UParticleSystem* InTrailVFX = nullptr, USoundBase* InMovingSound = nullptr);

};
