// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingSystem.h"

#include "BnsController.h"
#include "TargetAble.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

// Sets default values for this component's properties
ATargetingSystem::ATargetingSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetSensor = CreateDefaultSubobject<USphereComponent>(TEXT("TargetSensors"));
	TargetSensor->SetSphereRadius(1000.0f);
	TargetSensor->SetHiddenInGame(false);
	TargetSensor->SetupAttachment(RootComponent);
	TargetSensor->SetCollisionProfileName(TEXT("TargetingSensor"));

	TargetSensor->OnComponentBeginOverlap.AddDynamic(this, &ATargetingSystem::OnOverlapBegin);
	TargetSensor->OnComponentEndOverlap.AddDynamic(this, &ATargetingSystem::OnOverlapEnd);
}

void ATargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	BnsController = Cast<ABnsController>(playerController);
	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FViewport::ViewportResizedEvent.AddUObject(this, &ATargetingSystem::OnViewportResized);

	OnViewportResized(GEngine->GameViewport->Viewport, 0);
}

void ATargetingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	BnsController = Cast<ABnsController>(playerController);

	// 타겟 유효성 검사 
	ValidateTarget();

	// 타겟 선정 
	if (Target == nullptr && TargetAbles.Num() > 0)
	{
		SetCurTarget();
	}

	// 타겟박스 계산 
	SetTargetBox();
}
void ATargetingSystem::OnViewportResized(FViewport* viewport, uint32 params)
{
	if (GEngine == nullptr)
		return;
	if (GEngine->GameViewport == nullptr)
		return;

	FIntPoint size = viewport->GetSizeXY();
	ViewportSize.X = size.X;
	ViewportSize.Y = size.Y;

	//GEngine->GameViewport->GetViewportSize(ViewportSize);
	float uiDpScale = UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport->GetWorld());
	UiViewportSize = ViewportSize * (1 / uiDpScale);
}

void ATargetingSystem::SetTargetBox()
{
	if (Target == nullptr)
		return;

	// 좌표 계산 
	FVector pos = ITargetAble::Execute_GetWorldLocation(Target);
	FVector2D screen;
	UGameplayStatics::ProjectWorldToScreen(BnsController, pos, screen);

	// viewport 기준 계산 
	float x = ((screen.X / ViewportSize.X) - 0.5f);
	float y = ((screen.Y / ViewportSize.Y) - 0.5f);

	// 실제 좌표 적용 
	x *= UiViewportSize.X;
	y *= UiViewportSize.Y;

	// 크기 계산 
	FVector camPos = CameraManager->GetCameraLocation();
	float distance = FVector::Distance(pos, camPos);
	float fov = CameraManager->GetFOVAngle() * 0.5f;
	float targetBoxRatio = (UiViewportSize.Y) / (distance * FMath::Tan(FMath::DegreesToRadians(fov)));

	FVector2D targetCenter = ITargetAble::Execute_GetTargetCenter(Target);
	FVector2D targetBoxSize = ITargetAble::Execute_GetTargetBoxSize(Target);

	targetBoxSize *= targetBoxRatio;

	if(OnTargetBoxChange.IsBound())
	{
		OnTargetBoxChange.Execute(targetCenter + FVector2D(x, y), targetBoxSize);
	}
}

void ATargetingSystem::RemoveCurrentTarget()
{
	ITargetAble::Execute_OnTargeted(Target, false);
	Target = nullptr;

	if (OnTargetBoxChange.IsBound())
	{
		OnTargetBoxChange.Execute(FVector2D::ZeroVector, FVector2D(100.0f, 100.0f));
	}
}

void ATargetingSystem::SetCurTarget()
{
	FVector pos = GetActorLocation();
	FVector forward = GetActorForwardVector();

	float distance = std::numeric_limits<float>::max();

	for (AActor* targetAble : TargetAbles)
	{
		// 비활성 대상 검사하지 않음 
		if (!ITargetAble::Execute_IsActiveTarget(targetAble))
			continue;

		FVector targetPos = ITargetAble::Execute_GetWorldLocation(targetAble);
		FVector dir = (targetPos - pos).GetSafeNormal();

		float dot = FVector::DotProduct(forward, dir);
		float distSqrt = FVector::DistSquared(pos, targetPos);

		// 45도 이내, 가장 짧은 거리 타겟 설정 
		if (dot > 0.5f && distSqrt < distance)
		{
			// 대상이 가려져있지 않은경우 등록 
			if (!IsTargetBlocked(targetPos))
			{
				distance = distSqrt;
				const ACharacterBase* characterBase = Cast<ACharacterBase>(targetAble);
				if (characterBase != nullptr)
				{
					Target = targetAble;
					ITargetAble::Execute_OnTargeted(Target, true);
				}
			}
		}
	}
}

void ATargetingSystem::ValidateTarget()
{
	if (Target == nullptr)
		return;
	UObject* obj = Cast<UObject>(Target);
	if (obj == nullptr)
	{
		RemoveCurrentTarget();
	}
	if (!ITargetAble::Execute_IsActiveTarget(obj))
	{
		RemoveCurrentTarget();
	}

	FVector pos = GetActorLocation();
	FVector targetPos = ITargetAble::Execute_GetWorldLocation(obj);

	FVector forward = GetActorForwardVector();
	FVector dir = (targetPos - pos).GetSafeNormal();
	float dot = FVector::DotProduct(forward, dir);

	// 45도 이상 벗어나면 타겟 해제 
	if (dot < 0.5f)
	{
		RemoveCurrentTarget();
	}

	// 대상이 가려져있는경우 타게팅 해제 
	if (IsTargetBlocked(targetPos))
	{
		RemoveCurrentTarget();
	}
}

bool ATargetingSystem::IsTargetBlocked(FVector targetPos)
{
	FVector pos = CameraManager->GetCameraLocation();
	FHitResult result;
	if (GetWorld()->LineTraceSingleByChannel(result, pos, targetPos, ECC_GameTraceChannel4))
	{
		return true;
	}
	return false;
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
	if (OtherActor->GetClass()->ImplementsInterface(UTargetAble::StaticClass()))
	{
		TargetAbles.Add(OtherActor);
	}
}

void ATargetingSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UTargetAble::StaticClass()))
	{
		TargetAbles.Remove(OtherActor);
		if (OtherActor == Target)
		{
			RemoveCurrentTarget();
		}
	}
}