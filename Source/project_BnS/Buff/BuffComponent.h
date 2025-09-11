// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BNS_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> BuffDataTable = nullptr;

	TMap<int32, struct FBuffData*> DataMap;
	void ParsingData();

	const struct FBuffData* GetBuffData(int32 buffIdx) const;

	/// <summary>
	/// 현재 적용된 버프 리스트 
	/// </summary>
	TArray<TObjectPtr<class UBuff>> BuffList;
	/// <summary>
	/// 매틱 실행되는 버프 리스트 
	/// </summary>
	std::vector<class IBuffTick*> BuffTickList;

	/// <summary>
	/// 매 틱 실행되어야 하는 버프 갱신 
	/// </summary>
	/// <param name="deltaTime"> 델타타임 </param>
	void BuffTick(float deltaTime);

	// 지속 시간 갱신 및 만료된 버프 제거
	void UpdateBuffDuration(float deltaTime);
	// 동일한 버프 확인
	UBuff* FindBuff(int32 buffIdx) const;

public:
	const TArray<class UBuff*> GetBuffList() const;

	/// <summary>
	/// 버프 추가 
	/// </summary>
	/// <param name="target"> 버프 적용 대상 </param>
	/// <param name="buffIdx"> 추가할 버프 인덱스 </param>
	void AddBuff(class ACharacterBase* target, int32 buffIdx);

	/// <summary>
	/// 현재 걸려있는 버프 체크  
	/// </summary>
	/// <param name="buffIdx"> 체크 할 버프 인덱스 </param>
	/// <returns> 버프 적용 여부 </returns>
	bool IsBuff(int32 buffIdx) const;
};
