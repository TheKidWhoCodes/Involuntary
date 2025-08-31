// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("SkeletalMesh")));
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(FName(TEXT("CapsuleComponent")));

	SetRootComponent(Capsule);
	Mesh->SetupAttachment(GetRootComponent());

	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this,0);

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCapsuleOverlap);

	// Set random rotation and play a random initial pose
	SetActorRotation(FRotator(0.f, FMath::Rand(), 0.f));
	const int32 Selection = FMath::RandRange(0, InitialPoses.Num() - 1);
	Mesh->PlayAnimation(InitialPoses[Selection], true);
	
	OriginalTransform = GetActorTransform();
}

void AEnemy::Step()
{
	if(!PlayerCharacter) return;
	
	// Take one step closer to the player and change pose
	SetActorRotation(FRotator(GetActorRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation()).Yaw, GetActorRotation().Roll));
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * StepSize);
	const int32 Selection = FMath::RandRange(0, MovementPoses.Num() - 1);
	Mesh->PlayAnimation(MovementPoses[Selection], true);
}

void AEnemy::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("Player")))
	{
		OtherActor->Destroy();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::ReactToBlink_Implementation()
{
	IBlinkInterface::ReactToBlink_Implementation();
	Step();
}

void AEnemy::Reset_Implementation()
{
	// Resets actor transform and plays random initial pose
	IBlinkInterface::Reset_Implementation();
	SetActorTransform(OriginalTransform);
	const int32 Selection = FMath::RandRange(0, InitialPoses.Num() - 1);
	Mesh->PlayAnimation(InitialPoses[Selection], true);
}
