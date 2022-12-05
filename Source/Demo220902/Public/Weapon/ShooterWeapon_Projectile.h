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

	// �ӵ��࣬��Ҫ���ӵ�����ͼ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AShooterProjectile> ProjectileClass;

	// ��ը�����˺�
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 ExplosionDmg;

	// ��ը�뾶
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 ExplosionRad;

	// �˺�����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UDamageType> DmgType;
};


/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterWeapon_Projectile : public AShooterWeapon		// �̳л����� AShooterWeapon���������ټ̳� AShooterWeapon_Projectile ����ͼ��
{
	GENERATED_BODY()
	AShooterWeapon_Projectile();

public:
	// ��ǹ
	virtual void FireWeapon() override;

	// �����������ӵ�����
	const void GetWeaponConfig(FProjectileWeaponData &Data);

protected:
	// �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	FProjectileWeaponData ProjectileConfig;

};
