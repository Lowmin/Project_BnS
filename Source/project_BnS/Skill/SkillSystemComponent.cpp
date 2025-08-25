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
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"

// 디버깅
#define SKILLDBG 1
#if SKILLDBG
#define SKLOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT(fmt), ##__VA_ARGS__)
#else
#define SKLOG(...)
#endif

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
	// 테이블 캐시
	BuildSkillCache();

	// 슬롯 패널 생성
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
bool USkillSystemComponent::UseSkillByIndex(int32 Index, AActor* Target)
{
	return UseSlot_Internal(IndexToSlot(Index), Target);
}

bool USkillSystemComponent::UseSkillBySlot(ESkillSlot Slot, AActor* Target)
{
	return UseSlot_Internal(Slot, Target);
}

// ========== 애니 노티 ==========
void USkillSystemComponent::Notify_InputOpen(ESkillSlot Slot)
{
	FSlotRuntimeState& SlotState = GetState(Slot);
	SlotState.bInputWindowOpen = true;
	SlotState.bQueuedInput = false;
	SKLOG("[Slot%02d] INPUT-OPEN t=%.3f (queued=%d)", (int)Slot, GetWorld()->GetTimeSeconds(), GetState(Slot).bQueuedInput);
}

void USkillSystemComponent::Notify_AnimUnlock(ESkillSlot Slot)
{
	// 락 해제. 큐가 있으면 단 한 번만 소비
	FSlotRuntimeState& SlotState = GetState(Slot);
	SlotState.AnimLockEndAt = 0.f;
	SlotState.bInputWindowOpen = false;

	const float Now = GetWorld()->GetTimeSeconds();
	const float Elapsed = Now - SlotState.CurStartTime;
	const bool HadQueue = SlotState.bQueuedInput;

	SlotState.AnimLockEndAt = 0.f;
	SlotState.bInputWindowOpen = false;

	SKLOG("[Slot%02d] UNLOCK t=%.3f elapsed=%.3f queued=%d", (int)Slot, Now, Elapsed, HadQueue);

	if (HadQueue && !SlotState.bConsumedThisMontage)
	{
		SlotState.bQueuedInput = false;
		SlotState.bConsumedThisMontage = true;
		AActor* Queued = SlotState.QueuedTarget.Get();
		SlotState.QueuedTarget = nullptr;
		UseSlot_Internal(Slot, Queued);
	}
}

void USkillSystemComponent::Notify_Hit(ESkillSlot Slot)
{
	if (ActiveSkillActors.Contains(Slot) && ActiveSkillActors[Slot].IsValid())
	{
		ASkillBase* CurrentSkillActor = ActiveSkillActors[Slot].Get();
		if (CurrentSkillActor)
		{
			CurrentSkillActor->OnSkillNotify_Hit();
		}
	}
}

void USkillSystemComponent::Notify_Custom(ESkillSlot Slot, FName NotifyName)
{
	if (ActiveSkillActors.Contains(Slot) && ActiveSkillActors[Slot].IsValid())
	{
		ASkillBase* CurrentSkillActor = ActiveSkillActors[Slot].Get();
		if (CurrentSkillActor)
		{
			CurrentSkillActor->OnSkillNotify_Custom(NotifyName);
		}
	}
}

// ========== 실행 파이프라인 ==========
bool USkillSystemComponent::UseSlot_Internal(ESkillSlot Slot, AActor* Target)
{
	if (!GetWorld() || !SkillTable || !SlotPanel) return false;

	FSlotRuntimeState& SlotState = GetState(Slot);
	const float Now = GetWorld()->GetTimeSeconds();

	// 1) 슬롯 락: 막혀 있으면 선입력 1회만 큐하고 종료
	if (Now < SlotState.AnimLockEndAt)
	{
		if (SlotState.bInputWindowOpen && !SlotState.bQueuedInput)
		{
			SlotState.bQueuedInput = true;
			SlotState.QueuedTarget = Target;
		}
		return false;
	}

	// 아이콘 교체
	const int32 UseID = ResolveSkillToUse(Slot);
	if (UseID <= 0) return false;
	const FSkillDataRow* Row = FindRowByID(UseID);
	if (!Row) return false;

	// 2) 글로벌 락 검사(우선순위/락무시 반영)
	if (IsGlobalLock(*Row, Now))
	{
		// 입력창 열려 있으면 1회만 큐
		if (SlotState.bInputWindowOpen && !SlotState.bQueuedInput)
		{
			SlotState.bQueuedInput = true;
			SlotState.QueuedTarget = Target;
		}
		return false;
	}

	if (!CanUseSkill(*Row, Target)) return false;

	// 3) MP
	if (Row->MpCost > 0 && CachedStat.IsValid())
	{
		const int32 Mp = CachedStat->GetCurMp();
		if (Mp < Row->MpCost) return false;
		CachedStat->SetCurMp(FMath::Max(0, Mp - Row->MpCost));
	}

	// 4) 스킬 액터 생성 + 실행
	ASkillBase* skill = CreateSkill(*Row);
	if (!skill)
	{
		// 스폰 실패시 소모된 MP 복구
		if (Row->MpCost > 0 && CachedStat.IsValid())
		{
			CachedStat->SetCurMp(CachedStat->GetCurMp() + Row->MpCost);
		}
		UpdateDisplayForSlot(Slot);
		return false;
	}
	ActiveSkillActors.Add(Slot, skill);

	SlotState.CurStartTime = Now;
	SlotState.bConsumedThisMontage = false;

	skill->InitFromRow(*Row);
	skill->SetSkillTarget(Target);

	ISkillInterface::Execute_ExecuteSkill(skill);

	// 5) 상태 커밋, UI
	CommitStateAfterUse(*Row, SlotState);
	UpdateDisplayForSlot(Slot);
	return true;
}

