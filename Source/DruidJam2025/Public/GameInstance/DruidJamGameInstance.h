// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DruidJamGameInstance.generated.h"

UCLASS()
class DRUIDJAM2025_API UDruidJamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void PlayerDied();

private:

	void SpawnTimerEnded();
	
	FTransform CheckpointTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector);

	FTimerHandle SpawnTimerHandle;

public:

	/* Setters */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCheckpointTransform(const FTransform Transform) { CheckpointTransform = Transform; }

	/* Getters */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FTransform GetCheckpointTransform() const { return CheckpointTransform; }
	
};
