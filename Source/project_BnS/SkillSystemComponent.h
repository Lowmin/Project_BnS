// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	int32 CurrentComboID = 0;
	FTimerHandle ComboTimerHandle;
		
};
