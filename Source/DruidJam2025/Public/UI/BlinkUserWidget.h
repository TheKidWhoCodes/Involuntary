// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlinkUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRUIDJAM2025_API UBlinkUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartBlinkingAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void ReverseBlinkingAnimation();

	/**
	 * Makes character constantly blink
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void GoCrazy();
};
