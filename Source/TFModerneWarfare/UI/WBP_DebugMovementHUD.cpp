#include "WBP_DebugMovementHUD.h"
#include "Components/TextBlock.h"
#include "TFModerneWarfare/Characters/TFC_PlayerBase.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"

void UWBP_DebugMovementHUD::SetPlayer(ATFC_PlayerBase* Player)
{
	PlayerRef = Player;
}

void UWBP_DebugMovementHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!PlayerRef) return;

	const FPlayerClassData* ClassData = PlayerRef->GetClassData();
	if (!ClassData) return;

	Text_ClassName->SetText(FText::FromString(UEnum::GetDisplayValueAsText(PlayerRef->PlayerClassType).ToString()));
	Text_FinalWalkSpeed->SetText(FText::AsNumber(PlayerRef->FinalWalkSpeed));
	Text_FinalSprintSpeed->SetText(FText::AsNumber(PlayerRef->FinalSprintSpeed));
	Text_SlideImpulse->SetText(FText::AsNumber(ClassData->SlideImpulse));
	Text_MovementState->SetText(FText::FromString(UEnum::GetDisplayValueAsText(PlayerRef->GetMovementState()).ToString()));
}
