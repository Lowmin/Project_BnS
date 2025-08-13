// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystemComponent.h"
#include "SkillController.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h" 
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "../TargetingSystem.h"

USkillSystemComponent::USkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Controller)
	{
		Controller = NewObject<USkillController>(this);
		if (!Controller) return;

		Controller->SetOwnerActor(GetOwner());
		Controller->Setup(DT_SkillCommon);
		Controller->RegisterTypeTable(ESkill_Type::Melee, DT_Melee);
		Controller->RegisterTypeTable(ESkill_Type::Projectile, DT_Projectile);
	}

	ComboStep = 0;
	CanInputNext = true;
}

void USkillSystemComponent::OnRegister()
{
	Super::OnRegister();
}

void USkillSystemComponent::HandleBasicAttack()
{
	if (!Controller) return;

	if (!CanInputNext) return;

	if (ComboStep <= 0)
		ComboStep = 1;
	else
		ComboStep++;

	if (ComboStep > ComboMax) ComboStep = 1;

	const FName SkillName = FName(*FString::Printf(TEXT("BasicAttack_%d"), ComboStep));

	if (GEngine)
	{
		const FString Msg = FString::Printf(TEXT("%s : (%d)"), *SkillName.ToString(), ComboStep);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, Msg);
	}

	// 타겟
	AActor* CurrentTarget = FindCurrentTarget();
	Controller->Execute(SkillName, CurrentTarget);

	// 타이머 시작
	StartComboWindow(ComboTimeLimit);
}

void USkillSystemComponent::UseProjectileSkill()
{
	if (!Controller) return;
	AActor* CurrentTarget = FindCurrentTarget();
	Controller->Execute(ProjectileRowName, CurrentTarget);
	// 수정 필요
	StartComboWindow(ComboTimeLimit);
}

void USkillSystemComponent::StartComboWindow(float TimeSec)
{
	CanInputNext = true;
	GetWorld()->GetTimerManager().ClearTimer(ComboTimer);
	GetWorld()->GetTimerManager().SetTimer(
		ComboTimer, this, &USkillSystemComponent::CloseComboWindow, TimeSec, false
	);
}

void USkillSystemComponent::CloseComboWindow()
{
	CanInputNext = true;
	ComboStep = 0;
}

AActor* USkillSystemComponent::FindCurrentTarget() const
{
	if (const ATargetingSystem* TargetSysOwner = Cast<ATargetingSystem>(GetOwner()))
	{
		return TargetSysOwner->GetTarget();
	}

	// 월드의 TargetingSystem에서 현재 타겟 가져옴
	if (ATargetingSystem* TargetSys = Cast<ATargetingSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ATargetingSystem::StaticClass())))
	{
		return TargetSys->GetTarget();
	}
	return nullptr;
}




