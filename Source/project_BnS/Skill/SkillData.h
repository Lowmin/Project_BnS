#pragma once                             

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StructUtils/InstancedStruct.h"
#include "../CrowdControlComponent.h"
#include "Sound/SoundBase.h"
#include "SkillData.generated.h"

UENUM(BlueprintType)
enum class ECooldownUIType : uint8
{
	Skill,   // 개별 스킬 쿨타임
	Global   // GCD
};

class ASkillBase;
class AActor;
class UTexture2D;
class UParticleSystem;

// 스킬 슬롯 (입력)
UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	None	  UMETA(DisplayName = "None"),		// Enemy
	Slot0	  UMETA(DisplayName = "LMB"),		// 기본 공격
	Slot1     UMETA(DisplayName = "1"),			// 발사체
	Slot2     UMETA(DisplayName = "2"),
	Slot3     UMETA(DisplayName = "3"),
	Slot4     UMETA(DisplayName = "4"),
	SlotQ     UMETA(DisplayName = "Q"),
	SlotE     UMETA(DisplayName = "E")
};

// 각 슬롯 내 존재하는 스킬 분류 Layer
UENUM(BlueprintType)
enum class ESkillLayer : uint8
{
	// 발동 조건 없는 기본 공격
	Base		= 0,
	// 연계 스킬
	Chain		= 1,
	// 막기/회피시 활성화
	Proc		= 2,
	// 특정 조건 이후 활성화되는 스킬 (3콤보 후 강한 공격)
	Finisher	= 3,
	// 합격기 타이밍에 활성화되는 스킬
	BossCC		= 4 
};


// 스킬 타입 세팅	(Melee, Projectile, Heal/Buff, Area of Field, Dash Attack...)
USTRUCT(BlueprintType)
struct FSkillTypeBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSkillType_Melee : public FSkillTypeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackLength = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanHitMultiTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HitNotifyName = TEXT("Hit");
};

USTRUCT(BlueprintType)
struct FSkillType_Projectile : public FSkillTypeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Gravity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LifeSec = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> TrailVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<USoundBase> MovingSound;
};

USTRUCT(BlueprintType)
struct FSkillType_Area : public FSkillTypeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageTick = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TickRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 3.f;
};

// 스킬 공통 테이블
USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	int32 Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	int32 SkillID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName SkillName;

	// 입력 슬롯 (None:Enemy, 0:LMB, 1~E:Keyboard 1,2,3,4,Q,E)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	ESkillSlot Slot = ESkillSlot::None;

	// 스킬 계층 (Layer0:조건X, Layer1:콤보체인, Layer2:막기/회피 활성화, Layer3:일정 스택/마무리, Layer4:보스 합격기)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	ESkillLayer Layer = ESkillLayer::Base;

	// 같은 Layer 내 우선 순위 수치 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	int32 MpCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	float Damage = 1.f;

	// 스킬 발동 가능 최대 사거리 (ex. 발사체 800(8m))
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	float MaxRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	float CooldownSec = 0.f;

	// 스킬 사용 직후 다른 스킬 사용 불가 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	float AnimLockSec = 0.3f;

	// Combo (없으면 -1)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	int32 ChainNextID = -1;

	// Combo 연계 가능 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	float ChainWindowSec = 0.f;

	// 회피/막기 시 Lock 무시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	bool bOffLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	TSoftObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	TSoftObjectPtr<UTexture2D> SkillIcon = nullptr;

	// 캐스팅 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> CastVFX;

	// 캐스팅 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<USoundBase> CastSound;

	// 타격 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> HitVFX;

	// 타격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<USoundBase> HitSound;

	// CC 상태 부여
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd Control")
	ECrowdControlType ApplyCCType = ECrowdControlType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd Control")
	float ApplyCCDuration = 0.f;

	// 세팅한 리스트 상태(down, stun)가 모두 타겟에 있어야 사용 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	TArray<ECrowdControlType> NeedTargetCC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	TSubclassOf<ASkillBase> SkillActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	FInstancedStruct SkillTypeData;
};