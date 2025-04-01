// WBP_DebugMovementHUD.cpp (corrigé)

#include "WBP_DebugMovementHUD.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Characters/Components/Movement/TFC_MovementComponent.h"
#include "TFModerneWarfare/Characters/Components/Health/TFC_HealthComponent.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"

void UWBP_DebugMovementHUD::SetPlayer(ATFC_PlayerBase* Player)
{
	PlayerRef = Player;

	if (PlayerRef)
	{
		MoveComp = PlayerRef->FindComponentByClass<UTFC_MovementComponent>();
		CachedHealthComponent = PlayerRef->FindComponentByClass<UTFC_HealthComponent>();
		CachedArmorComponent = PlayerRef->FindComponentByClass<UTFC_ArmorComponent>();
	}
}

void UWBP_DebugMovementHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!PlayerRef) return;

	if (!bDataBuilt)
	{
		if (const FPlayerClassData* ClassData = PlayerRef->GetClassData())
		{
			BuildClassDataUI(ClassData);
			bDataBuilt = true;
		}
	}

	ClearDynamicDebugEntries();

	if (Text_MovementState && MoveComp)
	{
		const EMovementState CurrentState = MoveComp->GetMovementState();
		const FText StateText = UEnum::GetDisplayValueAsText(CurrentState);
		Text_MovementState->SetText(FText::FromString(FString::Printf(TEXT("Movement State: %s"), *StateText.ToString())));
	}

	if (CachedHealthComponent)
	{
		AddDebugEntry(TEXT("Health"), FString::Printf(TEXT("%.0f / %.0f"), CachedHealthComponent->GetCurrentHealth(), CachedHealthComponent->GetMaxHealth()));
	}

	if (CachedArmorComponent)
	{
		AddDebugEntry(TEXT("Armor Absorption"), FString::Printf(TEXT("%.0f%%"), CachedArmorComponent->GetDamageAbsorption() * 100.0f));
		AddDebugEntry(TEXT("Armor Type"), UEnum::GetDisplayValueAsText(CachedArmorComponent->GetArmorType()).ToString());
	}
}

void UWBP_DebugMovementHUD::BuildClassDataUI(const FPlayerClassData* ClassData)
{
	if (!Scroll_ClassData) return;

	for (TFieldIterator<const FProperty> PropIt(FPlayerClassData::StaticStruct()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		const FString PropertyName = Property->GetName();
		FString ValueStr;
		Property->ExportText_Direct(ValueStr, Property->ContainerPtrToValuePtr<void>(ClassData), nullptr, nullptr, PPF_None);

		UTextBlock* LineWidget = NewObject<UTextBlock>(this);
		LineWidget->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), *PropertyName, *ValueStr)));
		LineWidget->SetJustification(ETextJustify::Left);
		FSlateFontInfo FontInfo = LineWidget->GetFont();
		FontInfo.Size = 10;
		LineWidget->SetFont(FontInfo);
		Scroll_ClassData->AddChild(LineWidget);
	}

	Text_MovementState = NewObject<UTextBlock>(this);
	Text_MovementState->SetJustification(ETextJustify::Left);
	FSlateFontInfo FontInfo = Text_MovementState->GetFont();
	FontInfo.Size = 10;
	Text_MovementState->SetFont(FontInfo);
	Scroll_ClassData->AddChild(Text_MovementState);
}

void UWBP_DebugMovementHUD::AddDebugEntry(const FString& Label, const FString& Value)
{
	if (!Scroll_LiveDebug) return;
	const FString FullLine = FString::Printf(TEXT("%s : %s"), *Label, *Value);

	UTextBlock** ExistingText = DebugTextEntries.Find(Label);
	if (ExistingText && *ExistingText)
	{
		(*ExistingText)->SetText(FText::FromString(FullLine));
		return;
	}

	UTextBlock* LineWidget = NewObject<UTextBlock>(this);
	LineWidget->SetText(FText::FromString(FullLine));
	LineWidget->SetJustification(ETextJustify::Left);
	FSlateFontInfo FontInfo = LineWidget->GetFont();
	FontInfo.Size = 10;
	LineWidget->SetFont(FontInfo);
	Scroll_LiveDebug->AddChild(LineWidget);
	DebugTextEntries.Add(Label, LineWidget);
}

void UWBP_DebugMovementHUD::ClearDynamicDebugEntries()
{
	for (auto& Elem : DebugTextEntries)
	{
		if (Elem.Value)
		{
			Elem.Value->RemoveFromParent();
		}
	}
	DebugTextEntries.Empty();
}