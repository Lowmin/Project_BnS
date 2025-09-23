// Fill out your copyright notice in the Description page of Project Settings.


#include "LootPopupWidget.h"
#include "LootItemSlotWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Button.h"
#include "../MyPlayer.h"
#include "LootBox.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void ULootPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_LootAll)
	{
		Button_LootAll->OnClicked.AddDynamic(this, &ULootPopupWidget::OnLootAllButtonClicked);
	}
}

void ULootPopupWidget::PopulateItems(const TArray<FDropItemInfo>& Items, ALootBox* OwnerBox)
{
	OwnerLootBox = OwnerBox;
	LootableItems = Items;

	if (!ItemScrollBox || !LootItemSlotClass) return;

	ItemScrollBox->ClearChildren();

	for (const FDropItemInfo& ItemInfo : LootableItems)
	{
		UDataTable* ItemTable = ItemInfo.ItemDataTable.LoadSynchronous();
		if (ItemTable)
		{
			const FItemData* ItemData = ItemTable->FindRow<FItemData>(ItemInfo.ItemID, "");
			if (ItemData)
			{
				ULootItemSlotWidget* SlotWidget = CreateWidget<ULootItemSlotWidget>(this, LootItemSlotClass);
				if (SlotWidget)
				{
					SlotWidget->SetItemInfo(ItemData, ItemInfo.Quantity);

					UPanelSlot* PanelSlot = ItemScrollBox->AddChild(SlotWidget);

					if (UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot))
					{
						ScrollBoxSlot->SetHorizontalAlignment(HAlign_Fill);
					}
				}
			}
		}
	}
}

void ULootPopupWidget::OnLootAllButtonClicked()
{
	AMyPlayer* Player = Cast<AMyPlayer>(GetOwningPlayerPawn());

	if (Player && Player->GetInventoryComponent())
	{
		for (const FDropItemInfo& ItemInfo : LootableItems)
		{
			int32 ItemIdAsInt = FCString::Atoi(*ItemInfo.ItemID.ToString());
			Player->GetInventoryComponent()->AddItem(ItemIdAsInt, ItemInfo.Quantity);
		}
	}

	RemoveFromParent();
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	if (OwnerLootBox.IsValid())
	{
		OwnerLootBox->Destroy();
	}
}

