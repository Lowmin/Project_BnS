// SkillSystemComponent.cpp
#include "SkillSystemComponent.h"
#include "SkillSlotController.h"
#include "SkillBase.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"
#include "../CrowdControlComponent.h"
#include "../BossEnemy.h"
#include "../TargetingSystem.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Texture2D.h"

USkillSystemComponent::USkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		CachedStat = Character->GetStatusComponent();
		CachedOwnerCC = Character->GetCrowdControlComponent();
	}
	BuildSkillCache();

	SlotPanel = NewObject<USkillSlotController>(this);
	if (SlotPanel)
	{
		SlotPanel->Initialize(SkillTable, GetOwner(), GetWorld());
		SlotPanel->OnSlotIconChanged.AddDynamic(this, &USkillSystemComponent::HandleSlotIconChanged);
		SlotPanel->OnSlotPairChanged.AddDynamic(this, &USkillSystemComponent::HandleSlotPairChanged);
		SlotPanel->OnSlotCooldownTick.AddDynamic(this, &USkillSystemComponent::HandleSlotCooldownTick);

		// 에디터(MyPlayer)에서 세팅(BaseSkillConfig); 1타 슬롯에 장착
		for (const auto& BaseAttack : BaseSkillConfig)
		{
			SlotPanel->EquipBaseSkill(BaseAttack.Key, BaseAttack.Value);
		}
	}

	LastCheckedTarget = nullptr;
	LastCheckedCCType = ECrowdControlType::None;
	bLastTargetValid = false;

	CheckCC = nullptr;
}

void USkillSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ATargetingSystem* OwnerAsTargetingSystem = Cast<ATargetingSystem>(GetOwner());
	if (!OwnerAsTargetingSystem || !SlotPanel) return;

	// 1. 현재 상태
	AActor* CurrentTarget = OwnerAsTargetingSystem->GetTarget();
	const bool bIsTargetValidNow = IsValid(CurrentTarget);
	ECrowdControlType CurrentCCType = ECrowdControlType::None;
	int32 CurrentStackCount = 0;
	int32 CurrentMP = 0;

	if (bIsTargetValidNow)
	{
		if (const UCrowdControlComponent* TargetCC = CurrentTarget->FindComponentByClass<UCrowdControlComponent>())
		{
			CurrentCCType = TargetCC->GetCrowdControlType();
			CurrentStackCount = TargetCC->GetCurrentStack();
		}
	}

	if (CachedStat.IsValid())
	{
		CurrentMP = CachedStat->GetCurMp();
	}

	// 2. 타겟, CC 상태
	const bool bNeedsFullUpdate = (bIsTargetValidNow != bLastTargetValid || CurrentTarget != LastCheckedTarget.Get() || CurrentCCType != LastCheckedCCType || CurrentStackCount != LastCheckedStackCount);

	if (bNeedsFullUpdate)
	{
		UpdateAllSkillDisplays(CurrentTarget);

		LastCheckedTarget = CurrentTarget;
		bLastTargetValid = bIsTargetValidNow;
		LastCheckedCCType = CurrentCCType;
		LastCheckedStackCount = CurrentStackCount;
	}
	else
	{
		// 3. 스킬 사용 가능/불가능 여부
		for (const auto& Pair : BaseSkillConfig)
		{
			ESkillSlot Slot = Pair.Key;
			const int32 CurrentSkillID = SlotPanel->GetCurrentSkillID(Slot);
			if (CurrentSkillID <= 0) continue;

			const FSkillDataRow* Row = FindRowByID(CurrentSkillID);
			const bool bIsCurrentlyUsable = (Row && CanUseSkill(*Row, CurrentTarget, false));

			bool& bLastUsability = LastSkillUsable.FindOrAdd(Slot, true);
			if (bLastUsability != bIsCurrentlyUsable)
			{
				UI_OnSkillUsable.Broadcast(SlotToIndex(Slot), bIsCurrentlyUsable);
				bLastUsability = bIsCurrentlyUsable;
			}
		}
	}

	LastCheckedMP = CurrentMP;
}

// ========== 외부 API ==========
bool USkillSystemComponent::UseSkillByIndex(int32 Index, AActor* Target) { return UseSlot_Internal(IndexToSlot(Index), Target); }
bool USkillSystemComponent::UseSkillBySlot(ESkillSlot Slot, AActor* Target) { return UseSlot_Internal(Slot, Target); }

