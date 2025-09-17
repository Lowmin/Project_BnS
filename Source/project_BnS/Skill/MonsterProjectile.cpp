// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../CharacterBase.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

AMonsterProjectile::AMonsterProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->InitSphereRadius(30.f);
	RootComponent = HitSphere;

	HitSphere->SetCollisionObjectType(ECC_GameTraceChannel7);
	HitSphere->SetCollisionProfileName(TEXT("MonsterProjectile"));
	HitSphere->SetNotifyRigidBodyCollision(true);
	HitSphere->SetGenerateOverlapEvents(true);

	HitSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectile::OnOverlapBegin);

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
}

void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);
}

void AMonsterProjectile::DoExplore(const FVector& HitPos)
{
	if (!CurrentData.ExplosionOnImpact || CurrentData.ExplosionRadius <= 0.f) return;

	DrawDebugSphere(GetWorld(), HitPos, CurrentData.ExplosionRadius, 16, FColor::Orange, false, 1.0f, 0, 2.0f);
}

void AMonsterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();

	UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapped with: %s"), *GetNameSafe(Other));

	// 자기 자신 또는 아군("Enemy" 태그)과 겹치면 무시하고 통과
	if (!Other || Other == MyOwner || (MyOwner->ActorHasTag(FName("Enemy")) && Other->ActorHasTag(FName("Enemy"))))
	{
		return;
	}

	// 유효한 타겟(플레이어 등)과 겹쳤을 때의 로직
	UE_LOG(LogTemp, Warning, TEXT("Monster Projectile Overlapped with %s"), *GetNameSafe(Other));

	if (OnHitActor.IsBound())
	{
		OnHitActor.Broadcast(Other);
	}

	if (HitVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFX, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, SweepResult.ImpactPoint);
	}

	const FVector Impact = FVector(SweepResult.ImpactPoint);
	const FVector Self = GetActorLocation();
	const FVector Position = SweepResult.bBlockingHit ? Impact : Self;
	DoExplore(Position);
	Destroy(); // 유효 타겟과 겹쳤으므로 파괴
}

void AMonsterProjectile::SetupProjectileData(const FProjectileData& InData, AActor* InOwner, UParticleSystem* InHitVFX, USoundBase* InHitSound, UParticleSystem* InTrailVFX, USoundBase* InMovingSound)
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
		UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn VFX: %s"), *InTrailVFX->GetName());
		TrailVFXComponent->SetTemplate(InTrailVFX);
		TrailVFXComponent->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VFX is NULL in Data Table!"));
	}
	if (MovingSoundComponent && InMovingSound)
	{
		MovingSoundComponent->SetSound(InMovingSound);
		MovingSoundComponent->Play();
	}
}