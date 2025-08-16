// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillCommonData.h"
#include "SkillController.generated.h"

class UDataTable;
class ASkillBase;
class AActor;

UCLASS()
class PROJECT_BNS_API USkillController : public UObject
{
	GENERATED_BODY()

public:
	USkillController();

	void Setup(UDataTable* InCommonDataTable);
	void RegisterTypeTable(ESkill_Type Type, UDataTable* TypeDataTable);
	void SetOwnerActor(AActor* InOwner);

	bool Execute(FName SkillRowName, AActor* Target = nullptr);

	// 데이터 조회
	const FSkillCommonData* FindCommonData(FName SkillRowName) const;
	float GetCurrentTime() const;

private:
	UDataTable* GetTypeTable(ESkill_Type Type) const;
	bool IsCooldownReady(FName SkillRowName) const;
	void StampCooldown(FName SkillRowName, float CooldownDuration);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> DT_SkillCommon = nullptr;

	UPROPERTY()
	TMap<ESkill_Type, TObjectPtr<UDataTable>> TypeTables;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY()
	TMap<FName, float> LastUsedTimestamps;
};
