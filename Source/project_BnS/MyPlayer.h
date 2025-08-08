// myplayer.h
#pragma once

#include "CoreMinimal.h"
#include "TargetingSystem.h"
#include "InputActionValue.h"
#include "USMoveComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyPlayer.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_Single_FF, float, float);

UCLASS()
class PROJECT_BNS_API AMyPlayer : public ATargetingSystem
{
    GENERATED_BODY()

public:
    AMyPlayer();

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputMappingContext* IC_Player;

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_Movement;

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_Jump;

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_Look;

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_Run;

    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_Glide;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyTPS_Cam, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = myTPS_Cam, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    // Skill
    UPROPERTY(EditAnywhere, Category = Input)
    class UInputAction* IA_BasicAttack;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    virtual void Jump() override;

    void SMoveToggle();
    void SGlidingToggle();

    // Skill
    void HandleBasicAttack(const FInputActionValue& Value);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USMoveComponent* MovementSystem;

    // Status
private:
    UPROPERTY(VisibleAnywhere, Category = "Status")
    float CurStamina = 10000.0f;
    UPROPERTY(VisibleAnywhere, Category = "Status")
    float MaxStamina = 10000.0f;
    UPROPERTY(VisibleAnywhere, Category = "Status")
    float CurExp = 0.0f;
    UPROPERTY(VisibleAnywhere, Category = "Status")
    float MaxExp = 100.0f;
public:
    FDele_Single_FF OnStaminaChange = nullptr;
    FDele_Single_FF OnExpChange = nullptr;
    
    void SetCurStamina(float stamina);
    float GetCurStamina() const;
    float GetMaxStamina() const;
    void AddExp(float exp);
    float GetCurExp() const;
    float GetMaxExp() const;
};