// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkill.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

bool UProjectileSkill::CanExecuteSkill_Implementation(ACharacter* Caster)
{
	return Caster != nullptr;
}

void UProjectileSkill::ExecuteSkill_Implementation(ACharacter* Caster, AActor* Target, FVector TargetPos)
{
	LoadSkillData();                                 
	if (!Caster || !Caster->GetWorld()) return;

	FHitResult HitInfo;
	AActor* FirstEnemy = FindFirstEnemy(Caster, HitInfo);
	if (!FirstEnemy) return;

	UGameplayStatics::ApplyDamage(FirstEnemy, HitDamage, Caster->GetController(), Caster, nullptr);

}

AActor* UProjectileSkill::FindFirstEnemy(ACharacter* Caster, FHitResult& OutHit) const
{
	if (!Caster) return nullptr;

	const FVector StartLocation = (Caster->GetMesh() ? Caster->GetMesh()->GetComponentLocation() : Caster->GetActorLocation());
	const FVector ForwardDirection = (Caster->GetMesh() ? Caster->GetMesh()->GetForwardVector() : Caster->GetActorForwardVector());
	const float MaxRangeMeter = MaxRange * 100.f;
	const FVector EndLocation = StartLocation + ForwardDirection * MaxRangeMeter;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ProjectileSkillSweep), false, Caster);
	QueryParams.AddIgnoredActor(Caster);

	// 폰 검사(커스텀 채널 생성 필요)
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	const FCollisionShape Shape = FCollisionShape::MakeSphere(SweepRadius);

	TArray<FHitResult> AllHits;
	const bool Got =
		Caster->GetWorld()->SweepMultiByObjectType(
			AllHits, StartLocation, EndLocation, FQuat::Identity, ObjectParams, Shape, QueryParams
		);

	if (!Got) return nullptr;
	
	// 앞쪽부터
	AllHits.Sort([](const FHitResult& Front, const FHitResult& Back) { return Front.Time < Back.Time; });

	for (const FHitResult& Hit : AllHits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			OutHit = Hit;
			return HitActor;
		}
	}
	return nullptr;
}
