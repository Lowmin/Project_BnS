// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "MeleeSkill.generated.h"

UCLASS()
class PROJECT_BNS_API AMeleeSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void OnSkillNotify_Hit() override;

protected:
	virtual void OnSkillNotify_Custom(FName NotifyName) override;

};
