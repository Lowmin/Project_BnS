
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
	//PerformMeleeAttack();
	PerformMelee(GetTypeData_Melee());
}

void AMeleeSkill::OnSkillNotify_Custom(FName NotifyName)
{
	// 커스텀 노티파이
}