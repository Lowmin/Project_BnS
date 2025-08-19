// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "WorldFloatingUiComponent.h"
#include "Nameplate.h"
#include "HpBar.h"
#include "EnemyData.h"

#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	UCapsuleComponent* capsuleComponent = GetCapsuleComponent();
	capsuleComponent->SetCollisionProfileName(TEXT("AAA"));
	
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
		HpBar->SetupAttachment(capsuleComponent);

		HpBar->SetWidgetClass(hpBarClass.Class);
		HpBar->SetDrawSize(FVector2D(266.0f, 26.0f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
		HpBar->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		HpBar->SetVisibility(false);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> nameplateClass(TEXT("/Game/UI/WBP_Nameplate.WBP_Nameplate_C"));
	if (nameplateClass.Succeeded())
	{
		Nameplate = CreateDefaultSubobject<UWidgetComponent>(TEXT("Nameplate"));
		Nameplate->SetupAttachment(HpBar);

		Nameplate->SetWidgetClass(nameplateClass.Class);
		Nameplate->SetWidgetSpace(EWidgetSpace::World);
		Nameplate->SetDrawSize(FVector2D(250, 30.f));
		Nameplate->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyDataTable && !EnemyRowName.IsNone())
	{
		const FEnemyData* Data = EnemyDataTable->FindRow<FEnemyData>(EnemyRowName, TEXT(""));

		if (Data)
		{
			GetMesh()->SetSkeletalMesh(Data->Mesh);
			GetMesh()->SetAnimClass(Data->AnimBP);

			Cast<UNameplate>(Nameplate->GetWidget())->SetNameplate(Data->D_EnemyName);
			Status->SetMaxHp(Data->D_MaxHP);
			Status->SetCurHp(Data->D_MaxHP);
			Status->SetLevel(Data->D_Level);
			Status->SetAtk(Data->D_Atk);
			Status->SetDef(Data->D_Def);
		}
	}

	Status->OnHpChange.AddUObject(Cast<UHpBar>(HpBar->GetWidget()), &UHpBar::OnChangeHp);
}

bool AEnemy::IsActiveTarget_Implementation() const
{
	return !IsDead();
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

void AEnemy::OnTargeted_Implementation(bool isTarget)
{
	HpBar->SetVisibility(isTarget);
}
