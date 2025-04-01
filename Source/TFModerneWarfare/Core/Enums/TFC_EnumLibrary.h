#pragma once

#include "CoreMinimal.h"
#include "EArmorType.h"
#include "EMovementState.h"
#include "EPlayerClass.h"

/**
 * Librairie statique utilitaire pour les enums du projet TFModerneWarfare.
 */
class TFC_EnumLibrary
{
public:

	// =========================
	// 🧱 ToString()
	// =========================

	static FString ToString(EArmorType Type)
	{
		return UEnum::GetDisplayValueAsText(Type).ToString();
	}

	static FString ToString(EMovementState State)
	{
		return UEnum::GetDisplayValueAsText(State).ToString();
	}

	static FString ToString(EPlayerClass Class)
	{
		return UEnum::GetDisplayValueAsText(Class).ToString();
	}

	// =========================
	// ✅ IsValid()
	// =========================

	static bool IsValid(EArmorType Type)
	{
		return Type != EArmorType::None && Type != EArmorType::_MAX;
	}

	static bool IsValid(EMovementState State)
	{
		return State != EMovementState::None && State != EMovementState::_MAX;
	}

	static bool IsValid(EPlayerClass Class)
	{
		return Class != EPlayerClass::None && Class != EPlayerClass::_MAX;
	}
};
