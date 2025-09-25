// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "StatComponent.h"
#include "CrowdControlComponent.h"

#include "Skill/SkillSystemComponent.h"

#include "Buff/BuffComponent.h"

#include "Sound/SoundBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// statComponent 생성 
	Status = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	// CrowdControlComponent 생성
	CrowdControl = CreateDefaultSubobject<UCrowdControlComponent>(TEXT("CrowdControl"));
	// skillComponent 생성
	SkillSystem = CreateDefaultSubobject<USkillSystemComponent>(TEXT("SkillSystem"));
	// BuffComponent 생성
	BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Attack()
{
	
}

bool ACharacterBase::IsDead() const
{
	return Status->GetCurHp() <= 0.0f;
}

void ACharacterBase::OnDamaged(int32 damage)
{
	Status->SetCurHp(Status->GetCurHp() - damage);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}

void ACharacterBase::Die()
{
	
}

UStatComponent* ACharacterBase::GetStatusComponent() const
{
	return Status;
}

const FString& ACharacterBase::GetCharacterName() const
{
	return Status->GetCharacterName();
}

inline float ACharacterBase::GetCurHp() const
{
	return Status->GetCurHp();
}

inline float ACharacterBase::GetMaxHp() const
{
	return Status->GetMaxHp();
}

inline int ACharacterBase::GetCurMp() const
{
	return Status->GetCurMp();
}

inline int ACharacterBase::GetCharacterLevel() const
{
	return Status->GetLevel();
}

UCrowdControlComponent* ACharacterBase::GetCrowdControlComponent()
{
	return CrowdControl;
}

void ACharacterBase::SetCrowdControl(ECrowdControlType type, float duration)
{
	CrowdControl->ApplyCrowdControl(type, duration);
}

ECrowdControlType ACharacterBase::GetCrowdControlType()
{
	return CrowdControl->GetCrowdControlType();
}

UBuffComponent* ACharacterBase::GetBuffComponent() const
{
	return BuffComponent;
}