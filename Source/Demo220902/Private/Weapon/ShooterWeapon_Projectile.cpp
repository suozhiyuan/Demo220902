// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/ShooterProjectile.h"

void AShooterWeapon_Projectile::FireWeapon()
{
	Super::FireWeapon();

	const FVector ShooterDirection = GetAdjustAim();							// 眼睛方向
	const FVector Origin = GetMuzzleLocation();									// 枪口位置
	const FTransform SpawnTM(ShooterDirection.Rotation(), Origin);	// 位置方向

	 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	  * 为了让参数全部初始化完毕，延迟生成参与者的实例
	  * 参数1:上下文，要生成实例的对象
	  * 参数2:根据子弹类型去生成具体的对象，是一个TSubclassOf<AActor> ActorClass
	  * 参数3:生成的位置和朝向信息，是一个const FTransform& SpawnTransform
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));
	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());		// 保持统一的Instigator
		Projectile->SetOwner(this);						// 设置子弹拥有者
		Projectile->InitVelocity(ShooterDirection);		// 设置子弹速度
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);		// 对应BeginDeferredActorSpawnFromClass，延迟结束
	}

}
