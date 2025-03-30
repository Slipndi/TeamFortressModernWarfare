#include "TFC_GameStatics.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UTFC_GameStatics::ArmorDataTable = nullptr;
UDataTable* UTFC_GameStatics::ClassDataTable = nullptr;

void UTFC_GameStatics::EnsureDataTablesLoaded()
{
	if (!ArmorDataTable)
	{
		ArmorDataTable = Cast<UDataTable>(StaticLoadObject(
			UDataTable::StaticClass(), nullptr,
			TEXT("/Game/Data/DT_ArmorTypes.DT_ArmorTypes")
		));

		if (!ArmorDataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Impossible de charger DT_ArmorTypes."));
		}
	}

	if (!ClassDataTable)
	{
		ClassDataTable = Cast<UDataTable>(StaticLoadObject(
			UDataTable::StaticClass(), nullptr,
			TEXT("/Game/Data/DT_PlayerClasses.DT_PlayerClasses")
		));

		if (!ClassDataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Impossible de charger DT_PlayerClasses."));
		}
	}
}

const FArmorData* UTFC_GameStatics::GetArmorData(EArmorType ArmorType)
{
	EnsureDataTablesLoaded();
	if (!ArmorDataTable) return nullptr;

	FName RowName = *UEnum::GetValueAsString(ArmorType).Replace(TEXT("EArmorType::"), TEXT(""));
	constexpr const TCHAR* Context = TEXT("UTFC_GameStatics::GetArmorData");

	const FArmorData* Row = ArmorDataTable->FindRow<FArmorData>(RowName, Context);
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Row %s introuvable dans ArmorDataTable"), *RowName.ToString());
	}
	return Row;
}

const FPlayerClassData* UTFC_GameStatics::GetClassData(EPlayerClass PlayerClass)
{
	EnsureDataTablesLoaded();
	if (!ClassDataTable) return nullptr;

	FName RowName = *UEnum::GetValueAsString(PlayerClass).Replace(TEXT("EPlayerClass::"), TEXT(""));
	constexpr const TCHAR* Context = TEXT("UTFC_GameStatics::GetClassData");

	const FPlayerClassData* Row = ClassDataTable->FindRow<FPlayerClassData>(RowName, Context);
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Row %s introuvable dans ClassDataTable"), *RowName.ToString());
	}
	return Row;
}
