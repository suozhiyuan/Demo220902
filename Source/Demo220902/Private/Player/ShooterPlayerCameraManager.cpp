// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterPlayerCameraManager.h"
#include "Player/ShooterCharacter.h"

AShooterPlayerCameraManager::AShooterPlayerCameraManager()
{
}

void AShooterPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	AShooterCharacter* MyPawn = Cast<AShooterCharacter>(PCOwner ? PCOwner->GetPawn() : nullptr);
	if (MyPawn)
	{
		MyPawn->OnCanmeraUpdate(GetCameraLocation(), GetCameraRotation());
	}
}
