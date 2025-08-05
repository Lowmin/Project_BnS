// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingSystem.h"

#include "BnsController.h"
#include "CharacterBase.h"
#include "MainUi.h"
#include "TargetAble.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTargetingSystem::UTargetingSystem()
{
	// ...
}

void UTargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	// 틱 활성화 
	PrimaryComponentTick.Target = this;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	PrimaryComponentTick.RegisterTickFunction(GetComponentLevel());
	
	SetSphereRadius(400);
	SetHiddenInGame(false);

	OnComponentBeginOverlap.AddDynamic(this, &UTargetingSystem::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UTargetingSystem::OnOverlapEnd);
}

void UTargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UE_LOG(LogActor, Warning, TEXT("Tick"));

	FVector2D size;
	GEngine->GameViewport->GetViewportSize(size);
	
	UE_LOG(LogActor, Warning, TEXT("Size : %s"), *size.ToString());

	auto con = GetWorld()->GetFirstPlayerController();
	ABnsController* bcon = Cast<ABnsController>(con);

	FVector pos(2520, 1320, 96); 
	FVector2D screen;
	UGameplayStatics::ProjectWorldToScreen(con, pos, screen);
	UE_LOG(LogActor, Warning, TEXT("screen : %s"), *screen.ToString());

	float x = ((screen.X/size.X) - 0.5f);
	float y = ((screen.Y/size.Y) - 0.5f);
	bcon->MainUi->SetTarget(true, FVector2D(x, y), FVector2D(200, 200));
}

void UTargetingSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogActor, Warning, TEXT("Overlap : %s"), *OtherActor->GetActorNameOrLabel());

	ITargetAble* targetAble = Cast<ITargetAble>(OtherActor);
	
	if(targetAble != nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Is TargetAble"));
	}
}

void UTargetingSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogActor, Warning, TEXT("overlap end : %s"), *OtherActor->GetActorNameOrLabel());
}

