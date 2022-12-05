// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeapon.h"

#include "ShooterWeapon_Projectile.generated.h"


class AShooterProjectile;

USTRUCT()
struct FProjectileWeaponData
{
	//GENERATED_BODY()
	GENERATED_USTRUCT_BODY()

	// 子弹类，主要给子弹的蓝图类用
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AShooterProjectile> ProjectileClass;

	// 爆炸中心伤害
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 ExplosionDmg;

	// 爆炸半径
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 ExplosionRad;

	// 伤害类型
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UDamageType> DmgType;
};


/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterWeapon_Projectile : public AShooterWeapon		// 继承基础的 AShooterWeapon，引擎中再继承 AShooterWeapon_Projectile 做蓝图类
{
	GENERATED_BODY()
	AShooterWeapon_Projectile();

public:
	// 火枪
	virtual void FireWeapon() override;

	// 返回武器的子弹配置
	const void GetWeaponConfig(FProjectileWeaponData &Data);

protected:
	// 子弹
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	FProjectileWeaponData ProjectileConfig;

};
