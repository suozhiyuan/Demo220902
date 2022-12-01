// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/ShooterProjectile.h"

void AShooterWeapon_Projectile::FireWeapon()
{
	Super::FireWeapon();

	FVector ShooterDirection;
	FVector Origin;
	ShooterDirection = GetAdjustAim();		// 眼睛方向
	Origin = GetMuzzleLocation();			// 枪口位置
	FTransform SpawnTM(ShooterDirection.Rotation(), Origin);	//位置方向

	// 动态延迟生成参与者的实例
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));

	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());
		Projectile->SetOwner(this);
	}

}