int32 USkillSystemComponent::ResolveSkillToUse(ESkillSlot Slot) const
{
	const float Now = GetWorld()->GetTimeSeconds();

	const int32 BaseID = GetBaseID(Slot);
	if (BaseID <= 0) return -1;

	const FSlotRuntimeState* SlotState = TryGetState(Slot);
	if (!SlotState || SlotState->LastUsedSkillID <= 0) return BaseID;

	// 마지막 사용 스킬
	const FSkillDataRow* Last = FindRowByID(SlotState->LastUsedSkillID);
	if (!Last) return BaseID;

	// 체인 유효시간이 지났으면 1타로 리셋
	if ((Now - SlotState->LastUsedSkillAt) > Last->ChainWindowSec)
		return BaseID;

	return (Last->ChainNextID > 0) ? Last->ChainNextID : BaseID;
}

bool USkillSystemComponent::CanUseSkill(const FSkillDataRow& Row, AActor* Target) const
{
	const float Now = GetWorld()->GetTimeSeconds();

	// 1) 쿨다운
	if (const float* EndAt = CooldownEndAt.Find(Row.SkillID))
	{
		if (*EndAt > Now) return false;
	}

	// 2) MP(코스트가 있으면 스탯 필요)
	if (Row.MpCost > 0)
	{
		if (!CachedStat.IsValid()) return false;
		if (CachedStat->GetCurMp() < Row.MpCost) return false;
	}

	// 3) 체인 스텝이면 연결 유효성 보장(안전망)
	if (Row.Layer == ESkillLayer::Chain)
	{
		const FSlotRuntimeState* SlotState = TryGetState(Row.Slot);
		if (!SlotState || SlotState->LastUsedSkillID < 0) return false;
		const FSkillDataRow* Last = FindRowByID(SlotState->LastUsedSkillID);
		if (!Last) return false;

		const bool Ok = (Last->ChainNextID == Row.SkillID) && ((Now - SlotState->LastUsedSkillAt) <= Last->ChainWindowSec);
		if (!Ok) return false;
	}

	// 4) 타겟 CC 요구(있는 스킬만)
	if (Row.NeedTargetCC.Num() > 0)
	{
		if (!Target) return false;
		const UCrowdControlComponent* TargetCC = Target->FindComponentByClass<UCrowdControlComponent>();
		if (!TargetCC) return false;
	}

	return true;
}

void USkillSystemComponent::CommitStateAfterUse(const FSkillDataRow& Row, FSlotRuntimeState& SlotState)
{
	const float Now = GetWorld()->GetTimeSeconds();

	// 1) 슬롯 락
	if (Row.AnimLockSec > 0.f)
	{
		SlotState.AnimLockEndAt = Now + Row.AnimLockSec;
	}

	// 글로벌 락
	if (Row.AnimLockSec > 0.f)
	{
		GlobalLockEndAt = FMath::Max(GlobalLockEndAt, Now + Row.AnimLockSec);
		GlobalLockPriority = Row.Priority;
		if (SlotPanel)
		{
			SlotPanel->PlayCooldownShow_All(GlobalLockEndAt, Row.AnimLockSec, true);
		}
	}

	// 2) 연계
	SlotState.LastUsedSkillID = Row.SkillID;
	SlotState.LastUsedSkillAt = Now;

	if (Row.ChainNextID > 0 && Row.ChainWindowSec > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SlotState.ChainTimer);
		GetWorld()->GetTimerManager().SetTimer(
			SlotState.ChainTimer,
			FTimerDelegate::CreateUObject(this, &USkillSystemComponent::OnChainExpire, Row.Slot),
			Row.ChainWindowSec, false);
	}
	else
	{
		SlotState.LastUsedSkillID = -1;
		SlotState.LastUsedSkillAt = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(SlotState.ChainTimer);
	}

	// 3) 쿨타임
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
	FSlotRuntimeState& SlotState = GetState(Slot);
	SlotState.LastUsedSkillID = -1;
	SlotState.LastUsedSkillAt = 0.f;
	SlotState.bQueuedInput = false;
	SlotState.bInputWindowOpen = false;

	if (SlotPanel) SlotPanel->ResetToBase(Slot);
}

