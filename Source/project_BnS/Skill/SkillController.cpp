// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillController.h"
#include "SkillCommonData.h" 
#include "MeleeData.h"				// 체인 시간용
#include "SkillBase.h"
#include "GameFramework/Actor.h"         
#include "GameFramework/Pawn.h"      
#include "Engine/World.h" 

USkillController::USkillController()
{
}

void USkillController::Setup(UDataTable* InCommon)
{
	DT_SkillCommon = InCommon;
	TypeTables.Empty();
}

void USkillController::RegisterTypeTable(ESkill_Type Type, UDataTable* Table)
{
	TypeTables.Add(static_cast<uint8>(Type), Table);
}

void USkillController::SetOwnerActor(AActor* InOwner)
{
	OwnerActor = InOwner;
}

bool USkillController::Execute(FName CommonRowName)
{
	if (!DT_SkillCommon || !OwnerActor) return false;

	const FSkillCommonData* CommonRow = FindCommon(CommonRowName);
	if (!CommonRow) return false;
	if (!IsCooldownReady(CommonRowName, CommonRow->Cooldown)) return false;

	if (!CommonRow->SkillClass) return false;

	UWorld* WorldPtr = OwnerActor->GetWorld();
	if (!WorldPtr) return false;

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = OwnerActor;
	SpawnParam.Instigator = Cast<APawn>(OwnerActor);

	// 스킬 액터 스폰, 위치
	ASkillBase* SpawnSkill =
		WorldPtr->SpawnActor<ASkillBase>(CommonRow->SkillClass.Get(), OwnerActor->GetActorLocation(), OwnerActor->GetActorRotation(), SpawnParam);

	if (!SpawnSkill) return false;

	// 공통 데이터 핸들
	FDataTableRowHandle CommonHandle;
	CommonHandle.DataTable = DT_SkillCommon;
	CommonHandle.RowName = CommonRowName;

	// 타입 데이터 핸들
	FDataTableRowHandle TypeHandle;
	if (UDataTable* TypeDataTable = GetTypeTable(static_cast<uint8>(CommonRow->SkillType)))
	{
		TypeHandle.DataTable = TypeDataTable;
		TypeHandle.RowName = CommonRow->TypeDataRow;
	}

	SpawnSkill->InitSkill(CommonHandle, TypeHandle);
	ISkillInterface::Execute_InitSkillExecute(SpawnSkill);
	ISkillInterface::Execute_ExecuteSkill(SpawnSkill);

	// 마지막 사용 시간 저장
	StampCooldown(CommonRowName);
	return Execute(CommonRowName, nullptr);
}

bool USkillController::Execute(FName CommonRowName, AActor* InTarget)
{
	if (!DT_SkillCommon || !OwnerActor) return false;

	const FSkillCommonData* CommonRow = FindCommon(CommonRowName);
	if (!CommonRow) return false;
	if (!IsCooldownReady(CommonRowName, CommonRow->Cooldown)) return false;
	if (!CommonRow->SkillClass) return false;

	UWorld* WorldPtr = OwnerActor->GetWorld();
	if (!WorldPtr) return false;

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = OwnerActor;
	SpawnParam.Instigator = Cast<APawn>(OwnerActor);

	ASkillBase* SpawnSkill =
		WorldPtr->SpawnActor<ASkillBase>(CommonRow->SkillClass.Get(), OwnerActor->GetActorLocation(), OwnerActor->GetActorRotation(), SpawnParam);
	if (!SpawnSkill) return false;

	// 타겟 전달
	SpawnSkill->SetSkillTarget(InTarget);

	FDataTableRowHandle CommonHandle;
	CommonHandle.DataTable = DT_SkillCommon;
	CommonHandle.RowName = CommonRowName;

	FDataTableRowHandle TypeHandle;
	if (UDataTable* TypeDataTable = GetTypeTable(static_cast<uint8>(CommonRow->SkillType)))
	{
		TypeHandle.DataTable = TypeDataTable;
		TypeHandle.RowName = CommonRow->TypeDataRow;
	}

	SpawnSkill->InitSkill(CommonHandle, TypeHandle);
	ISkillInterface::Execute_InitSkillExecute(SpawnSkill);
	ISkillInterface::Execute_ExecuteSkill(SpawnSkill);

	StampCooldown(CommonRowName);

	return true;
}

float USkillController::GetChainInputTimeFor(FName CommonRowName) const
{
	// 콤보 입력 대기 시간
	const FSkillCommonData* CommonRow = FindCommon(CommonRowName);
	if (!CommonRow) return 0.f;

	if (CommonRow->SkillType == ESkill_Type::Melee)
	{
		if (UDataTable* DataTable = GetTypeTable(static_cast<uint8>(CommonRow->SkillType)))
		{
			if (const FMeleeData* NextMelee = DataTable->FindRow<FMeleeData>(CommonRow->TypeDataRow, TEXT("ChainLookup")))
			{
				return NextMelee->ChainInput;
			}
		}
	}

	return 0.0f;
}

const FSkillCommonData* USkillController::FindCommon(FName Row) const
{
	if (!DT_SkillCommon) return nullptr;
	return DT_SkillCommon->FindRow<FSkillCommonData>(Row, TEXT("SkillCommonLookup"));
}

UDataTable* USkillController::GetTypeTable(uint8 Type) const
{
	if (UDataTable* const* Found = TypeTables.Find(Type))
	{
		return *Found;
	}
	return nullptr;
}

float USkillController::Now() const
{
	const UWorld* WorldExist = GetWorld();
	return WorldExist ? WorldExist->GetTimeSeconds() : 0.f;
}

bool USkillController::IsCooldownReady(FName Row, float CooldownSec) const
{
	if (CooldownSec <= 0.f) return true;
	const float* Last = LastUsedAt.Find(Row);
	if (!Last) return true;
	return Now() >= (*Last + CooldownSec);
}

void USkillController::StampCooldown(FName Row)
{
	// 쿨타임 시작 시간
	LastUsedAt.FindOrAdd(Row) = Now();
}




