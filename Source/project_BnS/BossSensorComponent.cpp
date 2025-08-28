// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSensorComponent.h"

#include "BossEnemy.h"

UBossSensorComponent::UBossSensorComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UBossSensorComponent::OnBossOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UBossSensorComponent::OnBossOverlapEnd);
}

void UBossSensorComponent::OnBossOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossEnemy* boss = Cast<ABossEnemy>(OtherActor);
	if (boss != nullptr)
	{
		BossList.Add(boss);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "find boss");
	}
}

void UBossSensorComponent::OnBossOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABossEnemy* boss = Cast<ABossEnemy>(OtherActor);
	if (boss != nullptr)
	{
		BossList.Remove(boss);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "missing boss");
	}
}

