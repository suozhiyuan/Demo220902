// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/ShooterProjectile.h"

void AShooterWeapon_Projectile::FireWeapon()
{
	Super::FireWeapon();

	FVector ShooterDirection;
	FVector Origin;
	ShooterDirection = GetAdjustAim();		// �۾�����
	Origin = GetMuzzleLocation();			// ǹ��λ��
	FTransform SpawnTM(ShooterDirection.Rotation(), Origin);	//λ�÷���

	// ��̬�ӳ����ɲ����ߵ�ʵ��
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));

	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());
		Projectile->SetOwner(this);
	}

}
