// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsGameMode.h"

#include "BnsController.h"
#include "MyPlayer.h"
#include "UObject/ConstructorHelpers.h"

ABnsGameMode::ABnsGameMode()
{
	PlayerControllerClass = ABnsController::StaticClass();

	static ConstructorHelpers::FClassFinder<AMyPlayer> MyPlayerBPClass(TEXT("/Game/BP_MyPlayer.BP_MyPlayer_C"));

	if (MyPlayerBPClass.Succeeded())
	{
		DefaultPawnClass = MyPlayerBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find BP_MyPlayer. Using AMyPlayer as default pawn."));
		DefaultPawnClass = AMyPlayer::StaticClass();
	}
}
