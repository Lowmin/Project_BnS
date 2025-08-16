// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../CharacterBase.h"

AProjectileBall::AProjectileBall()
{
	PrimaryActorTick.bCanEverTick = false;

	// 충돌 구
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->InitSphereRadius(30.f);
	// 콜리전 채널 세팅 필요
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitSphere->SetCollisionObjectType(ECC_WorldDynamic);
	HitSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	HitSphere->SetGenerateOverlapEvents(true);
	RootComponent = HitSphere;

	HitSphere->OnComponentHit.AddDynamic(this, &AProjectileBall::OnHit);
	HitSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBall::OnOverlap);

	MoveSphere = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Move"));
	MoveSphere->bRotationFollowsVelocity = true;
	MoveSphere->ProjectileGravityScale = 0.f;
	MoveSphere->InitialSpeed = 1200.f;
	MoveSphere->MaxSpeed = 1200.f;

	MoveSphere->UpdatedComponent = HitSphere;

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
	if (!Other || Other == GetOwner())
	{
		Destroy();
		return;
	}
	const FVector Impact = FVector(Hit.ImpactPoint);
	const FVector Self = GetActorLocation();
	const FVector Position = Hit.bBlockingHit ? Impact : Self;
	DoExplore(Position);
}

void AProjectileBall::OnOverlap(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	if (IsDamage) { Destroy(); return; }
	if (!Other || Other == GetOwner()) { Destroy(); return; }

	ProjectileDamage(Other);
	IsDamage = true;

	const FVector Position = bFromSweep ? FVector(SweepHit.ImpactPoint) : GetActorLocation();
	DoExplore(Position);
	Destroy();
}

void AProjectileBall::ProjectileDamage(AActor* Enemy)
{
	if (!Enemy || Enemy == GetOwner()) return;
	if (ACharacterBase* Character = Cast<ACharacterBase>(Enemy))
	{
		const int32 TotalDamage = FMath::Max(1, DamageValue);
		Character->OnDamaged(TotalDamage);
	}
}


void AProjectileBall::SetupProjectileData(const FProjectileData& InData, AActor* InOwner, int32 InDamage)
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

	DamageValue = (InDamage == INDEX_NONE) ? 0 : FMath::Max(1, InDamage);
}
