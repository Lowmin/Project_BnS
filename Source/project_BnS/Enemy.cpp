// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "WorldFloatingUiComponent.h"
#include "Nameplate.h"
#include "HpBar.h"
#include "StatComponent.h"

#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skMeshRes(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	if (skMeshRes.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skMeshRes.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> hpBarClass(TEXT("/Game/UI/WBP_IngameHpBar.WBP_IngameHpBar_C"));
	if (hpBarClass.Succeeded())
	{
		HpBar = CreateDefaultSubobject<UWorldFloatingUiComponent>(TEXT("HpBar"));
		HpBar->SetupAttachment(RootComponent);

		HpBar->SetWidgetClass(hpBarClass.Class);
		HpBar->SetDrawSize(FVector2D(80.f, 8.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> nameplateClass(TEXT("/Game/UI/WBP_Nameplate.WBP_Nameplate_C"));
	if (nameplateClass.Succeeded())
	{
		Nameplate = CreateDefaultSubobject<UWidgetComponent>(TEXT("Nameplate"));
		Nameplate->SetupAttachment(HpBar);

		Nameplate->SetWidgetClass(nameplateClass.Class);
		Nameplate->SetWidgetSpace(EWidgetSpace::World);
		Nameplate->SetDrawSize(FVector2D(250, 30.f));
		Nameplate->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
		Nameplate->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Cast<UNameplate>(Nameplate->GetWidget())->SetNameplate(TEXT("Enemy"));
	Status->OnHpChange.AddUObject(Cast<UHpBar>(HpBar->GetWidget()), &UHpBar::OnChangeHp);
}

FVector AEnemy::GetWorldLocation_Implementation() const
{
	return GetActorLocation();
}

FVector2D AEnemy::GetTargetCenter_Implementation() const
{
	return FVector2D::ZeroVector;
}

FVector2D AEnemy::GetTargetBoxSize_Implementation() const
{
	float radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	return FVector2D(radius*2, radius*2);
}