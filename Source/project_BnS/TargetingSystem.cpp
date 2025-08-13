// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingSystem.h"

#include "BnsController.h"
#include "CharacterBase.h"
#include "MainUi.h"
#include "MainUIPresenter.h"
#include "TargetAble.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "Components/SphereComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

// Sets default values for this component's properties
ATargetingSystem::ATargetingSystem()
{
	// ...
	//Primar Tick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;

	TargetSensor = CreateDefaultSubobject<USphereComponent>(TEXT("TargetSensors"));
	TargetSensor->SetSphereRadius(1000);
	TargetSensor->SetHiddenInGame(false);
	TargetSensor->SetupAttachment(RootComponent);

	TargetSensor->OnComponentBeginOverlap.AddDynamic(this, &ATargetingSystem::OnOverlapBegin);
	TargetSensor->OnComponentEndOverlap.AddDynamic(this, &ATargetingSystem::OnOverlapEnd);
}

void ATargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	GEngine->GameViewport->GetViewportSize(ViewportSize);
	float uiDpScale = UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport->GetWorld());
	UiViewportSize = ViewportSize * (1 / uiDpScale);
}

void ATargetingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟 유효성 검사 
	ValidateTarget();

	// 타겟 선정 
	if(Target == nullptr && TargetAbles.Num() > 0)
	{
		SetCurTarget();
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	ABnsController* bnsController = Cast<ABnsController>(playerController);
	
	if(Target == nullptr)
	{
		bnsController->UIPresenter->OnTargetChange(true, FVector2D::ZeroVector, FVector2D(100, 100));
	}
	else
	{
		UObject* obj = Cast<UObject>(Target);
		if (obj == nullptr)
		{
			Target = nullptr;
			return;
		}
		// 좌표 계산 
		FVector pos = ITargetAble::Execute_GetWorldLocation(obj);
		FVector2D screen;
		UGameplayStatics::ProjectWorldToScreen(playerController, pos, screen);

		float x = ((screen.X/ViewportSize.X) - 0.5f);
		float y = ((screen.Y/ViewportSize.Y) - 0.5f);

		x *= UiViewportSize.X;
		y *= UiViewportSize.Y;

		// 크기 계산 
		FVector camPos = Cast<AMyPlayer>(bnsController->GetPawn())->FollowCamera->GetComponentLocation();
		float radius = 42.0f * 2.0f;
		float distance = FVector::Distance(pos, camPos);
		float fov = 90.0f * 0.5f;
		float targetBoxSize = radius / (distance * FMath::Tan(FMath::DegreesToRadians(fov)));
		targetBoxSize *= (UiViewportSize.Y);

		bnsController->UIPresenter->OnTargetChange(true, FVector2D(x, y), FVector2D(targetBoxSize, targetBoxSize));
	}
}

void ATargetingSystem::SetCurTarget()
{
	FVector pos = GetActorLocation();
	FVector forward = GetActorForwardVector();
	
	float distance = std::numeric_limits<float>::max();

	for (ITargetAble* targetAble : TargetAbles)
	{
		UObject* target = Cast<UObject>(targetAble);
		if (target == nullptr)
			continue;
		FVector targetPos = ITargetAble::Execute_GetWorldLocation(target);
		FVector dir = (targetPos - pos).GetSafeNormal();
		float dot = FVector::DotProduct(forward, dir);

		float distSqrt = FVector::DistSquared(pos, targetPos);

		// 45도 이내, 가장 짧은 거리 타겟 설정 
		if(dot > 0.5f && distSqrt < distance)
		{
			distance = distSqrt;
			const ACharacterBase*  characterBase = Cast<ACharacterBase>(targetAble);
			if(characterBase != nullptr)
			{
				Target = targetAble;
			}
		}
	}
}

void ATargetingSystem::ValidateTarget()
{
	if(Target == nullptr)
		return;
	UObject* obj = Cast<UObject>(Target);
	if (obj == nullptr)
	{
		Target = nullptr;
	}
	
	FVector pos = GetActorLocation();
	FVector targetPos = ITargetAble::Execute_GetWorldLocation(obj);
	
	FVector forward = GetActorForwardVector();
	FVector dir = (targetPos - pos).GetSafeNormal();
	float dot = FVector::DotProduct(forward, dir);

	// 45도 이상 벗어나면 타겟 해제 
	if(dot < 0.5f)
	{
		Target = nullptr;
	}
}

bool ATargetingSystem::IsTarget() const
{
	return Target != nullptr;
}

ACharacterBase* ATargetingSystem::GetTarget() const
{
	return Cast<ACharacterBase>(Target);
}

void ATargetingSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ITargetAble* targetAble = Cast<ITargetAble>(OtherActor);
	if(targetAble != nullptr)
	{
		TargetAbles.Add(targetAble);
	}
}

void ATargetingSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ITargetAble* targetAble = Cast<ITargetAble>(OtherActor);
	
	if(targetAble != nullptr)
	{
		TargetAbles.Remove(targetAble);
		if(targetAble == Target)
		{
			Target = nullptr;
		}
	}
}

