// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkill.h"
#include "ProjectileBall.h"
#include "ProjectileData.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

void AProjectileSkill::ExecuteSkill_Implementation()
{
	FProjectileData Data;
	if (!LoadProjectileData(Data)) return;

	Super::ExecuteSkill_Implementation();

	SpawnProjectile(Data);
}

bool AProjectileSkill::LoadProjectileData(FProjectileData& OutData)
{
	if (!TypeHandle.DataTable) return false;
	if (const FProjectileData* Row = TypeHandle.GetRow<FProjectileData>(TEXT("Projectile_Skill")))
	{
		OutData = *Row;
		return true;
	}

	return false;
}

void AProjectileSkill::SpawnProjectile(const FProjectileData& Data)
{
	ACharacter* Player = GetOwnerCharacter();
	if (!Player) return;

	const FVector StartPos =
		Player->GetActorLocation() + Player->GetActorForwardVector() * OffsetForward + FVector(0, 0, OffsetUp);

	AActor* Target = GetSkillTarget();
	const FRotator Rot =
		Target ? (Target->GetActorLocation() - StartPos).Rotation() : Player->GetActorRotation();

	FActorSpawnParameters Spawn;
	Spawn.Owner = Player;
	Spawn.Instigator = Player;
	Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TSubclassOf<AActor> ProjClass = Data.ProjectileClass.LoadSynchronous();
	if (!ProjClass) return;

	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjClass, StartPos, Rot, Spawn);
	if (!NewProjectile) return;

	if (AProjectileBall* Ball = Cast<AProjectileBall>(NewProjectile))
	{
		Ball->SetupProjectileData(Data, Player, CalculateDamage());
	}

}

