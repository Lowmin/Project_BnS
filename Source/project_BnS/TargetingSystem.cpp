// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingSystem.h"

#include "BnsController.h"
#include "CharacterBase.h"
#include "MainUi.h"
#include "TargetAble.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"


// Sets default values for this component's properties
UTargetingSystem::UTargetingSystem()
{
	// ...
}

void UTargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	// 틱 활성화 
	PrimaryComponentTick.Target = this;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	PrimaryComponentTick.RegisterTickFunction(GetComponentLevel());
	
	SetSphereRadius(400);
	SetHiddenInGame(false);

	OnComponentBeginOverlap.AddDynamic(this, &UTargetingSystem::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UTargetingSystem::OnOverlapEnd);
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	DpScale = GEngine->GameViewport->GetDPIScale();
}

void UTargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
		bnsController->MainUi->SetTarget(true, FVector2D::ZeroVector, FVector2D(100, 100));
	}
	else
	{
		// 좌표 계산 
		FVector pos = Target->GetWorldLocation();
		FVector2D screen;
		UGameplayStatics::ProjectWorldToScreen(playerController, pos, screen);

		float x = ((screen.X/ViewportSize.X) - 0.5f);
		float y = ((screen.Y/ViewportSize.Y) - 0.5f);

		// 크기 계산 
		FVector camPos = Cast<AMyPlayer>(bnsController->GetPawn())->FollowCamera->GetComponentLocation();
		float radius = 42.0f * 2.0f;
		float distance = FVector::Distance(pos, camPos);
		float fov = 90.0f * 0.5f;
		float targetBoxSize = radius / (distance * FMath::Tan(FMath::DegreesToRadians(fov)));
		targetBoxSize *= ((1 / DpScale) * ViewportSize.Y * 2);

		bnsController->MainUi->SetTarget(true, FVector2D(x, y), FVector2D(targetBoxSize, targetBoxSize));
	}
}

void UTargetingSystem::SetCurTarget()
{
	FVector pos = GetComponentLocation();
	FVector forward = GetForwardVector();
	
	float distance = std::numeric_limits<float>::max();

	for (ITargetAble* targetAble : TargetAbles)
	{
		FVector targetPos = targetAble->GetWorldLocation();
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

void UTargetingSystem::ValidateTarget()
{
	if(Target == nullptr)
		return;
	
	FVector pos = GetComponentLocation();
	FVector targetPos = Target->GetWorldLocation();
	
	FVector forward = GetForwardVector();
	FVector dir = (targetPos - pos).GetSafeNormal();
	float dot = FVector::DotProduct(forward, dir);

	// 45도 이상 벗어나면 타겟 해제 
	if(dot < 0.5f)
	{
		Target = nullptr;
	}
}

bool UTargetingSystem::IsTargetAble() const
{
	return Target != nullptr;
}

ACharacterBase* UTargetingSystem::GetTarget() const
{
	return nullptr;
}

void UTargetingSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ITargetAble* targetAble = Cast<ITargetAble>(OtherActor);
	if(targetAble != nullptr)
	{
		TargetAbles.Add(targetAble);
	}
}

void UTargetingSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

