// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Instant.h"

#include "Player/ShooterCharacter.h"

void AShooterWeapon_Instant::FireWeapon()
{
	Super::FireWeapon();

	// 计算子弹发射方向
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandStream(RandomSeed);

	const float CurrentSpread = InstantConfig.WeaponSpread;							 // 扩散角度
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5);		 // 圆锥半角,   DegreesToRadians 将角度转换为弧度
	const FVector AimDir = GetAdjustAim();											 // 获取瞄准方向（眼睛方向）

	FVector ShootDir =  WeaponRandStream.VRandCone(AimDir, ConeHalfAngle, ConeHalfAngle);	// 返回一个随机单位向量，均匀分布，在指定的圆锥内。

	// 计算伤害的对象
	const FVector StartTrace = GetMuzzleLocation();									// 子弹的发射位置
	const float WeaponRange = InstantConfig.WeaponRange;							// 武器杀伤距离
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;					// 结束追踪位置

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// 伤害处理
	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);

}

void AShooterWeapon_Instant::ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir,	int32 RandonSeed, float ReticleSpreed)
{
	ProcessInstantHit_Confirm(Impact, Orign, ShootDir, RandonSeed, ReticleSpreed);
}

void AShooterWeapon_Instant::ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed)
{
	// 生命值处理
	DealDamage(Impact, ShootDir);
}

void AShooterWeapon_Instant::DealDamage(const FHitResult& Impact, const FVector ShootDir)
{
	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = InstantConfig.DamageType;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = InstantConfig.HitDamage;

	if (Impact.GetActor() != nullptr)
	{
		Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, PawnOwner->Controller, this);
	}
}
