// SkillSystemComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "SkillData.h"
#include "SkillSystemComponent.generated.h"

// UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUISetIcon, int32, Index, UTexture2D*, Icon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUISetIconStep, int32, Index, UTexture2D*, Cur, UTexture2D*, Next);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUICooldownTick, int32, Index, float, Remain, float, Total, bool, isVisibleNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUIAnimatedSetIcon, int32, Index, UTexture2D*, PrevIcon, UTexture2D*, NewIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUISkillUsable, int32, Index, bool, bIsUsable);

class UDataTable;
class USkillSlotController;
class ASkillBase;
class UStatComponent;
class UCrowdControlComponent;
class AActor;

// 슬롯에 속한 스킬들 캐싱
USTRUCT()
struct FSkillIDArray
{
	GENERATED_BODY()
	UPROPERTY() TArray<int32> IDs;
};

USTRUCT()
struct FSlotRuntimeState
{
	GENERATED_BODY()

	float        AnimLockEndAt = 0.f;		// 애니메이션 락 풀리는 시간

	int32        LastUsedSkillID = -1;		// 체인 유효 판단 (ID, 시간)
	float        LastUsedSkillAt = 0.f;

	FTimerHandle ChainTimer;				// 체인 유효 끝난 후 리셋 타이머

	bool         bInputWindowOpen = false;	// 창 열렸을 때 선입력(1번)
	bool         bQueuedInput = false;
	TWeakObjectPtr<AActor> QueuedTarget;

	float CurStartTime = 0.f;				// 이번 몽타주 시작 시각
	bool  bConsumedThisMontage = false;		// 이번 몽타주에서 큐를 이미 한번 썼는가
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillSystemComponent();
	virtual void BeginPlay() override;
	// Target 상태에 따른 스킬 사용 및 표시를 위해
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// ==== 데이터 ====
	UPROPERTY(EditAnywhere, Category = "Skill Config")
	TObjectPtr<UDataTable> SkillTable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Skill Config")
	TMap<ESkillSlot, int32> BaseSkillConfig;

