// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSlot.h"

#include "Components/HorizontalBox.h"
#include "BuffIcon.h"


UBuffSlot::UBuffSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBuffIcon> buffIconClass(TEXT(""));
	if (buffIconClass.Succeeded())
	{
		BuffIconClass = buffIconClass.Class;
	}
}

void UBuffSlot::AddBuff(const FBuffData& data)
{
	if (BuffIconClass == nullptr)
		return;

	UBuffIcon* buffIcon = CreateWidget<UBuffIcon>(this, BuffIconClass);
	Root->AddChildToHorizontalBox(buffIcon);

	buffIcon->SetBuff(data);
}

void UBuffSlot::RemoveBuff(const FBuddData& data)
{
	
}
