// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BlinkInterface.h"
#include "Enemy.generated.h"

class UCapsuleComponent;

UCLASS()
class DRUIDJAM2025_API AEnemy : public AActor, public IBlinkInterface
{
	GENERATED_BODY()
	
public:
	
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void ReactToBlink_Implementation() override;
	virtual void Reset_Implementation() override;

protected:
	
	virtual void BeginPlay() override;

private:

	void Step();

	FTransform OriginalTransform;

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	float StepSize = 50.f;

	UPROPERTY()
	ACharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TArray<UAnimSequence*> InitialPoses;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TArray<UAnimSequence*> MovementPoses;

	/* Components */

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;

public:

	/* Setters */
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStepSize(const float Value) { StepSize = Value; }
	
};
