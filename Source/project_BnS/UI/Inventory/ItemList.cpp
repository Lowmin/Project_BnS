// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemList.h"

#include "EquipSlot.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "ItemSlot.h"
#include "../../Inventory/ItemData.h"
#include "../../Inventory/Item.h"

UItemList::UItemList(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryRow(TEXT("/Game/UI/Inventory/WBP_InventoryRow.WBP_InventoryRow_C"));
	if (inventoryRow.Succeeded())
	{
		InventoryRowClass = inventoryRow.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryLock(TEXT("/Game/UI/Inventory/WBP_InventoryLock.WBP_InventoryLock_C"));
	if (inventoryLock.Succeeded())
	{
		InventoryLockClass = inventoryLock.Class;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> allEnable(TEXT("/Game/Image/Inventory/Category_All_Enable.Category_All_Enable"));
	if (allEnable.Succeeded())
	{
		TextureAllEnable = allEnable.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> allDisable(TEXT("/Game/Image/Inventory/Category_All_Disable.Category_All_Disable"));
	if(allDisable.Succeeded())
	{
		TextureAllDisable = allDisable.Object;
	}
		static ConstructorHelpers::FObjectFinder<UTexture2D> equipEnable(TEXT("/Game/Image/Inventory/Category_Equip_Enable.Category_Equip_Enable"));
		if(equipEnable.Succeeded())
		{
			TextureEquipEnable = equipEnable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> equipDisable (TEXT("/Game/Image/Inventory/Category_Equip_Disable.Category_Equip_Disable"));
		{
			TextureEquipDisable = equipDisable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> soulShieldEnable(TEXT("/Game/Image/Inventory/Category_SoulShield_Enable.Category_SoulShield_Enable"));
		if(soulShieldEnable.Succeeded())
		{
			TextureSoulShieldEnable = soulShieldEnable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> soulShieldDisable(TEXT("/Game/Image/Inventory/Category_SoulShield_Disable.Category_SoulShield_Disable"));
		if(soulShieldDisable.Succeeded())
		{
			TextureSoulShieldDisable = soulShieldDisable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> usableEnable(TEXT("/Game/Image/Inventory/Category_Usable_Enable.Category_Usable_Enable"));
		if(usableEnable.Succeeded())
		{
			TextureUsableEnable = usableEnable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> usableDisable(TEXT("/Game/Image/Inventory/Category_Usable_Disable.Category_Usable_Disable"));
		if(usableDisable.Succeeded())
		{
			TextureUsableDisable = usableDisable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> materialEnable(TEXT("/Game/Image/Inventory/Category_Material_Enable.Category_Material_Enable"));
		if(materialEnable.Succeeded())
		{
			TextureMaterialEnable = materialEnable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> materialDisable(TEXT("/Game/Image/Inventory/Category_Material_Disable.Category_Material_Disable"));
		if(materialDisable.Succeeded())
		{
			TextureMaterialDisable = materialDisable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> etcEnable(TEXT("/Game/Image/Inventory/Category_Etc_Enable.Category_Etc_Enable"));
		if(etcEnable.Succeeded())
		{
			TextureEtcEnable = etcEnable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UTexture2D> etcDisable(TEXT("/Game/Image/Inventory/Category_Etc_Disable.Category_Etc_Disable"));
		if(etcDisable.Succeeded())
		{
			TextureEtcDisable = etcDisable.Object;
		}
}

bool UItemList::Initialize()
{
	if(!Super::Initialize())
		return false;

	if (InventoryRowClass == nullptr)
		return false;
	if (InventoryLockClass == nullptr)
		return false;

	// Root->ClearChildren();

	for(int i=0; i<5; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryRowClass);
		Root->AddChild(widget);

		UHorizontalBox* box = Cast<UHorizontalBox>(widget->GetRootWidget());
		for (int j = 0; j < box->GetChildrenCount(); ++j)
		{
			UItemSlot* itemSlot = Cast<UItemSlot>(box->GetChildAt(j));
			itemSlot->SetIndex(ItemSlots.Num());
			ItemSlots.Add(itemSlot);
		}
	}

	for (int i = 5; i < 11; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryLockClass);
		Root->AddChild(widget);
	}
	
	return true;
}

void UItemList::NativeConstruct()
{
	Super::NativeConstruct();

	BtnAll->OnClicked.AddDynamic(this, &UItemList::HighlightAll);
	BtnEquip->OnClicked.AddDynamic(this, &UItemList::HighlightEquip);
	BtnSoulShield->OnClicked.AddDynamic(this, &UItemList::HighlightSoulShield);
	BtnUsable->OnClicked.AddDynamic(this, &UItemList::HighlightUsable);
	BtnMaterial->OnClicked.AddDynamic(this, &UItemList::HighlightMaterial);
	BtnEtc->OnClicked.AddDynamic(this, &UItemList::HighlightEtc);
}

void UItemList::HighlightAll()
{
	SetCategoryImage(EItemCategory::All);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::All);
	}
}

void UItemList::HighlightEquip()
{
	SetCategoryImage(EItemCategory::Equip);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Equip);
	}
}

void UItemList::HighlightSoulShield()
{
	SetCategoryImage(EItemCategory::SoulShield);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::SoulShield);
	}
}

void UItemList::HighlightUsable()
{
	SetCategoryImage(EItemCategory::Usable);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Usable);
	}
}

void UItemList::HighlightMaterial()
{
	SetCategoryImage(EItemCategory::Material);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Material);
	}
}

void UItemList::HighlightEtc()
{
	SetCategoryImage(EItemCategory::Etc);

	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Etc);
	}
}

void UItemList::SetCategoryImage(EItemCategory category)
{
	SetButtonImage(BtnAll, category == EItemCategory::All ? TextureAllEnable : TextureAllDisable);
	SetButtonImage(BtnEquip, category == EItemCategory::Equip ? TextureEquipEnable : TextureEquipDisable);
	SetButtonImage(BtnSoulShield, category == EItemCategory::SoulShield ? TextureSoulShieldEnable : TextureSoulShieldDisable);
	SetButtonImage(BtnUsable, category == EItemCategory::Usable ? TextureUsableEnable : TextureUsableDisable);
	SetButtonImage(BtnMaterial, category == EItemCategory::Material ? TextureMaterialEnable : TextureMaterialDisable);
	SetButtonImage(BtnEtc, category == EItemCategory::Etc ? TextureEtcEnable : TextureEtcDisable);
}

void UItemList::SetButtonImage(UButton* btn, UTexture2D* texture)
{
	FButtonStyle buttonStyle = btn->GetStyle();
	if (buttonStyle.Normal.GetResourceObject() == texture)
		return;

	buttonStyle.Normal.SetResourceObject(texture);
	buttonStyle.Hovered.SetResourceObject(texture);
	buttonStyle.Disabled.SetResourceObject(texture);
	buttonStyle.Pressed.SetResourceObject(texture);

	btn->SetStyle(buttonStyle);
}


void UItemList::SetItemSlot(int32 idx, const UItem* data, bool isHighlight)
{
	if (idx >= ItemSlots.Num())
		return;

	ItemSlots[idx]->SetInfo(data, isHighlight);
}

TArray<class UItemSlot*> UItemList::GetItemSlotList() const
{
	return ItemSlots;
}