void USkillSystemComponent::UpdateDisplayForSlot(ESkillSlot Slot)
{
	if (!SlotPanel) return;

	const int32 PrevID = SlotPanel->GetCurrentSkillID(Slot);
	const int32 NextID = ResolveSkillToUse(Slot);

	if (NextID <= 0)
	{
		SlotPanel->ResetToBase(Slot);
		return;
	}

	if (PrevID != NextID)
	{
		const FSkillDataRow* PrevRow = FindRowByID(PrevID);
		const FSkillDataRow* NextRow = FindRowByID(NextID);
		if (PrevRow && NextRow)
		{
			UTexture2D* PrevIcon = PrevRow->SkillIcon.LoadSynchronous();
			UTexture2D* NextIcon = NextRow->SkillIcon.LoadSynchronous();
			UI_OnAnimatedSetIcon.Broadcast(SlotToIndex(Slot), PrevIcon, NextIcon);
		}
	}

	SlotPanel->ShowSkill(Slot, NextID);
}

bool USkillSystemComponent::IsGlobalLock(const FSkillDataRow& Row, float Now) const
{
	// bOffLock 이 true면(회피/막기같은) 락 무시 가능
	if (Row.bOffLock) return false;

	// 시간이 락 안이면 막힘. 단, 더 높은 우선순위 스킬은 허용
	if (Now < GlobalLockEndAt && Row.Priority <= GlobalLockPriority)
	{
		return true; // 막힘
	}
	return false;
}

// ========== 스폰/캐시 ==========
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
	if (!SkillTable) return;

	//
	ensureAlwaysMsgf(SkillTable->GetRowStruct() == FSkillDataRow::StaticStruct(),
		TEXT("SkillTable row type mismatch. Expected FSkillDataRow."));

	const TArray<FName> Names = SkillTable->GetRowNames();
	RowNameByID.Reserve(Names.Num());

	for (const FName& Nm : Names)
	{
		const FSkillDataRow* Row = SkillTable->FindRow<FSkillDataRow>(Nm, TEXT("BuildSkillCache"));
		if (!Row || Row->SkillID < 0) continue;

		RowNameByID.FindOrAdd(Row->SkillID) = Nm;

		if (Row->Slot != ESkillSlot::None)
			CandidateIDsBySlot.FindOrAdd(Row->Slot).IDs.Add(Row->SkillID);
	}
}

const FSkillDataRow* USkillSystemComponent::FindRowByID(int32 SkillID) const
{
	if (!SkillTable) return nullptr;
	if (const FName* RowName = RowNameByID.Find(SkillID))
		return SkillTable->FindRow<FSkillDataRow>(*RowName, TEXT("FindRowByID"));
	return nullptr;
}

void USkillSystemComponent::GatherCandidatesForSlot(ESkillSlot Slot, TArray<const FSkillDataRow*>& OutRows) const
{
	OutRows.Reset();
	if (!SkillTable) return;

	if (const FSkillIDArray* Arr = CandidateIDsBySlot.Find(Slot))
	{
		OutRows.Reserve(Arr->IDs.Num());
		for (int32 ID : Arr->IDs)
			if (const FSkillDataRow* Row = FindRowByID(ID))
				OutRows.Add(Row);
	}
}

// ========== 유틸 ==========
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

// UI 브릿지
void USkillSystemComponent::HandleSlotIconChanged(ESkillSlot Slot, UTexture2D* NewIcon)
{
	UI_OnSetIcon.Broadcast(SlotToIndex(Slot), NewIcon);
}
void USkillSystemComponent::HandleSlotPairChanged(ESkillSlot Slot, UTexture2D* CurIcon, UTexture2D* NextIcon)
{
	UI_OnSetIconStep.Broadcast(SlotToIndex(Slot), CurIcon, NextIcon);
}
void USkillSystemComponent::HandleSlotCooldownTick(ESkillSlot Slot, float Remain, float Total)
{
	UI_OnCooldownTick.Broadcast(SlotToIndex(Slot), Remain, Total);
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
