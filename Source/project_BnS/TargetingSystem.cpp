// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingSystem.h"
#include "CharacterBase.h"
#include "TargetAble.h"


// Sets default values for this component's properties
UTargetingSystem::UTargetingSystem()
{
	// ...
	
	//TargetSensor = CreateDefaultSubobject<USphereComponent>(TEXT("Sensor");
	


	//SetSphereRadius(400.0f);
	//SetHiddenInGame(false);
}

void UTargetingSystem::BeginPlay()
{
	SetSphereRadius(400);
	SetHiddenInGame(false);

	OnComponentBeginOverlap.AddDynamic(this, &UTargetingSystem::OnOverlapBegin);
}

void UTargetingSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogActor, Warning, TEXT("Overlap : %s"), *OtherActor->GetName());

	ITargetAble* targetAble = Cast<ITargetAble>(OtherActor);
	
	if(targetAble != nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Is TargetAble"));
	}
}

