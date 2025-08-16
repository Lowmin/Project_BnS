// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSystemComponent.h"
#include "SkillController.h"
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
		Controller->SetOwnerActor(GetOwner());
		Controller->Setup(DT_SkillCommon);
		Controller->RegisterTypeTable(ESkill_Type::Melee, DT_Melee);
		Controller->RegisterTypeTable(ESkill_Type::Projectile, DT_Projectile);
	}

	if (BasicAttackIcons.IsValidIndex(0))
		OnSkillIconChanged.Broadcast(BasicAttackSlotIndex, BasicAttackIcons[0]);

	if (DT_SkillCommon)
	{
		const FSkillCommonData* Data = DT_SkillCommon->FindRow<FSkillCommonData>(ProjectileRowName, TEXT("InitialIcon"));
		if (Data && !Data->SkillIcon.IsNull())
		{
			UTexture2D* Icon = Data->SkillIcon.LoadSynchronous();
			OnSkillIconChanged.Broadcast(ProjectileSlotIndex, Icon);
		}
	}
	bCanInputNext = true;
	ComboStep = 0;
}

void USkillSystemComponent::HandleBasicAttack()
{
	if (!Controller || !bCanInputNext)
	{
		return;
	}

	// 콤보 단계
	const int32 NextStep = (ComboStep % ComboMax) + 1;
	const FName SkillRowName = FName(*FString::Printf(TEXT("BasicAttack_%d"), NextStep));

	if (Controller->Execute(SkillRowName, FindCurrentTarget()))
	{
		ComboStep = NextStep;
		StartComboWindow();
		BroadcastSkillUI(SkillRowName, BasicAttackSlotIndex);
	}
}

void USkillSystemComponent::UseProjectileSkill()
{
	if (!Controller) return;

	if (Controller->Execute(ProjectileRowName, FindCurrentTarget()))
	{
		BroadcastSkillUI(ProjectileRowName, ProjectileSlotIndex);
	}
}

void USkillSystemComponent::BroadcastSkillUI(const FName& SkillRowName, int32 SlotIndex)
{
	if (!DT_SkillCommon) return;

	const FSkillCommonData* Data = DT_SkillCommon->FindRow<FSkillCommonData>(SkillRowName, TEXT("UIBroadcast"));
	if (!Data) return;

	// Icon
	if (SkillRowName.ToString().StartsWith(TEXT("BasicAttack_")))
	{
		const int32 IconIndex = FMath::Clamp(ComboStep - 1, 0, 2);
		if (BasicAttackIcons.IsValidIndex(IconIndex))
		{
			OnSkillIconChanged.Broadcast(SlotIndex, BasicAttackIcons[IconIndex]);
		}
	}
	else
	{
		if (!Data->SkillIcon.IsNull())
		{
			UTexture2D* Icon = Data->SkillIcon.LoadSynchronous();
			OnSkillIconChanged.Broadcast(SlotIndex, Icon);
		}
	}

	// 쿨다운 UI 타이머 시작
	if (Data->Cooldown > 0.f)
	{
		const float EndTime = Controller->GetCurrentTime() + Data->Cooldown;
		CooldownEndTimes.FindOrAdd(SlotIndex) = EndTime;

		// UI 타이머 안 돌면 시작
		if (!GetWorld()->GetTimerManager().IsTimerActive(CooldownUITimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(CooldownUITimerHandle, this, &USkillSystemComponent::TickCooldownUI, 0.1f, true);
		}
	}
}

void USkillSystemComponent::TickCooldownUI()
{
	const float Now = Controller->GetCurrentTime();
	TArray<int32> FinishedSlots;

	for (auto& Pair : CooldownEndTimes)
	{
		const int32 Slot = Pair.Key;
		const float EndTime = Pair.Value;
		const float Remain = FMath::Max(0.f, EndTime - Now);

		float TotalCooldown = 1.f; // 임시값

		OnSkillCooldownTick.Broadcast(Slot, Remain, TotalCooldown);

		if (Remain <= 0.f)
		{
			FinishedSlots.Add(Slot);
		}
	}

	for (const int32 Slot : FinishedSlots)
	{
		CooldownEndTimes.Remove(Slot);
	}

	if (CooldownEndTimes.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownUITimerHandle);
	}
}

void USkillSystemComponent::StartComboWindow()
{
	bCanInputNext = true;
	GetWorld()->GetTimerManager().SetTimer(
		ComboTimerHandle, this, &USkillSystemComponent::CloseComboWindow, ComboTimeLimit, false
	);
}

void USkillSystemComponent::CloseComboWindow()
{
	bCanInputNext = true;
	ComboStep = 0;

	if (BasicAttackIcons.IsValidIndex(0))
	{
		OnSkillIconChanged.Broadcast(BasicAttackSlotIndex, BasicAttackIcons[0]);
	}
}

AActor* USkillSystemComponent::FindCurrentTarget() const
{
	if (const ATargetingSystem* TargetSysOwner = Cast<ATargetingSystem>(GetOwner()))
	{
		return TargetSysOwner->GetTarget();
	}
	if (ATargetingSystem* TargetSys = Cast<ATargetingSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ATargetingSystem::StaticClass())))
	{
		return TargetSys->GetTarget();
	}
	return nullptr;
}


