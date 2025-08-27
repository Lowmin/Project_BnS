// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotController.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "TimerManager.h"

USkillSlotController::USkillSlotController()
{
}

void USkillSlotController::Initialize(UDataTable* InTable, AActor* InOwner, UWorld* InWorld)
{
	Table = InTable;
	Owner = InOwner;

	RowNameByID.Reset();

	if (!Table) return;

	const TArray<FName> RowName = Table->GetRowNames();
	RowNameByID.Reserve(RowName.Num());

	// ID -> Row Name 캐시
	for (const FName& Name : RowName)
	{
		const FSkillDataRow* Row = Table->FindRow<FSkillDataRow>(Name, TEXT("IDCache"));

		if (!Row) continue;
		if (Row->SkillID < 0) continue;
		if (RowNameByID.Contains(Row->SkillID)) continue;	// 중복 ID 방지

		RowNameByID.Emplace(Row->SkillID, Name);			// Key:ID, Value:RowName
	}
}

bool USkillSlotController::EquipBaseSkill(ESkillSlot SkillSlot, int32 SkillID)
{
	if (!Table) return false;
	const FSkillDataRow* Row = FindRowByID(SkillID);
	if (!Row) return false;
	if (Row->Slot != SkillSlot) return false;

	BaseIDBySlot.FindOrAdd(SkillSlot) = SkillID;
	CurIDBySlot.FindOrAdd(SkillSlot) = SkillID;

	UTexture2D* Icon = Row->SkillIcon.LoadSynchronous();

	OnSlotIconChanged.Broadcast(SkillSlot, Icon);
	return true;
}

void USkillSlotController::ResetToBase(ESkillSlot SkillSlot)
{
	const int32* BaseID = BaseIDBySlot.Find(SkillSlot);
	if (!BaseID) return;

	ShowSkill(SkillSlot, *BaseID);
}

bool USkillSlotController::ShowSkill(ESkillSlot SkillSlot, int32 SkillID)
{
	if (!Table) return false;
	const FSkillDataRow* Row = FindRowByID(SkillID);
	if (!Row) return false;

	CurIDBySlot.FindOrAdd(SkillSlot) = SkillID;

	UTexture2D* Icon = Row->SkillIcon.LoadSynchronous();

	OnSlotIconChanged.Broadcast(SkillSlot, Icon);
	return true;
}

bool USkillSlotController::ShowPairSkill(ESkillSlot SkillSlot, int32 CurSkillID, int32 NextSkillID)
{
	if (!Table) return false;
	const FSkillDataRow* Cur = FindRowByID(CurSkillID);
	const FSkillDataRow* Next = FindRowByID(NextSkillID);
	if (!Cur || !Next) return false;

	CurIDBySlot.FindOrAdd(SkillSlot) = CurSkillID;

	UTexture2D* CurIcon = Cur->SkillIcon.LoadSynchronous();
	UTexture2D* NextIcon = Next->SkillIcon.LoadSynchronous();

	OnSlotPairChanged.Broadcast(SkillSlot, CurIcon, NextIcon);
	return true;
}

int32 USkillSlotController::GetCurrentSkillID(ESkillSlot SkillSlot) const
{
	if (const int32* ID = CurIDBySlot.Find(SkillSlot))
	{
		return *ID;
	}
	return -1;
}

void USkillSlotController::PlayCooldownShow(ESkillSlot SkillSlot, int32 SkillID, float EndAt, float TotalSec)
{
	ShowEndAt.FindOrAdd(SkillSlot) = EndAt;
	ShowTotal.FindOrAdd(SkillSlot) = TotalSec;
	ShowType.FindOrAdd(SkillSlot) = ECooldownUIType::Skill;

	if (UWorld* World = GetWorldSafe())
	{
		if (FTimerHandle* Handle = ShowTickTimer.Find(SkillSlot))
		{
			World->GetTimerManager().ClearTimer(*Handle);
		}

		FTimerHandle NewHandle;
		World->GetTimerManager().SetTimer(NewHandle, FTimerDelegate::CreateUObject(this, &USkillSlotController::OnCooldownShowTick, SkillSlot), 0.05f, true);
		ShowTickTimer.FindOrAdd(SkillSlot) = NewHandle;

		OnCooldownShowTick(SkillSlot);
	}
}

void USkillSlotController::StopCooldownShow(ESkillSlot SkillSlot)
{
	if (UWorld* World = GetWorldSafe())
	{
		if (FTimerHandle* Handle = ShowTickTimer.Find(SkillSlot))
		{
			World->GetTimerManager().ClearTimer(*Handle);
		}
	}
}

void USkillSlotController::PlayCooldownShow_All(float EndAt, float TotalSec, bool bSkipSlotCool)
{
	UWorld* World = GetWorldSafe();
	if (!World) return;

	const float Now = World->GetTimeSeconds();

	ESkillSlot Slots[] =
	{
		ESkillSlot::Slot0,
		ESkillSlot::Slot1
		// 슬롯 늘어나면 추가
	};

	for (int i = 0; i < static_cast<int>(UE_ARRAY_COUNT(Slots)); i++)
	{
		ESkillSlot Slot = Slots[i];

		if (bSkipSlotCool)
		{
			const float curEnd = ShowEndAt.FindRef(Slot);
			if (curEnd > Now)
			{
				continue;
			}
		}

		ShowEndAt.FindOrAdd(Slot) = EndAt;
		ShowTotal.FindOrAdd(Slot) = TotalSec;
		ShowType.FindOrAdd(Slot) = ECooldownUIType::Global;

		FTimerHandle NewHandle;
		World->GetTimerManager().SetTimer(NewHandle, FTimerDelegate::CreateUObject(this, &USkillSlotController::OnCooldownShowTick, Slot), 0.05f, true);
		ShowTickTimer.FindOrAdd(Slot) = NewHandle;

		OnCooldownShowTick(Slot);
	}
}


const FSkillDataRow* USkillSlotController::FindRowByID(int32 SkillID) const
{
	if (!Table) return nullptr;
	const FName* RowName = RowNameByID.Find(SkillID);
	if (!RowName) return nullptr;

	return Table->FindRow<FSkillDataRow>(*RowName, TEXT("FindRowByID"));
}

void USkillSlotController::OnCooldownShowTick(ESkillSlot SkillSlot)
{
	if (UWorld* World = GetWorldSafe())
	{
		const float Total = ShowTotal.FindRef(SkillSlot);
		const float End = ShowEndAt.FindRef(SkillSlot);
		const float Now = World->GetTimeSeconds();
		const float Remain = FMath::Max(0.f, End - Now);
		const ECooldownUIType Type = ShowType.FindRef(SkillSlot);

		OnSlotCooldownTick.Broadcast(SkillSlot, Remain, Total, Type);
		if (Remain <= 0.f)
		{
			StopCooldownShow(SkillSlot);
		}
	}
}

UWorld* USkillSlotController::GetWorldSafe() const
{
	if (Owner.IsValid()) return Owner->GetWorld();
	return nullptr;
}
