// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DruidJamLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class DRUIDJAM2025_API ADruidJamLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BeginFinalSequence();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetViewToFinaleCamera();
	
};
