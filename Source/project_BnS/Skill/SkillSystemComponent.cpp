// SkillSystemComponent.cpp
#include "SkillSystemComponent.h"
#include "SkillSlotController.h"
#include "SkillBase.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"
#include "../CrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Texture2D.h"

USkillSystemComponent::USkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		UE_LOG(LogTemp, Error, TEXT("SkillSystem: Failed to spawn SkillActor. MP refunded."), Row->SkillID);
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
	int32 MaxPriority = TNumericLimits<int32>::Min();
	for (const int32 SkillID : CandidateIDs->IDs)
	{
		const FSkillDataRow* CurrentRow = FindRowByID(SkillID);
		if (!CurrentRow) continue;
		if (CurrentRow->Layer == ESkillLayer::Chain || CurrentRow->Layer == ESkillLayer::Base) continue;
		if (CheckActivationConditions(*CurrentRow, Target) && CurrentRow->Priority > MaxPriority)
		{
			BestSkillRow = CurrentRow;
			MaxPriority = CurrentRow->Priority;
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
	case ESkillLayer::BossCC:
	{
		if (Row.NeedTargetCC.Num() == 0) return true;
		if (!Target) return false;
		if (const UCrowdControlComponent* TargetCC = Target->FindComponentByClass<UCrowdControlComponent>())
		{
			// CC 처리 필요
			return true;
		}
	}
	}
	return false;
}

bool USkillSystemComponent::CanUseSkill(const FSkillDataRow& Row, AActor* Target) const
{
	const float Now = GetWorld()->GetTimeSeconds();
	if (const float* EndAt = CooldownEndAt.Find(Row.SkillID))
	{
		if (*EndAt > Now) return false;
	}
	if (Row.MpCost != 0)
	{
		if (!CachedStat.IsValid()) return false;
		if (Row.MpCost > 0)
		{
			if (CachedStat->GetCurMp() < Row.MpCost) return false;
		}
		else
		{
			if (CachedStat->GetCurMp() >= CachedStat->GetMaxMp()) return false;
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
		if (!Target->FindComponentByClass<UCrowdControlComponent>()) return false;
	}
	return true;
}

void USkillSystemComponent::CommitStateAfterUse(const FSkillDataRow& Row, FSlotRuntimeState& SlotState)
{
	const float Now = GetWorld()->GetTimeSeconds();
	if (Row.AnimLockSec > 0.f)
	{
		SlotState.AnimLockEndAt = Now + Row.AnimLockSec;
		GlobalLockEndAt = FMath::Max(GlobalLockEndAt, SlotState.AnimLockEndAt);
		GlobalLockPriority = Row.Priority;
		if (SlotPanel)
		{
			SlotPanel->PlayCooldownShow_All(GlobalLockEndAt, Row.AnimLockSec, true);
		}
	}
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
	if (Row.Layer == ESkillLayer::Base && Row.CooldownSec > 0.f)
	{
		float& End = CooldownEndAt.FindOrAdd(Row.SkillID);
		if (End <= Now)
		{
			End = Now + Row.CooldownSec;
			if (SlotPanel)
			{
				SlotPanel->PlayCooldownShow(Row.Slot, Row.SkillID, End, Row.CooldownSec);
			}
		}
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
	if (PrevID == NextID) return;
	if (NextID <= 0)
	{
		SlotPanel->ResetToBase(Slot);
		return;
	}
	UTexture2D* PrevIcon = IconCache.FindRef(PrevID);
	UTexture2D* NextIcon = IconCache.FindRef(NextID);
	if (PrevIcon && NextIcon)
	{
		UI_OnAnimatedSetIcon.Broadcast(SlotToIndex(Slot), PrevIcon, NextIcon);
	}
	SlotPanel->ShowSkill(Slot, NextID);
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
	const bool isVisibleNum = (Type == ECooldownUIType::Global);

	UI_OnCooldownTick.Broadcast(SlotToIndex(Slot), Remain, Total, isVisibleNum);
}

// Enemy: 단발 실행
bool USkillSystemComponent::EnemyUseBasicMelee(int32 SkillID, AActor* Target)
{
	if (!GetWorld() || !SkillTable) return false;
	const FSkillDataRow* Row = FindRowByID(SkillID);
	if (!Row || Row->Layer != ESkillLayer::Base) return false;
	if (!CanUseSkill(*Row, Target)) return false;

	ASkillBase* skill = CreateSkill(*Row);
	if (!skill) return false;

	skill->InitFromRow(*Row);
	skill->SetSkillTarget(Target);
	ISkillInterface::Execute_ExecuteSkill(skill);

	FSlotRuntimeState Dummy;
	CommitStateAfterUse(*Row, Dummy);
	return true;
}
