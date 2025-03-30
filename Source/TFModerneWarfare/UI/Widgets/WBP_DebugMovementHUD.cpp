#include "WBP_DebugMovementHUD.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TFModerneWarfare/Characters/Components/Movement/TFC_MovementComponent.h"

void UWBP_DebugMovementHUD::SetPlayer(ATFC_PlayerBase* Player)
{
	PlayerRef = Player;

	if (PlayerRef)
	{
		MoveComp = Cast<UTFC_MovementComponent>(PlayerRef->GetComponentByClass(UTFC_MovementComponent::StaticClass()));
	}
}

void UWBP_DebugMovementHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!PlayerRef) return;

	// 🔹 Une seule fois : on affiche les données de classe
	if (!bDataBuilt)
	{
		const FPlayerClassData* ClassData = PlayerRef->GetClassData();
		if (ClassData)
		{
			BuildClassDataUI(ClassData);
			bDataBuilt = true;
		}
	}

	// 🔄 Update live du mouvement uniquement
	if (Text_MovementState && MoveComp)
	{
		EMovementState CurrentState = MoveComp->GetMovementState();
		if (LastKnownMovementState != CurrentState)
		{
			LastKnownMovementState = CurrentState;

			const FText StateText = UEnum::GetDisplayValueAsText(CurrentState);
			Text_MovementState->SetText(FText::FromString(FString::Printf(TEXT("Movement State: %s"), *StateText.ToString())));
		}
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

		FText Line = FText::FromString(FString::Printf(TEXT("%s : %s"), *PropertyName, *ValueStr));

		UTextBlock* LineWidget = NewObject<UTextBlock>(this);
		LineWidget->SetText(Line);
		LineWidget->SetJustification(ETextJustify::Left);
		FSlateFontInfo FontInfo = LineWidget->GetFont();
		FontInfo.Size = 10;
		LineWidget->SetFont(FontInfo);


		Scroll_ClassData->AddChild(LineWidget);
	}

	// Ajout du champ dynamique pour MovementState
	Text_MovementState = NewObject<UTextBlock>(this);
	Text_MovementState->SetJustification(ETextJustify::Left);
	FSlateFontInfo FontInfo = Text_MovementState->GetFont();
	FontInfo.Size = 10;
	Text_MovementState->SetFont(FontInfo);

	Scroll_ClassData->AddChild(Text_MovementState);
}