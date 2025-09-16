
#include "MeleeSkill.h"
#include "SkillData.h"
#include "../CharacterBase.h"
#include "../BossEnemy.h"

#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void AMeleeSkill::OnSkillNotify_Hit()
{
	PerformMeleeAttack();
}

void AMeleeSkill::OnSkillNotify_Custom(FName NotifyName)
{

}

void AMeleeSkill::PerformMeleeAttack()
{
	const FSkillType_Melee* Data = GetTypeData_Melee();
	ACharacter* Player = GetOwnerCharacter();
	if (!Data || !Player || !GetWorld()) return;

	const FVector Start = Player->GetActorLocation();
	const FVector Dir = Player->GetActorForwardVector();
	const FVector End = Start + Dir * Data->AttackLength;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Data->AttackRadius);
	FCollisionQueryParams QP(SCENE_QUERY_STAT(MeleeTrace), false, Player);

	FCollisionObjectQueryParams Obj;
	Obj.AddObjectTypesToQuery(ECC_Pawn);
	Obj.AddObjectTypesToQuery(ECC_GameTraceChannel6);

	TArray<FHitResult> HitObject;
	const bool bAny = GetWorld()->SweepMultiByObjectType(HitObject, Start, End, FQuat::Identity, Obj, Sphere, QP);

	// 디버그 구체
	DrawDebugSphere(GetWorld(), Start, Data->AttackRadius, 16, bAny ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.5f);

	for (const FHitResult& Hit : HitObject)
	{
		ACharacterBase* TargetCharacter = Cast<ACharacterBase>(Hit.GetActor());
		if (!TargetCharacter || TargetCharacter == Player || HitActors.Contains(TargetCharacter)) continue;

		ApplyDamageToCharacter(TargetCharacter);
		ApplyCCToCharacter(TargetCharacter);
		ApplyBuffToCharacter(TargetCharacter);

		if (MyHitVFX)
		{
			// 맞은 위치(ImpactPoint)에 이펙트 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MyHitVFX, Hit.ImpactPoint, Dir.Rotation());
		}
		if (MyHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, MyHitSound, Hit.ImpactPoint);
		}

		HitActors.Add(TargetCharacter);

		if (!Data->bCanHitMultiTarget) break;
	}
}