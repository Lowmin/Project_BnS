// myplayer.cpp
#include "MyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Skill/SkillSystemComponent.h"
#include "StatComponent.h"
#include "BossSensorComponent.h"

AMyPlayer::AMyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    MovementSystem = CreateDefaultSubobject<USMoveComponent>(TEXT("MovementSystem"));

    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->SetupAttachment(RootComponent);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 500.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    Status->SetCharacterName("BNS_User");

    BossSensor = CreateDefaultSubobject<UBossSensorComponent>(TEXT("BossInfo"));
    BossSensor->SetupAttachment(RootComponent);
}

void AMyPlayer::BeginPlay()
{
    Super::BeginPlay();

    if (FastGlideEffectMaterial)
    {
        DynamicFastGlideMaterial = UMaterialInstanceDynamic::Create(FastGlideEffectMaterial, this);
        PostProcessComponent->AddOrUpdateBlendable(DynamicFastGlideMaterial);
    }

    CurrentBlurStrength = 0.f;
    TargetBlurStrength = 0.f;
}


void AMyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (MovementSystem && DynamicFastGlideMaterial)
    {
        TargetBlurStrength = (MovementSystem->GetMoveState() == EMoveState::FastGliding) ? 0.4f : 0.0f;

        CurrentBlurStrength = FMath::FInterpTo(CurrentBlurStrength, TargetBlurStrength, DeltaTime, 5.0f);

        DynamicFastGlideMaterial->SetScalarParameterValue(FName("BlurStrength"), CurrentBlurStrength);
    }
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedPlayerInputComponent != nullptr)
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController != nullptr)
        {
            UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
            if (EnhancedSubsystem != nullptr)
            {
                EnhancedSubsystem->AddMappingContext(IC_Player, 1);
            }
        }

        EnhancedPlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
        EnhancedPlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyPlayer::Jump);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMyPlayer::StopJumping);
        EnhancedPlayerInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &AMyPlayer::SMoveToggle);

        if (IA_BasicAttack)
        {
            EnhancedPlayerInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Started, this, &AMyPlayer::HandleBasicAttack);
        }
        if (IA_ProjectileSkill)
        {
            EnhancedPlayerInputComponent->BindAction(IA_ProjectileSkill, ETriggerEvent::Started, this, &AMyPlayer::HandleProjectileSkill);
        }
    }
}

void AMyPlayer::Move(const FInputActionValue& Value)
{

    if (MovementSystem && MovementSystem->GetMoveState() == EMoveState::WallRunning)
    {
        return;
    }

    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AMyPlayer::Jump()
{
    if (MovementSystem)
    {
        if (GetCharacterMovement() && GetCharacterMovement()->IsMovingOnGround())
        {
            MovementSystem->SJump();
        }
        else if (MovementSystem->GetMoveState() == EMoveState::WaterRunning)
        {
            MovementSystem->JumpOnWater();
        }
        else if (MovementSystem->GetMoveState() == EMoveState::WallRunning)
        {
            MovementSystem->WallJump();
        }
        else if (MovementSystem->GetMoveState() == EMoveState::Swim)
        {
            MovementSystem->JumpOnWater();
        }
        else
        {
            MovementSystem->GlideToggle();
        }
    }
}

void AMyPlayer::SMoveToggle()
{
    if (MovementSystem)
    {
        MovementSystem->SMoveToggle();
    }
}

void AMyPlayer::SGlidingToggle()
{
    if (MovementSystem)
    {
        MovementSystem->GlideToggle();
    }
}

void AMyPlayer::HandleBasicAttack(const FInputActionValue& Value)
{
    USkillSystemComponent* SkillSys = FindComponentByClass<USkillSystemComponent>();
    if (!SkillSys) return;

    const bool ok = SkillSys->UseSkillBySlot(ESkillSlot::Slot0, GetTarget());
}

void AMyPlayer::HandleProjectileSkill(const FInputActionValue& Value)
{
    USkillSystemComponent* SkillSys = FindComponentByClass<USkillSystemComponent>();
    if (!SkillSys) return;

    const bool ok = SkillSys->UseSkillBySlot(ESkillSlot::Slot1, GetTarget());
}

UBossSensorComponent* AMyPlayer::GetBossSensorComponent()
{
    return BossSensor;
}

void AMyPlayer::SetCurStamina(float stamina)
{
    if(FMath::IsNearlyEqual(CurStamina, stamina))
        return;
    
    CurStamina = stamina;

    OnStaminaChange.ExecuteIfBound(CurStamina, MaxStamina);
}

float AMyPlayer::GetCurStamina() const
{
    return CurStamina;
}

float AMyPlayer::GetMaxStamina() const
{
    return MaxStamina;
}

void AMyPlayer::AddExp(float exp)
{
    if(exp <= 0.f)
        return;
    
    CurExp += exp;

    OnExpChange.ExecuteIfBound(CurExp, MaxExp);
}

float AMyPlayer::GetCurExp() const
{
    return CurExp;
}

float AMyPlayer::GetMaxExp() const
{
    return MaxExp;
}
