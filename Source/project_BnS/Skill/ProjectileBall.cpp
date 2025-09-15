// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../CharacterBase.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

AProjectileBall::AProjectileBall()
{
	PrimaryActorTick.bCanEverTick = false;

	// 충돌 구
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->InitSphereRadius(30.f);
	RootComponent = HitSphere;

	HitSphere->SetCollisionObjectType(ECC_GameTraceChannel7);
	HitSphere->SetCollisionProfileName(TEXT("Projectile"));
	HitSphere->SetNotifyRigidBodyCollision(true);

	HitSphere->OnComponentHit.AddDynamic(this, &AProjectileBall::OnHit);

	MoveSphere = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Move"));
	MoveSphere->bRotationFollowsVelocity = true;
	MoveSphere->ProjectileGravityScale = 0.f;
	MoveSphere->InitialSpeed = 1200.f;
	MoveSphere->MaxSpeed = 1200.f;

	MoveSphere->UpdatedComponent = HitSphere;

	// 이펙트
	TrailVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailVFX"));
	TrailVFXComponent->SetupAttachment(RootComponent);

	MovingSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TravelSound"));
	MovingSoundComponent->SetupAttachment(RootComponent);

	// 유도 기능

}

void AProjectileBall::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);
}


void AProjectileBall::DoExplore(const FVector& HitPos)
{
	if (!CurrentData.ExplosionOnImpact || CurrentData.ExplosionRadius <= 0.f) return;

	DrawDebugSphere(GetWorld(), HitPos, CurrentData.ExplosionRadius, 16, FColor::Orange, false, 1.0f, 0, 2.0f);

	Destroy();
}

void AProjectileBall::OnHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (Other && Other == GetOwner())
	{
		return;
	}

	if (MyOwner->ActorHasTag("Enemy") && Other->ActorHasTag("Enemy"))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *GetNameSafe(Other));
	if (!Other || Other == GetOwner())
	{
		Destroy();
		return;
	}
	if (OnHitActor.IsBound())
	{
		OnHitActor.Broadcast(Other);
	}

	if (HitVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFX, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
	}

	const FVector Impact = FVector(Hit.ImpactPoint);
	const FVector Self = GetActorLocation();
	const FVector Position = Hit.bBlockingHit ? Impact : Self;
	DoExplore(Position);
	Destroy();
}

void AProjectileBall::SetupProjectileData(const FProjectileData& InData, AActor* InOwner, UParticleSystem* InHitVFX, USoundBase* InHitSound, UParticleSystem* InTrailVFX, USoundBase* InMovingSound)
{
	CurrentData = InData;
	SetOwner(InOwner);
	if (APawn* Pawn = Cast<APawn>(InOwner)) SetInstigator(Pawn);

	// 속도, 수명, 중력
	MoveSphere->InitialSpeed = CurrentData.ProjectileSpeed;
	MoveSphere->MaxSpeed = CurrentData.ProjectileSpeed;
	SetLifeSpan(CurrentData.ProjectileLifeTime);
	MoveSphere->ProjectileGravityScale = CurrentData.GravityAffects ? 1.f : 0.f;

	// 발사 방향
	MoveSphere->bIsHomingProjectile = false;
	MoveSphere->Velocity = GetActorForwardVector() * MoveSphere->InitialSpeed;

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, FColor::Cyan, false, 2.f, 0, 2.f);

	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 이펙트
	HitVFX = InHitVFX;
	HitSound = InHitSound;

	if (TrailVFXComponent && InTrailVFX)
	{
		TrailVFXComponent->SetTemplate(InTrailVFX);
		TrailVFXComponent->Activate();
	}
	if (MovingSoundComponent && InMovingSound)
	{
		MovingSoundComponent->SetSound(InMovingSound);
		MovingSoundComponent->Play();
	}
}