	// ==== 외부 API ====
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool UseSkillBySlot(ESkillSlot Slot, AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool UseSkillByIndex(int32 Index, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UpdateAllSkillDisplays(AActor* CurrentTarget);

	// ==== 몽타주 노티파이 ====
	UFUNCTION(BlueprintCallable, Category = "Skill Notify")
	void Notify_InputOpen(ESkillSlot Slot);
	UFUNCTION(BlueprintCallable, Category = "Skill Notify")
	void Notify_AnimUnlock(ESkillSlot Slot);
	UFUNCTION(BlueprintCallable, Category = "Skill Notify")
	void Notify_Hit(ESkillSlot Slot);
	UFUNCTION(BlueprintCallable, Category = "Skill Notify")
	void Notify_Custom(ESkillSlot Slot, FName NotifyName);

	// ==== UI 델리게이트 =====
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnUISetIcon       UI_OnSetIcon;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnUISetIconStep   UI_OnSetIconStep;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnUICooldownTick  UI_OnCooldownTick;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnUIAnimatedSetIcon UI_OnAnimatedSetIcon;
	UPROPERTY(BlueprintAssignable, Category = "Skill UI")
	FOnUISkillUsable UI_OnSkillUsable;

	UFUNCTION(BlueprintPure, Category = "Skill Debug")
	int32 GetCurrentSkillID(ESkillSlot Slot) const;


	// Enemy 보조
	UFUNCTION(BlueprintCallable, Category = "Skill Enemy")
	ASkillBase* UseSkillbyID(int32 SkillID, AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Skill Enemy")
	bool IsReady(int32 SkillID) const;

private:
	// ==== 실행 파이프라인 ====
	bool  UseSlot_Internal(ESkillSlot Slot, AActor* Target);
	bool  CanUseSkill(const FSkillDataRow& Row, AActor* Target, bool bCheckMPCost = true) const;
	void  CommitStateAfterUse(const FSkillDataRow& Row, FSlotRuntimeState& SlotState);
	void  OnChainExpire(ESkillSlot Slot);
	void  UpdateDisplayForSlot(ESkillSlot Slot, AActor* Target);
	bool  IsGlobalLock(const FSkillDataRow& Row, float Now) const;

	// ==== 스킬 결정 ====
	int32 ResolveSkillToExecute(ESkillSlot Slot, AActor* Target) const;
	int32 ResolveChainSkill(ESkillSlot Slot) const;
	const FSkillDataRow* FindHighestPrioritySkillForSlot(ESkillSlot Slot, AActor* Target) const;
	bool CheckActivationConditions(const FSkillDataRow& Row, AActor* Target) const;

	void RefreshCooldownViewForSlot(ESkillSlot Slot, int32 SkillID);

	// ==== 스폰 / 캐시 / 유틸 ====
	ASkillBase* CreateSkill(const FSkillDataRow& Row);
	void  BuildSkillCache();
	const FSkillDataRow* FindRowByID(int32 SkillID) const;
	int32 GetBaseID(ESkillSlot Slot) const;
	static int32      SlotToIndex(ESkillSlot Slot);
	static ESkillSlot IndexToSlot(int32 Index);
	FSlotRuntimeState& GetState(ESkillSlot Slot) { return SlotRuntimeStates.FindOrAdd(Slot); }
	const FSlotRuntimeState* TryGetState(ESkillSlot Slot) const { return SlotRuntimeStates.Find(Slot); }

	// ==== SlotController 브릿지 ====
	UFUNCTION()
	void HandleSlotIconChanged(ESkillSlot Slot, UTexture2D* NewIcon);
	UFUNCTION()
	void HandleSlotPairChanged(ESkillSlot Slot, UTexture2D* CurIcon, UTexture2D* NextIcon);
	UFUNCTION()
	void HandleSlotCooldownTick(ESkillSlot Slot, float Remain, float Total, ECooldownUIType Type);

	// ==== 타겟 상태변화 ====
	UFUNCTION()
	void OnTargetCCStateChange();

	UFUNCTION()
	void OnTargetImmuneStateChanged();

private:
	UPROPERTY(Transient)
	TObjectPtr<USkillSlotController> SlotPanel = nullptr;	// UI
	UPROPERTY(Transient)
	TMap<ESkillSlot, FSlotRuntimeState> SlotRuntimeStates;	// 실행 상태
	UPROPERTY(Transient)
	TMap<int32, float> CooldownEndAt;						// SkillID, 쿨 종료 시각
	UPROPERTY(Transient)
	float GlobalLockEndAt = 0.f;							// GCD
	UPROPERTY(Transient)
	int32 GlobalLockPriority = 0;

	UPROPERTY(Transient)
	TMap<ESkillSlot, bool> LastSkillUsable;


	// ==== 노티파이용 ==== 
	UPROPERTY(Transient)
	TMap<ESkillSlot, TWeakObjectPtr<ASkillBase>> ActiveSkillActors;

	// ==== 테이블 탐색 캐시 ====
	UPROPERTY(Transient)
	TMap<int32, FName> RowNameByID;
	UPROPERTY(Transient)
	TMap<ESkillSlot, FSkillIDArray> CandidateIDsBySlot;

	// ==== 아이콘 ====
	UPROPERTY(Transient)
	TMap<int32, TObjectPtr<UTexture2D>> IconCache;

	// ==== 외부 시스템 ====
	TWeakObjectPtr<UStatComponent>         CachedStat;
	TWeakObjectPtr<UCrowdControlComponent> CachedOwnerCC;

	// ==== Tick 최적화 상태 캐시 ====
	TWeakObjectPtr<AActor> LastCheckedTarget;
	ECrowdControlType LastCheckedCCType = ECrowdControlType::None;
	bool bLastTargetValid = false;
	int32 LastCheckedStackCount = 0;
	int32 LastCheckedMP = 0;

	TWeakObjectPtr<UCrowdControlComponent> CheckCC;
	TWeakObjectPtr<class ABossEnemy> LastCheckedBoss;
};