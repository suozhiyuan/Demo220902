// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Projectile.h"


#include "Kismet/GameplayStatics.h"
#include "Weapon/ShooterProjectile.h"

AShooterWeapon_Projectile::AShooterWeapon_Projectile()
{
}

void AShooterWeapon_Projectile::FireWeapon()
{
	Super::FireWeapon();

	const FVector ShooterDirection = GetAdjustAim();							// 眼睛方向
	const FVector Origin = GetMuzzleLocation();									// 枪口位置
	const FTransform SpawnTM(ShooterDirection.Rotation(), Origin);				// 通过参数的引用获取对象的位置和方向

	 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	  * BeginDeferredActorSpawnFromClass：为了让参数全部初始化完毕，延迟生成参与者的实例
	  * 参数1:上下文，要生成实例的对象
	  * 参数2:根据子弹类型去生成具体的对象，是一个TSubclassOf<AActor> ActorClass
	  * 参数3:生成的位置和朝向信息，是一个const FTransform& SpawnTransform
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));
	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());								// 保持统一的Instigator
		Projectile->SetOwner(this);												// 设置子弹拥有者，在计算子弹伤害时会从拥有者身上取属性
		Projectile->InitVelocity(ShooterDirection);								// 设置子弹速度，传进去的是方向角度，和速度有啥关系
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);				// 对应BeginDeferredActorSpawnFromClass，延迟结束
	}
}

// 返回武器的子弹配置
const void AShooterWeapon_Projectile::GetWeaponConfig(FProjectileWeaponData& Data)
{
	Data = ProjectileConfig;
}
