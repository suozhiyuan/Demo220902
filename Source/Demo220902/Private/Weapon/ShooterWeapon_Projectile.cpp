// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Projectile.h"

#include "Kismet/GameplayStatics.h"

void AShooterWeapon_Projectile::FireWeapon()
{
	Super::FireWeapon();

	FVector ShooterDirection, Origin;
	ShooterDirection = GetAdjustAim();

	Origin = GetMuzzleLocation();

	//UGameplayStatics::BeginDeferredActorSpawnFromClass();
}
