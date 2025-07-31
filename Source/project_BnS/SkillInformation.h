// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillInformation.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Melee,          // 기본 공격
    Projectile,     // 발사형 공격
    SelfEffect,     // 회복기
    AreaOfEffect,   // 장판형 공격
    SpecialAttack   // 합격기(필살기)
};

UENUM(BlueprintType)
enum class ETargetingType : uint8
{
    Self,
    TargetEnemy,
    GroundLocation
};

UENUM(BlueprintType)
enum class ESkillReqType : uint8
{
    NONE,
    ReqTarget,
    TargetCrowdControl,
    SelfCrowdControl
};


USTRUCT(BlueprintType)
struct FSkillInformation : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Index;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETargetingType TargetingType;


    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ManaCost = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CastRange = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESkillReqType ActivationRequirement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HealAmount = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EffectRadius = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EffectDuration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> AoEClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 NextComboSkillID = -1; // 다음 연계 스킬의 ID (-1이면 연계 없음)

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ComboAvailableTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 manaRecover = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> CastMontage;   // 애니메이션
};