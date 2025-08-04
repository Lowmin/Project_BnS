// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"

AEnemy::AEnemy()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skMeshRes(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	if (skMeshRes.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skMeshRes.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

FVector2D AEnemy::GetTargetCenter_Implementation()
{
	return FVector2D::ZeroVector;
}

FVector2D AEnemy::GetTargetSize_Implementation()
{
	return FVector2D(200.0f, 200.0f);
}
