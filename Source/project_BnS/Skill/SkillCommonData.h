// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "SkillCommonData.generated.h"

class ASkillBase;

UENUM(BlueprintType)
enum class ESkill_Type : uint8
{
    Melee,          // 기본 공격
    Projectile,     // 발사형 공격
    Heal,           // 회복기
    Area,           // 장판형 공격
    Ultimate        // 합격기(필살기)
};

UENUM(BlueprintType)
enum class ETargeting_Type : uint8
{
    Self,
    TargetEnemy,
    Ground
};

USTRUCT(BlueprintType)
struct FSkillCCOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bUseCC = false; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bUseCC"))
    ECrowdControlType CCType = ECrowdControlType::None;     // None, Down, Sturn
};

USTRUCT(BlueprintType)
struct FSkillCommonData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Index = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESkill_Type SkillType = ESkill_Type::Melee;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETargeting_Type TargetingType = ETargeting_Type::Self;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSkillCCOption CC;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 CostMP = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> AnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageFlat = 0.f;     // 고정 대미지

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageRatio = 0.f;    // 공격력 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CastRange = 0.f;      // 스킬 시전 최대 거리

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinCastRange = 0.f;   // 스킬 시전 최소 거리

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName TypeDataRow;          // 타입별 상세 정보

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<class ASkillBase> SkillClass;    // 스킬 실행 클래스
};