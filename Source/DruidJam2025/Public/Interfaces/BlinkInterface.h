// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlinkInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlinkInterface : public UInterface
{
	GENERATED_BODY()
};

class DRUIDJAM2025_API IBlinkInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReactToBlink();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reset();
	
};
