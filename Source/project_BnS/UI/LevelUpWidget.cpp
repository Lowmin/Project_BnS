// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpWidget.h"
#include "../CharacterBase.h"

void ULevelUpWidget::SetLevelUpText(int32 Level)
{
	ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;

	UStatComponent* StatComponent = PlayerCharacter->GetStatusComponent();
	if (!StatComponent) return;

	FString LevelString = FString::Printf(TEXT("%d"), StatComponent->GetLevel());

	if (Text_Level)
	{
		Text_Level->SetText(FText::AsNumber(Level));
	}

	if (Text_LevelUpText)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Level"), Level);

		FText FormatText = NSLOCTEXT("LevelUpUI", "LevelUpMessage", "캐릭터가 {Level} 레벨로 성장하였습니다.");

		Text_LevelUpText->SetText(FText::Format(FormatText, Args));
	}
}