void USkillSystemComponent::UpdateAllSkillDisplays(AActor* CurrentTarget)
{
	if (!SlotPanel) return;
	for (const auto& Pair : BaseSkillConfig)
	{
		UpdateDisplayForSlot(Pair.Key, CurrentTarget);
	}
}

// ========== 애니 노티파이 ==========
void USkillSystemComponent::Notify_InputOpen(ESkillSlot Slot)
{
	FSlotRuntimeState& SlotState = GetState(Slot);
	SlotState.bInputWindowOpen = true;
	SlotState.bQueuedInput = false;
}

void USkillSystemComponent::Notify_AnimUnlock(ESkillSlot Slot)
{
	FSlotRuntimeState& SlotState = GetState(Slot);
	SlotState.bInputWindowOpen = false;
	if (SlotState.bQueuedInput && !SlotState.bConsumedThisMontage)
	{
		AActor* QueuedTarget = SlotState.QueuedTarget.Get();
		SlotState.bQueuedInput = false;
		SlotState.QueuedTarget = nullptr;
		SlotState.bConsumedThisMontage = true;
		UseSlot_Internal(Slot, QueuedTarget);
	}
}

void USkillSystemComponent::Notify_Hit(ESkillSlot Slot)
{
	if (TWeakObjectPtr<ASkillBase>* FoundActor = ActiveSkillActors.Find(Slot))
	{
		if (FoundActor->IsValid()) FoundActor->Get()->OnSkillNotify_Hit();
	}
}

void USkillSystemComponent::Notify_Custom(ESkillSlot Slot, FName NotifyName)
{
	if (TWeakObjectPtr<ASkillBase>* FoundActor = ActiveSkillActors.Find(Slot))
	{
		if (FoundActor->IsValid()) FoundActor->Get()->OnSkillNotify_Custom(NotifyName);
	}
}

// ========== 실행 파이프라인 ==========
bool USkillSystemComponent::UseSlot_Internal(ESkillSlot Slot, AActor* Target)
{
	// 1. 사전 검사
	if (!GetWorld() || !SkillTable) return false;

	FSlotRuntimeState& SlotState = GetState(Slot);
	const float Now = GetWorld()->GetTimeSeconds();

	if (Now < SlotState.AnimLockEndAt)
	{
		if (SlotState.bInputWindowOpen && !SlotState.bQueuedInput)
		{
			SlotState.bQueuedInput = true;
			SlotState.QueuedTarget = Target;
		}
		return false;
	}

	const int32 UseID = ResolveSkillToExecute(Slot, Target);
	if (UseID <= 0) return false;
	const FSkillDataRow* Row = FindRowByID(UseID);
	if (!Row) return false;

	check(Row->Slot == Slot);

	// 글로벌 락 검사(우선순위 / 락무시 반영)
	if (IsGlobalLock(*Row, Now))
	{
		if (SlotState.bInputWindowOpen && !SlotState.bQueuedInput)
		{
			SlotState.bQueuedInput = true;
			SlotState.QueuedTarget = Target;
		}
		return false;
	}

	if (!CanUseSkill(*Row, Target)) return false;

	// 2. 실행

	// 롤백용 원본 MP
	int32 OriginalMp = 0;
	if (CachedStat.IsValid()) OriginalMp = CachedStat->GetCurMp();

	if (Row->MpCost != 0 && CachedStat.IsValid())
	{
		const int32 FinalMp = FMath::Clamp(OriginalMp - Row->MpCost, 0, CachedStat->GetMaxMp());
		CachedStat->SetCurMp(FinalMp);
	}

	if (TWeakObjectPtr<ASkillBase>* FoundActor = ActiveSkillActors.Find(Slot))
	{
		if (FoundActor->IsValid()) FoundActor->Get()->CancelSkill_Implementation();
	}

	ASkillBase* skill = CreateSkill(*Row);
	if (!skill)
	{
		// 스폰 실패시 소모된 MP 복구
		if (Row->MpCost != 0 && CachedStat.IsValid()) CachedStat->SetCurMp(OriginalMp);
		return false;
	}

	// 3. 상태 커밋
	ActiveSkillActors.Add(Slot, skill);
	SlotState.CurStartTime = Now;
	SlotState.bConsumedThisMontage = false;
	skill->InitFromRow(*Row);
	skill->SetSkillTarget(Target);
	ISkillInterface::Execute_ExecuteSkill(skill);

	CommitStateAfterUse(*Row, SlotState);
	UpdateDisplayForSlot(Slot, Target);
	return true;
}

