// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSlot.h"

#include "Components/HorizontalBox.h"
#include "BuffIcon.h"
#include "project_BnS/Buff/BuffData.h"


UBuffSlot::UBuffSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBuffIcon> buffIconClass(TEXT("/Game/UI/Buff/WBP_BuffIcon.WBP_BuffIcon_C"));
	if (buffIconClass.Succeeded())
	{
		BuffIconClass = buffIconClass.Class;
	}
}

void UBuffSlot::AddBuff(const FBuffData& data)
{
	if (BuffIconClass == nullptr)
		return;
	UBuffIcon* buffIcon = nullptr;
	for(UBuffIcon* icon : BuffIcons)
	{
		if(icon->GetBuffID() == data.Idx)
		{
			buffIcon = icon;
			break;
		}
	}

	if(buffIcon == nullptr)
	{
		buffIcon = CreateWidget<UBuffIcon>(this, BuffIconClass);
		Root->AddChildToHorizontalBox(buffIcon);
		BuffIcons.Add(buffIcon);
	}
	
	buffIcon->SetBuff(data);
}

void UBuffSlot::RemoveBuff(const FBuffData& data)
{
	for(int i=0; i<BuffIcons.Num(); ++i)
	{
		if(BuffIcons[i]->GetBuffID() == data.Idx)
		{
			BuffIcons[i]->RemoveFromParent();
			BuffIcons.RemoveAt(i);
			return;
		}
	}
}
