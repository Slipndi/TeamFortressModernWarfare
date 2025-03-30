#include "TFC_GameStatics.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UTFC_GameStatics::ArmorDataTable = nullptr;
UDataTable* UTFC_GameStatics::ClassDataTable = nullptr;

void UTFC_GameStatics::EnsureDataTablesLoaded()
{
	if (!ArmorDataTable)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> ArmorDT(TEXT("DataTable'/Game/Data/DT_ArmorTypes.DT_ArmorTypes'"));
		if (ArmorDT.Succeeded())
		{
			ArmorDataTable = ArmorDT.Object;
			UE_LOG(LogTemp, Warning, TEXT("✅ Armor DataTable loaded"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Failed to load Armor DataTable"));
		}
	}

	if (!ClassDataTable)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> ClassDT(TEXT("DataTable'/Game/Data/DT_PlayerClasses.DT_PlayerClasses'"));
		if (ClassDT.Succeeded())
		{
			ClassDataTable = ClassDT.Object;
			UE_LOG(LogTemp, Warning, TEXT("✅ Class DataTable loaded"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Failed to load Class DataTable"));
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
