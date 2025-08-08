// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SkillInformation.h"
#include "SkillController.h"
#include "SkillSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BNS_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillSystemComponent();

	UFUNCTION(BlueprintCallable)
	void HandleBasicAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ExecuteComboAttack(const FSkillInformation* ComboData);
	void ResetCombo();

private:
	UPROPERTY(EditAnywhere, Category = "Combo")
	UDataTable* ComboSkillDataTable;

	UPROPERTY()
	TMap<FName, UAnimMontage*> LoadedMontages;

	int32 CurrentComboID = 0;
	FTimerHandle ComboTimerHandle;
	float LastComboDelay = 0.f;

	bool bIsComboPlaying = false;
	bool bIsComboNextInput = false;

	UFUNCTION()
	void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
