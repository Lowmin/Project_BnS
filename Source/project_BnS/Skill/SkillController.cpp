// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillController.h"
#include "SkillBase.h"
#include "GameFramework/Actor.h"         
#include "GameFramework/Pawn.h"      
#include "Engine/World.h" 
#include "Animation/AnimMontage.h"
#include "Curves/CurveFloat.h"

USkillController::USkillController()
{
}

void USkillController::Setup(UDataTable* InCommonDataTable)
{
	DT_SkillCommon = InCommonDataTable;
	TypeTables.Empty();
}

void USkillController::RegisterTypeTable(ESkill_Type Type, UDataTable* TypeDataTable)
{
	if (TypeDataTable)
	{
		TypeTables.Add(Type, TypeDataTable);
	}
}

void USkillController::SetOwnerActor(AActor* InOwner)
{
	OwnerActor = InOwner;
}

bool USkillController::Execute(FName SkillRowName, AActor* Target)
{
	UWorld* World = OwnerActor ? OwnerActor->GetWorld() : nullptr;
	if (!World) return false;

	const FSkillCommonData* CommonData = FindCommonData(SkillRowName);
	if (!CommonData) return false;
	if (!CommonData->SkillClass) return false;

	if (!IsCooldownReady(SkillRowName)) return false;

	UAnimMontage* PreloadedMontage = CommonData->AnimMontage.LoadSynchronous();
	UCurveFloat* PreloadedCurve = CommonData->DamageLevelCurve.LoadSynchronous();

	// 스킬 액터 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);

	ASkillBase* SpawnedSkill =
		World->SpawnActor<ASkillBase>(CommonData->SkillClass.Get(), OwnerActor->GetActorLocation(), OwnerActor->GetActorRotation(), SpawnParams);

	if (!SpawnedSkill) return false;

	FDataTableRowHandle CommonHandle;
	CommonHandle.DataTable = DT_SkillCommon;
	CommonHandle.RowName = SkillRowName;

	FDataTableRowHandle TypeHandle;
	if (UDataTable* TypeTable = GetTypeTable(CommonData->SkillType))
	{
		TypeHandle.DataTable = TypeTable;
		TypeHandle.RowName = CommonData->TypeDataRow;
	}

	SpawnedSkill->InitializeSkill(CommonHandle, TypeHandle, PreloadedMontage, PreloadedCurve);

	if (Target)
	{
		SpawnedSkill->SetSkillTarget(Target);
	}

	ISkillInterface::Execute_ExecuteSkill(SpawnedSkill);

	StampCooldown(SkillRowName, CommonData->Cooldown);

	return true;
}

const FSkillCommonData* USkillController::FindCommonData(FName SkillRowName) const
{
	if (!DT_SkillCommon) return nullptr;
	return DT_SkillCommon->FindRow<FSkillCommonData>(SkillRowName, TEXT("FindCommonData"));
}

float USkillController::GetCurrentTime() const
{
	return GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
}

UDataTable* USkillController::GetTypeTable(ESkill_Type Type) const
{
	if (const TObjectPtr<UDataTable>* FoundTable = TypeTables.Find(Type))
	{
		return *FoundTable;
	}
	return nullptr;
}

bool USkillController::IsCooldownReady(FName SkillRowName) const
{
	// 쿨다운중인지
	const FSkillCommonData* Data = FindCommonData(SkillRowName);
	if (!Data || Data->Cooldown <= 0.f)
	{
		return true;
	}

	// 사용 기록 있는지
	const float* LastUsedTime = LastUsedTimestamps.Find(SkillRowName);
	if (!LastUsedTime)
	{
		return true;
	}

	return GetCurrentTime() >= (*LastUsedTime + Data->Cooldown);
}

void USkillController::StampCooldown(FName SkillRowName, float CooldownDuration)
{
	if (CooldownDuration > 0.f)
	{
		LastUsedTimestamps.FindOrAdd(SkillRowName) = GetCurrentTime();
	}
}

