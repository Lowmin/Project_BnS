// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkill.h"
#include "ProjectileBall.h"
#include "ProjectileData.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

void AProjectileSkill::ExecuteSkill_Implementation()
{
	bAutoDestroyOnMontageEnd = false;

	const FSkillType_Projectile* Type = GetTypeData_Projectile();
	if (!Type || !Type->ProjectileClass)
	{
		Destroy();
		return;
	}

	if (!MyMontage)
	{
		SpawnProjectile(*Type);
		return;
	}
	Super::ExecuteSkill_Implementation();
}

void AProjectileSkill::OnSkillNotify_Custom(FName NotifyName)
{
	if (NotifyName == TEXT("Shoot"))
	{
		if (const FSkillType_Projectile* Type = GetTypeData_Projectile())
		{
			SpawnProjectile(*Type);
		}
	}
}

void AProjectileSkill::CalcSpawnTransform(FVector& OutPos, FRotator& OutRot) const
{
	OutPos = FVector::ZeroVector;
	OutRot = FRotator::ZeroRotator;

	ACharacter* Player = GetOwnerCharacter();
	if (!Player) return;

	FVector Start = Player->GetActorLocation();

	// 캐릭터와 조금 떨어진 위치 
	Start += Player->GetActorForwardVector() * OffsetForward;
	Start += FVector(0, 0, OffsetUp);

	const AActor* Target = GetSkillTarget();
	OutRot = Target ? (Target->GetActorLocation() - Start).Rotation() : Player->GetActorRotation();
	OutPos = Start;
}

void AProjectileSkill::SpawnProjectile(const FSkillType_Projectile& Type)
{
	if (!Type.ProjectileClass) return;

	ACharacter* Player = GetOwnerCharacter();
	UWorld* World = GetWorld();
	if (!Player || !World) return;

	FVector Pos;
	FRotator Rot;
	CalcSpawnTransform(Pos, Rot);

	FActorSpawnParameters SP;
	SP.Owner = Player;
	SP.Instigator = Player;
	SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 액터 스폰
	AActor* Spawned = World->SpawnActor<AActor>(Type.ProjectileClass, Pos, Rot, SP);
	if (!Spawned) return;

	// 발사체 데이터 세팅 (값만 전달)
	if (AProjectileBall* Ball = Cast<AProjectileBall>(Spawned))
	{
		FProjectileData Data;
		Data.ProjectileSpeed = Type.Speed;                      // 속도
		Data.ProjectileLifeTime = Type.LifeSec;                    // 수명
		Data.GravityAffects = (Type.Gravity > 0.f);            // 중력 적용 여부
		Data.ExplosionOnImpact = (Type.ExplosionRadius > 0.f);    // 충돌 시 폭발
		Data.ExplosionRadius = Type.ExplosionRadius;            // 폭발 반경

		const int32 Damage = CalculateDamage();
		Ball->SetupProjectileData(Data, Player, MyHitVFX, MyHitSound, Type.TrailVFX, Type.MovingSound);
		Ball->OnHitActor.AddDynamic(this, &AProjectileSkill::OnProjectileHit);
	}
}

void AProjectileSkill::OnProjectileHit(AActor* HitActor)
{
	if (ACharacter* TargetCharacter = Cast<ACharacter>(HitActor))
	{
		ApplyDamageToCharacter(TargetCharacter);
		ApplyCCToCharacter(TargetCharacter);
	}

	Destroy();
}


