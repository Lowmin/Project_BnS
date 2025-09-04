// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"

#include "../CharacterBase.h"
#include "Buff.h"
#include "BuffTick.h"

// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UDataTable> res(TEXT("/Game/DT_BuffData.DT_BuffData"));
	if (res.Succeeded())
	{
		BuffDataTable = res.Object;
	}
}


// Called when the game starts
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	ParsingData();
}


// Called every frame
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BuffTick(DeltaTime);
}

void UBuffComponent::ParsingData()
{
	if (!BuffDataTable)
		return;

	// Load the UDataTable
	TArray<FBuffData*> arr;
	BuffDataTable->GetAllRows<FBuffData>("buffdata", arr);

	if (arr.Num() <= 0)
		return;

	for (FBuffData* data : arr)
	{
		DataMap.Add(data->Idx, data);
	}
}

const FBuffData* UBuffComponent::GetBuffData(int32 buffIdx) const
{
	if (DataMap.Contains(buffIdx))
		return DataMap[buffIdx];

	return nullptr;
}

void UBuffComponent::BuffTick(float deltaTime)
{
	for (IBuffTick* buff : BuffTickList)
	{
		buff->BuffTick(deltaTime);
	}

	auto a = GetBuffList();
}

void UBuffComponent::UpdateBuffDuration(float deltaTime)
{
	for (int32 i = BuffList.Num() - 1; i >= 0; --i)
	{
		UBuff* buff = BuffList[i];
		if (buff == nullptr) continue;

		ACharacterBase* target = buff->GetTarget();
		bool bRemove = false;

		if (!target || target->IsDead())
		{
			bRemove = true;
		}
		else if (buff->CheckTickDuration(deltaTime))
		{
			bRemove = true;
		}

		if (bRemove)
		{
			// 버프 종료 델리게이트
			buff->OnBuffFinish.Broadcast(buff->GetBuffData());

			// 틱 리스트에 있는 버프 제거
			IBuffTick* buffTick = Cast<IBuffTick>(buff);
			if (buffTick)
			{
				BuffTickList.erase(std::remove(BuffTickList.begin(), BuffTickList.end(), buffTick), BuffTickList.end());
			}
			BuffList.RemoveAt(i);
		}
	}
}

const TArray<class UBuff*> UBuffComponent::GetBuffList() const
{
	return BuffList;
}

void UBuffComponent::AddBuff(class ACharacterBase* target, int32 buffIdx)
{
	const FBuffData* data = GetBuffData(buffIdx);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "add buff");
	if (data == nullptr)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "add buff 2");
	UBuff* buff = NewObject<UBuff>(this, data->BuffClass);
	buff->SetBuffData(data);
	buff->SetTarget(target);
	BuffList.Add(buff);

	IBuffTick* buffTick = Cast<IBuffTick>(buff);
	if (buffTick)
	{
		BuffTickList.push_back(buffTick);
	}
}

bool UBuffComponent::IsBuff(int32 buffIdx) const
{
	for (UBuff* buff : BuffList)
	{
		if (buff->GetBuffData().Idx == buffIdx)
			return true;
	}

	return false;
}

