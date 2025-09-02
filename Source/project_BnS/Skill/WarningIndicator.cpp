// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningIndicator.h"
#include "Components/DecalComponent.h"

// Sets default values
AWarningIndicator::AWarningIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
    if (DecalComponent)
    {
        const float DecalDepth = 200.0f;
        DecalComponent->DecalSize = FVector(DecalDepth, Radius * 2.0f, Radius * 2.0f);
    }
}

