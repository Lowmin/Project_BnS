// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "../MyPlayer.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALootBox::OnInteract_Implementation(AActor* Interactor)
{
	AMyPlayer* Player = Cast<AMyPlayer>(Interactor);
	if (Player)
	{
		Player->ShowLootPopup(ContainedItems);

		SetActorEnableCollision(false);
	}
}

