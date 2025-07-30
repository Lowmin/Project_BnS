// Fill out your copyright notice in the Description page of Project Settings.


#include "BnsGameMode.h"

#include "BnsController.h"
#include "MyPlayer.h"

ABnsGameMode::ABnsGameMode()
{
	PlayerControllerClass = ABnsController::StaticClass();
	DefaultPawnClass = AMyPlayer::StaticClass();
}
