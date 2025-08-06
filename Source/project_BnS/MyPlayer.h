// myplayer.h
#pragma once

#include "CoreMinimal.h"
#include "TargetingSystem.h"
#include "InputActionValue.h"
#include "USMoveComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyPlayer.generated.h"

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
};