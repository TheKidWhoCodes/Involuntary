// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CharacterStates.h"
#include "PlayerCharacter.generated.h"

class ADruidJamController;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UBlinkUserWidget;
class IBlinkInterface;

// Declares delegate
DECLARE_DELEGATE(FTestDelegate);

UCLASS()
class DRUIDJAM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ForceTeleport();
	
	UFUNCTION(BlueprintCallable)
	void EyesOpening();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void EmptyBlinkActors() { BlinkActors.Empty(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DisableTeleport() { bIsTeleportEnabled = false; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void EnableTeleport() { bIsTeleportEnabled = true; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddBlinkActors(AActor* Actor) { BlinkActors.Add(Actor); }

	// Make character constantly blink :)
	UFUNCTION(BlueprintCallable)
	void GoCrazy();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DisableBlinkInput() { bIsBlinkButtonEnabled = false; }

protected:

	virtual void BeginPlay() override;

private:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run();
	void Walk();
	void StartBlinking();
	void StopBlinking();
	virtual void Destroyed() override;
	void Teleport();

	UPROPERTY(EditAnywhere, Category = Movement)
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WalkSpeed = 400.f;

	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Neutral;

	UPROPERTY(VisibleAnywhere)
	EDimensionState DimensionState = EDimensionState::EDS_Normal;

	bool bIsInFinalStage = false;

	/* Blink */

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	bool bCanBlink = true;

	UPROPERTY(EditDefaultsOnly, Category = Blink)
	float MeterDepletionRate = 5.f;

	UPROPERTY(VisibleAnywhere, Category = Blink)
	TArray<AActor*> BlinkActors;

	UPROPERTY(EditDefaultsOnly, Category = Blink)
	float TeleportDistance = 0;

	UPROPERTY(EditDefaultsOnly, Category = Blink)
	float BlinkTime = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = Blink)
	USoundBase* TeleportSound;

	float BlinkMeter = 100.f;

	FTimerHandle BlinkTimerHandle;
	
	bool bIsTeleportEnabled = false;
	bool bIsBlinkButtonEnabled = true;

	/* UI */

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UBlinkUserWidget> BlinkWidgetClass;

	UPROPERTY()
	UBlinkUserWidget* BlinkWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> LossWidgetClass;

	UPROPERTY()
	UUserWidget* LossWidget;

	/* Input */

	UPROPERTY()
	ADruidJamController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* RunAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* BlinkAction;

public:

	/* Setters */
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCanBlink() { bCanBlink = true; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBlinkActors(const TArray<AActor*> Actors) { BlinkActors = Actors; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsInFinalStage() { bIsInFinalStage = true; }

	/* Getters */
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBlinkMeter() const { return BlinkMeter; }

};
