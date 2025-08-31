// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/DruidJamGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UDruidJamGameInstance::PlayerDied()
{
	if(CheckpointTransform.GetTranslation() == FVector::ZeroVector)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UDruidJamGameInstance::SpawnTimerEnded, 0.2f);
	}
}

void UDruidJamGameInstance::SpawnTimerEnded()
{
	if(ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		PlayerCharacter->SetActorTransform(CheckpointTransform);
	}
}