// ========== 스킬 결정 ==========
int32 USkillSystemComponent::ResolveSkillToExecute(ESkillSlot Slot, AActor* Target) const
{
	const FSkillDataRow* PrioritySkill = FindHighestPrioritySkillForSlot(Slot, Target);
	if (PrioritySkill) return PrioritySkill->SkillID;
	return ResolveChainSkill(Slot);
}

int32 USkillSystemComponent::ResolveChainSkill(ESkillSlot Slot) const
{
	const float Now = GetWorld()->GetTimeSeconds();
	const int32 BaseID = GetBaseID(Slot);
	if (BaseID <= 0) return -1;
	const FSlotRuntimeState* SlotState = TryGetState(Slot);
	if (!SlotState || SlotState->LastUsedSkillID <= 0) return BaseID;
	const FSkillDataRow* Last = FindRowByID(SlotState->LastUsedSkillID);
	if (!Last) return BaseID;
	if ((Now - SlotState->LastUsedSkillAt) > Last->ChainWindowSec) return BaseID;
	return (Last->ChainNextID > 0) ? Last->ChainNextID : BaseID;
}

const FSkillDataRow* USkillSystemComponent::FindHighestPrioritySkillForSlot(ESkillSlot Slot, AActor* Target) const
{
	const FSkillIDArray* CandidateIDs = CandidateIDsBySlot.Find(Slot);
	if (!CandidateIDs) return nullptr;

	const FSkillDataRow* BestSkillRow = nullptr;

	for (const int32 SkillID : CandidateIDs->IDs)
	{
		const FSkillDataRow* CurrentRow = FindRowByID(SkillID);
		if (!CurrentRow) continue;
		if (CurrentRow->Layer == ESkillLayer::Chain || CurrentRow->Layer == ESkillLayer::Base) continue;

		if (CheckActivationConditions(*CurrentRow, Target))
		{
			if (!BestSkillRow || CurrentRow->Priority > BestSkillRow->Priority)
			{
				BestSkillRow = CurrentRow;
			}
		}
	}
	return BestSkillRow;
}

