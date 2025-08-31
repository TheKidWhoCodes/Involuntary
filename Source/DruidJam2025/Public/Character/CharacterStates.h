#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Neutral,
	ECS_Blinking
};

UENUM(BlueprintType)
enum class EDimensionState : uint8
{
	EDS_Normal,
	EDS_Shadow
};