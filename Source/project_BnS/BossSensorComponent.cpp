// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSensorComponent.h"

#include "BossEnemy.h"

#include "BnsController.h"
#include "UI/MainUIPresenter.h"

UBossSensorComponent::UBossSensorComponent()
{
	// 초기화 호출을 위해 추가 
	bWantsInitializeComponent = true;

	// 틱 연산을 위해 추가 
	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic(this, &UBossSensorComponent::OnBossOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UBossSensorComponent::OnBossOverlapEnd);
}

void UBossSensorComponent::RemoveTargetBoss()
{
	Target->GetStatusComponent()->OnHpChange.RemoveAll(this);
	Target->GetCrowdControlComponent()->OnCCInfoChange.Unbind();
	Target->OnImmuneStateBegan.Clear();
	Target->OnImmuneStateEnded.Clear();
	Target = nullptr;

	if (OnBossInfoChange.IsBound())
	{
		OnBossInfoChange.Execute(nullptr, 0, 0.0f);
	}

	SetTargetBoss();
}

void UBossSensorComponent::SetTargetBoss()
{
	if (Target != nullptr)
		return;

	for (ABossEnemy* boss : BossList)
	{
		Target = boss;
		break;
	}

	if(Target != nullptr)
	{
		int activateCrowdControlCount = 0;
		Target->GetStatusComponent()->OnHpChange.AddUObject(this, &UBossSensorComponent::BossHpChange);
		Target->GetCrowdControlComponent()->OnCCInfoChange.BindUObject(this, &UBossSensorComponent::BossCCInfoChange);
		Target->OnImmuneStateBegan.AddDynamic(this, &UBossSensorComponent::BossImmuneBegan);
		Target->OnImmuneStateEnded.AddDynamic(this, &UBossSensorComponent::BossImmuneEnded);
		activateCrowdControlCount = Target->GetCrowdControlComponent()->GetActivateStackCount();

		if (OnBossInfoChange.IsBound())
		{
			FVector pos = GetOwner()->GetActorLocation();
			FVector target = Target->GetActorLocation();

			OnBossInfoChange.Execute(Target->GetStatusComponent(), activateCrowdControlCount, FVector::Distance(pos, target) * 0.01f);
		}
	}
}

void UBossSensorComponent::BossHpChange(float current, float max)
{
	if (OnBossHpChange.IsBound())
	{
		OnBossHpChange.Execute(current, max);
	}
}

void UBossSensorComponent::BossCCInfoChange(ECrowdControlType type, int32 count)
{
	if (OnBossCCInfoChange.IsBound())
	{
		OnBossCCInfoChange.Execute(type, count);
	}
}

void UBossSensorComponent::BossImmuneBegan()
{
	if (OnBossCCInfoChange.IsBound())
	{
		OnBossCCInfoChange.Execute(ECrowdControlType::Immune, 0);
	}
}

void UBossSensorComponent::BossImmuneEnded()
{
	if (OnBossCCInfoChange.IsBound())
	{
		OnBossCCInfoChange.Execute(ECrowdControlType::None, 0);
	}
}

void UBossSensorComponent::OnBossOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossEnemy* boss = Cast<ABossEnemy>(OtherActor);
	if (boss != nullptr)
	{
		BossList.Add(boss);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "find boss");
		
		SetTargetBoss();
	}
}

void UBossSensorComponent::OnBossOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABossEnemy* boss = Cast<ABossEnemy>(OtherActor);
	if (boss != nullptr)
	{
		BossList.Remove(boss);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "missing boss");

		if (boss == Target)
		{
			RemoveTargetBoss();
		}
	}
}

void UBossSensorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetSphereRadius(5000.0f);
	SetHiddenInGame(false);
	SetCollisionProfileName(TEXT("TargetingSensor"));
}

void UBossSensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Target != nullptr)
	{
		if (OnBossDistanceChange.IsBound())
		{
			FVector pos = GetOwner()->GetActorLocation();
			FVector target = Target->GetActorLocation();

			OnBossDistanceChange.Execute(FVector::Distance(pos, target) * 0.01f);
		}
	}
}

