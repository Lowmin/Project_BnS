// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningIndicator.h"
#include "Components/DecalComponent.h"

// Sets default values
AWarningIndicator::AWarningIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("WarningDecal"));
	RootComponent = DecalComponent;

	DecalComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DecalComponent->DecalSize = FVector(256.f, 256.f, 256.f);

}

// Called when the game starts or when spawned
void AWarningIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarningIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarningIndicator::SetDecalSize(float Radius)
{
	UE_LOG(LogTemp, Warning, TEXT("[WarningIndicator] SetDecalSize function CALLED. Received Radius: %.1f"), Radius);

	if (DecalComponent)
	{
		const float DecalDepth = 200.0f;
		DecalComponent->DecalSize = FVector(DecalDepth, Radius, Radius);
		DecalComponent->MarkRenderStateDirty();

		UE_LOG(LogTemp, Warning, TEXT("[WarningIndicator] DecalComponent Size SET TO: %s"), *DecalComponent->DecalSize.ToString());
	}
}