bool USkillSystemComponent::CheckActivationConditions(const FSkillDataRow& Row, AActor* Target) const
{
	switch (Row.Layer)
	{
	case ESkillLayer::Proc: return false;
	case ESkillLayer::Finisher:
	{
		if (Row.NeedTargetCC.Num() > 0)
		{
			if (!Target) return false;

			if (const UCrowdControlComponent* TargetCC = Target->FindComponentByClass<UCrowdControlComponent>())
			{
				if (TargetCC->IsEffect() && Row.NeedTargetCC.Contains(TargetCC->GetCrowdControlType()))
				{
					return true;
				}
			}
			return false;
		}
		return false;
	}

	case ESkillLayer::BossCC:
	{
		if (!Target) return false;
		if (ABossEnemy* BossTarget = Cast<ABossEnemy>(Target))
		{
			if (UCrowdControlComponent* TargetCC = BossTarget->GetCrowdControlComponent())
			{
				if (TargetCC->IsEffect())
				{
					if (Row.NeedTargetCC.Num() == 0 || Row.NeedTargetCC.Contains(TargetCC->GetCrowdControlType()))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	} // switch 끝
	return false;
}

void USkillSystemComponent::RefreshCooldownViewForSlot(ESkillSlot Slot, int32 SkillID)
{
	if (!SlotPanel) return;

	const float Now = GetWorld()->GetTimeSeconds();

	// 1. 해당 슬롯의 개별 스킬 쿨타임
	float SkillRemainTime = 0.f;
	const float* SkillEndAt = CooldownEndAt.Find(SkillID);
	if (SkillEndAt && *SkillEndAt > Now)
	{
		SkillRemainTime = *SkillEndAt - Now;
	}

	// 2. GCD
	float GcdRemainTime = 0.f;
	if (GlobalLockEndAt > Now)
	{
		GcdRemainTime = GlobalLockEndAt - Now;
	}

	// 3. 더 긴 쪽이 대표 쿨타임
	const float DisplayCooldown = FMath::Max(SkillRemainTime, GcdRemainTime);

	// 4. 대표 쿨타임을 기준으로 UI를 표시하거나 끔
	if (DisplayCooldown > 0.f)
	{
		if (SkillRemainTime > GcdRemainTime)
		{
			if (SkillEndAt)
			{
				const FSkillDataRow* Row = FindRowByID(SkillID);
				if (Row)
				{
					SlotPanel->PlayCooldownShow(Slot, SkillID, *SkillEndAt, Row->CooldownSec, ECooldownUIType::Skill);
				}
			}
		}
		else
		{
			SlotPanel->PlayCooldownShow(Slot, -1, GlobalLockEndAt, GcdRemainTime, ECooldownUIType::Global);
		}
	}
	else
	{
		SlotPanel->StopCooldownShow(Slot);
	}

}

bool USkillSystemComponent::CanUseSkill(const FSkillDataRow& Row, AActor* Target, bool bCheckMPCost) const
{
	const float Now = GetWorld()->GetTimeSeconds();
	if (const float* EndAt = CooldownEndAt.Find(Row.SkillID))
	{
		if (*EndAt > Now) return false;
	}
	if (bCheckMPCost && Row.MpCost != 0)
	{
		if (!CachedStat.IsValid()) return false;
		if (Row.MpCost > 0)
		{
			if (CachedStat->GetCurMp() < Row.MpCost) return false;
		}
	}

	if (Row.Layer == ESkillLayer::Chain)
	{
		const FSlotRuntimeState* SlotState = TryGetState(Row.Slot);
		if (!SlotState || SlotState->LastUsedSkillID < 0) return false;
		const FSkillDataRow* Last = FindRowByID(SlotState->LastUsedSkillID);
		if (!Last) return false;
		const bool bOk = (Last->ChainNextID == Row.SkillID) && ((Now - SlotState->LastUsedSkillAt) <= Last->ChainWindowSec);
		if (!bOk) return false;
	}

	if (Row.NeedTargetCC.Num() > 0)
	{
		if (!Target) return false;

		const UCrowdControlComponent* TargetCC = Target->FindComponentByClass<UCrowdControlComponent>();
		if (!TargetCC) return false;

		if (!Row.NeedTargetCC.Contains(TargetCC->GetCrowdControlType())) return false;
	}

	// 스킬 사용 유효 사거리
	if (Row.MaxRange > 0.f && Target)
	{
		const float Distance = GetOwner()->GetDistanceTo(Target);
		if (Distance > Row.MaxRange)
		{
			return false;
		}
	}
	return true;
}

void USkillSystemComponent::CommitStateAfterUse(const FSkillDataRow& Row, FSlotRuntimeState& SlotState)
{
	const float Now = GetWorld()->GetTimeSeconds();
	// 쿨타임 데이터
	if (Row.CooldownSec > 0.f)
	{
		float& End = CooldownEndAt.FindOrAdd(Row.SkillID);
		if (End <= Now)
		{
			End = Now + Row.CooldownSec;
		}
	}
	// GCD 데이터
	if (Row.AnimationLockSec > 0.f)
	{
		SlotState.AnimLockEndAt = Now + Row.AnimationLockSec;
	}

	if (Row.GlobalCooldownSec > 0.f)
	{
		float NewGlobalLockEndAt = Now + Row.GlobalCooldownSec;
		GlobalLockEndAt = FMath::Max(GlobalLockEndAt, NewGlobalLockEndAt);

		GlobalLockPriority = Row.Priority;

		if (SlotPanel)
		{
			SlotPanel->PlayCooldownShow_All(GlobalLockEndAt, Row.GlobalCooldownSec, true);
		}
	}
	// 연계 상태 데이터
	SlotState.LastUsedSkillID = Row.SkillID;
	SlotState.LastUsedSkillAt = Now;
	if (Row.ChainNextID > 0 && Row.ChainWindowSec > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SlotState.ChainTimer);
		GetWorld()->GetTimerManager().SetTimer(SlotState.ChainTimer, FTimerDelegate::CreateUObject(this, &USkillSystemComponent::OnChainExpire, Row.Slot), Row.ChainWindowSec, false);
	}
	else
	{
		SlotState.LastUsedSkillID = -1;
		GetWorld()->GetTimerManager().ClearTimer(SlotState.ChainTimer);
	}
}

void USkillSystemComponent::OnChainExpire(ESkillSlot Slot)
{
	GetState(Slot).LastUsedSkillID = -1;
	GetState(Slot).bQueuedInput = false;
	UpdateDisplayForSlot(Slot, nullptr);
}

void USkillSystemComponent::UpdateDisplayForSlot(ESkillSlot Slot, AActor* Target)
{
	if (!SlotPanel) return;

	const int32 PrevID = SlotPanel->GetCurrentSkillID(Slot);
	const int32 NextID = ResolveSkillToExecute(Slot, Target);

	const int32 CheckSkillID = (NextID > 0) ? NextID : GetBaseID(Slot);
	const FSkillDataRow* Row = FindRowByID(CheckSkillID);
	const bool IsCurrentUsable = (Row && CanUseSkill(*Row, Target, false));

	bool& bLastUsable = LastSkillUsable.FindOrAdd(Slot, true);
	if (bLastUsable != IsCurrentUsable)
	{
		UI_OnSkillUsable.Broadcast(SlotToIndex(Slot), IsCurrentUsable);
		bLastUsable = IsCurrentUsable;
	}

	if (PrevID == NextID)
	{
		RefreshCooldownViewForSlot(Slot, NextID > 0 ? NextID : GetBaseID(Slot));
		return;
	}

	SlotPanel->StopCooldownShow(Slot);

	if (Slot == ESkillSlot::Slot0)
	{
		const int32 BaseID = GetBaseID(Slot);
		int32 FinalPrevID = (PrevID > 0) ? PrevID : BaseID;
		int32 FinalNextID = (NextID > 0) ? NextID : BaseID;

		if (FinalNextID > 0)
		{
			SlotPanel->ShowPairSkill(Slot, FinalPrevID, FinalNextID);
		}
		else
		{
			SlotPanel->ResetToBase(Slot);
		}
	}
	else
	{
		if (NextID <= 0)
		{
			SlotPanel->ResetToBase(Slot);
		}
		else
		{
			SlotPanel->ShowSkill(Slot, NextID);
		}

		UTexture2D* PrevIcon = IconCache.FindRef(PrevID);
		UTexture2D* NextIcon = IconCache.FindRef(NextID);
		const int32 BaseID = GetBaseID(Slot);

		if (!PrevIcon) PrevIcon = IconCache.FindRef(BaseID);
		if (!NextIcon) NextIcon = IconCache.FindRef(BaseID);

		if (PrevIcon && NextIcon)
		{
			UI_OnAnimatedSetIcon.Broadcast(SlotToIndex(Slot), PrevIcon, NextIcon);
		}
	}
	RefreshCooldownViewForSlot(Slot, NextID > 0 ? NextID : GetBaseID(Slot));
}

bool USkillSystemComponent::IsGlobalLock(const FSkillDataRow& Row, float Now) const
{
	if (Row.bOffLock) return false;
	if (Now < GlobalLockEndAt && Row.Priority <= GlobalLockPriority) return true;
	return false;
}

// ========== 스폰/캐시/유틸 ==========
ASkillBase* USkillSystemComponent::CreateSkill(const FSkillDataRow& Row)
{
	if (!GetWorld() || !Row.SkillActorClass) return nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<ASkillBase>(Row.SkillActorClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams);
}

void USkillSystemComponent::BuildSkillCache()
{
	RowNameByID.Reset();
	CandidateIDsBySlot.Reset();
	IconCache.Reset();
	if (!SkillTable) return;
	if (SkillTable->GetRowStruct() != FSkillDataRow::StaticStruct()) return;

	const TArray<FName> Names = SkillTable->GetRowNames();
	RowNameByID.Reserve(Names.Num());
	for (const FName& Nm : Names)
	{
		const FSkillDataRow* Row = SkillTable->FindRow<FSkillDataRow>(Nm, TEXT("BuildSkillCache"));
		if (!Row || Row->SkillID < 0) continue;
		RowNameByID.FindOrAdd(Row->SkillID) = Nm;
		if (Row->Slot != ESkillSlot::None)
		{
			CandidateIDsBySlot.FindOrAdd(Row->Slot).IDs.Add(Row->SkillID);
		}
		if (!Row->SkillIcon.IsNull())
		{
			if (UTexture2D* Icon = Row->SkillIcon.LoadSynchronous())
			{
				IconCache.Add(Row->SkillID, Icon);
			}
		}
	}
}

const FSkillDataRow* USkillSystemComponent::FindRowByID(int32 SkillID) const
{
	if (!SkillTable) return nullptr;
	if (const FName* RowName = RowNameByID.Find(SkillID))
		return SkillTable->FindRow<FSkillDataRow>(*RowName, TEXT("FindRowByID"));
	return nullptr;
}

int32 USkillSystemComponent::GetBaseID(ESkillSlot Slot) const
{
	if (const int32* ID = BaseSkillConfig.Find(Slot)) return *ID;
	return -1;
}

int32 USkillSystemComponent::GetCurrentSkillID(ESkillSlot Slot) const
{
	return SlotPanel ? SlotPanel->GetCurrentSkillID(Slot) : -1;
}

int32 USkillSystemComponent::SlotToIndex(ESkillSlot Slot)
{
	switch (Slot)
	{
	case ESkillSlot::Slot0: return 0; // LMB
	case ESkillSlot::Slot1: return 1; // 1
	case ESkillSlot::Slot2: return 2; // 2
	case ESkillSlot::Slot3: return 3; // 3
	case ESkillSlot::Slot4: return 4; // 4
	case ESkillSlot::SlotQ: return 5; // Q
	case ESkillSlot::SlotE: return 6; // E
	default: return -1;
	}
}

ESkillSlot USkillSystemComponent::IndexToSlot(int32 Index)
{
	switch (Index)
	{
	case 0: return ESkillSlot::Slot0;
	case 1: return ESkillSlot::Slot1;
	case 2: return ESkillSlot::Slot2;
	case 3: return ESkillSlot::Slot3;
	case 4: return ESkillSlot::Slot4;
	case 5: return ESkillSlot::SlotQ;
	case 6: return ESkillSlot::SlotE;
	default: return ESkillSlot::None;
	}
}

// ========== UI 브릿지 ==========
void USkillSystemComponent::HandleSlotIconChanged(ESkillSlot Slot, UTexture2D* NewIcon)
{
	UI_OnSetIcon.Broadcast(SlotToIndex(Slot), NewIcon);
}

void USkillSystemComponent::HandleSlotPairChanged(ESkillSlot Slot, UTexture2D* CurIcon, UTexture2D* NextIcon)
{
	UI_OnSetIconStep.Broadcast(SlotToIndex(Slot), CurIcon, NextIcon);
}

void USkillSystemComponent::HandleSlotCooldownTick(ESkillSlot Slot, float Remain, float Total, ECooldownUIType Type)
{
	bool isVisibleNum = false;

	if (Type == ECooldownUIType::Skill)
	{
		isVisibleNum = (Remain > 0.f);
	}
	else if (Type == ECooldownUIType::Global)
	{
		isVisibleNum = (Remain > 1.0f);
	}

	UI_OnCooldownTick.Broadcast(SlotToIndex(Slot), Remain, Total, isVisibleNum);
}

void USkillSystemComponent::OnTargetCCStateChange()
{
	if (CheckCC.IsValid())
	{
		LastCheckedCCType = CheckCC->GetCrowdControlType();
		LastCheckedStackCount = CheckCC->GetCurrentStack();
		UpdateAllSkillDisplays(CheckCC->GetOwner());
	}
}

void USkillSystemComponent::OnTargetImmuneStateChanged()
{
	if (LastCheckedBoss.IsValid())
	{
		UpdateAllSkillDisplays(LastCheckedBoss.Get());
	}
}

ASkillBase* USkillSystemComponent::UseSkillbyID(int32 SkillID, AActor* Target)
{
	if (!GetWorld() || !SkillTable) return nullptr;
	const FSkillDataRow* Row = FindRowByID(SkillID);
	if (!Row) return nullptr;

	if (!CanUseSkill(*Row, Target, false)) return nullptr;

	ASkillBase* skill = CreateSkill(*Row);
	if (!skill) return nullptr;

	if (TWeakObjectPtr<ASkillBase>* FoundActor = ActiveSkillActors.Find(Row->Slot))
	{
		if (FoundActor->IsValid())
		{
			FoundActor->Get()->CancelSkill_Implementation();
		}
	}
	ActiveSkillActors.Add(Row->Slot, skill);

	skill->InitFromRow(*Row);
	skill->SetSkillTarget(Target);
	ISkillInterface::Execute_ExecuteSkill(skill);

	FSlotRuntimeState Dummy;
	CommitStateAfterUse(*Row, Dummy);

	return skill;
}

bool USkillSystemComponent::IsReady(int32 SkillID) const
{
	if (const float* EndAt = CooldownEndAt.Find(SkillID))
	{
		return *EndAt <= GetWorld()->GetTimeSeconds();
	}
	return true;
}
