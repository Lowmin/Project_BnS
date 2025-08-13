// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h" 
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

    void Setup(UDataTable* InCommon);
    void RegisterTypeTable(ESkill_Type Type, UDataTable* Table);
    void SetOwnerActor(AActor* InOwner);

    bool Execute(FName CommonRowName);
    bool Execute(FName CommonRowName, AActor* InTarget);
    bool TryExecuteSkill(FName CommonRowName) { return Execute(CommonRowName); }

    float GetChainInputTimeFor(FName CommonRowName) const;

private:
    const FSkillCommonData* FindCommon(FName Row) const;
    UDataTable* GetTypeTable(uint8 Type) const;

    float Now() const;
    bool IsCooldownReady(FName Row, float CooldownSec) const;
    void StampCooldown(FName Row);

private:
    UPROPERTY()
    UDataTable* DT_SkillCommon = nullptr;
    UPROPERTY()
    TMap<uint8, UDataTable*> TypeTables;
    UPROPERTY()
    AActor* OwnerActor = nullptr;
    UPROPERTY()
    TMap<FName, float> LastUsedAt;
};

