#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "TFModerneWarfare/Core/Enums/EArmorType.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFModerneWarfare/Core/Structs/FArmorData.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"

#include "TFC_GameStatics.generated.h"

UCLASS()
class TFMODERNEWARFARE_API UTFC_GameStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static const FArmorData* GetArmorData(EArmorType ArmorType);
	static const FPlayerClassData* GetClassData(EPlayerClass PlayerClass);

private:
	static void EnsureDataTablesLoaded();
	static UDataTable* ArmorDataTable;
	static UDataTable* ClassDataTable;
};
