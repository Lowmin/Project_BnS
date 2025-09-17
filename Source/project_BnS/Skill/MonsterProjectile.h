// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileData.h"
#include "Components/SphereComponent.h"
#include "MonsterProjectile.generated.h"

class UParticleSystem;
class USoundBase;
class UProjectileMovementComponent;
class ACharacterBase;
class UParticleSystemComponent;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterProjectileHitDelegate, AActor*, HitActor);

UCLASS()
class PROJECT_BNS_API AMonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterProjectile();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Projectile")
	FOnMonsterProjectileHitDelegate OnHitActor;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* HitSphere;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MoveSphere;

	UPROPERTY(Transient)
	FProjectileData CurrentData;

	UFUNCTION()
	void DoExplore(const FVector& HitPos);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
	void SetupProjectileData(const FProjectileData& InData, AActor* InOwner,
		UParticleSystem* InHitVFX = nullptr, USoundBase* InHitSound = nullptr,
		UParticleSystem* InTrailVFX = nullptr, USoundBase* InMovingSound = nullptr);
};
