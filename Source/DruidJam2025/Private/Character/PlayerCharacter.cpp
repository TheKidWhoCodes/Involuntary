// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "DruidJamController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/BlinkInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LevelScriptActor/DruidJamLevelScriptActor.h"
#include "UI/BlinkUserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanBlink)
	{
		BlinkMeter = FMath::Clamp(BlinkMeter - MeterDepletionRate * DeltaTime, 0.f, 100.f);
	}

	// TODO:
	// 1- heart beats
	// 2- fast depletion rate
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Input Actions
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Walk);
		EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Started, this, &APlayerCharacter::StartBlinking);
		EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopBlinking);
	}
}

void APlayerCharacter::ForceTeleport()
{
	// Teleport character below, then flip teleport direction
	SetActorLocation(GetActorLocation() + FVector(TeleportDistance, 0.f, 0.f));
	TeleportDistance *= -1;

	if(DimensionState == EDimensionState::EDS_Normal)
	{
		DimensionState = EDimensionState::EDS_Shadow;
	}
	else
	{
		DimensionState = EDimensionState::EDS_Normal;
	}
}

void APlayerCharacter::EyesOpening()
{
	BlinkMeter = 100.f;

	if(DimensionState == EDimensionState::EDS_Shadow) return;
	
	if(BlinkActors.IsEmpty()) return;
	
	for(AActor* Actor : BlinkActors)
	{
		IBlinkInterface::Execute_ReactToBlink(Actor);
	}
}

void APlayerCharacter::GoCrazy()
{
	// Start constant blinking animation
	if(BlinkWidget)
	{
		BlinkWidget->GoCrazy();
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(TEXT("Player"));
	PlayerController = Cast<ADruidJamController>(GetController());

	// Setup Subsystem
	if(PlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
		
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}

	// Add Blink widget
	if(BlinkWidgetClass)
	{
		BlinkWidget = Cast<UBlinkUserWidget>(CreateWidget(GetWorld(), BlinkWidgetClass));

		if(BlinkWidget)
		{
			BlinkWidget->AddToViewport();
		}
	}
	if(LossWidgetClass)
	{
		LossWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), LossWidgetClass));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	// Get controller rotation
	const FRotator ControllerRotation = FRotator(0.f, GetController()->GetControlRotation().Yaw, 0.f);

	// Controller's forward and right directions
	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(ControllerRotation);
	const FVector RightDirection = UKismetMathLibrary::GetRightVector(ControllerRotation);

	AddMovementInput(ForwardDirection,Input.X);
	AddMovementInput(RightDirection,Input.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	if(PlayerController)
	{
		PlayerController->AddYawInput(Input.X * 2.f);
		PlayerController->AddPitchInput(Input.Y);
	}
}

void APlayerCharacter::Run()
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void APlayerCharacter::Walk()
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::StartBlinking()
{
	if(!bCanBlink || !bIsBlinkButtonEnabled) return;

	// Disable blinking
	CharacterState = ECharacterState::ECS_Blinking;
	bCanBlink = false;

	// Start blinking animation
	if(BlinkWidget)
	{
		BlinkWidget->StartBlinkingAnimation();
	}

	if(bIsTeleportEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, this, &APlayerCharacter::Teleport, BlinkTime);
	}
}

void APlayerCharacter::StopBlinking()
{
	if(bCanBlink) return;

	CharacterState = ECharacterState::ECS_Neutral;

	if(BlinkWidget)
	{
		BlinkWidget->ReverseBlinkingAnimation();
	}
}

void APlayerCharacter::Teleport()
{
	if(CharacterState != ECharacterState::ECS_Blinking) return;

	// Teleport character below, then flip teleport direction
	SetActorLocation(GetActorLocation() + FVector(TeleportDistance, 0.f, 0.f));
	TeleportDistance *= -1;

	if(DimensionState == EDimensionState::EDS_Normal)
	{
		DimensionState = EDimensionState::EDS_Shadow;
	}
	else
	{
		DimensionState = EDimensionState::EDS_Normal;
	}

	if(TeleportSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), TeleportSound, 1.f, 2.f);
	}
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	if(BlinkWidget)
	{
		BlinkWidget->RemoveFromParent();
	}

	if(!bIsInFinalStage)
	{
		if(LossWidget)
		{
			LossWidget->AddToViewport();
		}
	}
	else
	{
		ADruidJamLevelScriptActor* LevelScript = Cast<ADruidJamLevelScriptActor>(GetLevel()->GetLevelScriptActor());
		LevelScript->BeginFinalSequence();
	}
}
