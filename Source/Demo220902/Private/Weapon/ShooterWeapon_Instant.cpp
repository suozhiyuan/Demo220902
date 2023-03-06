// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Instant.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/ShooterCharacter.h"
#include "Effects/ShooterImpactEffect.h"



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

	// 生成特效
	const FVector EndTrace = Orign + ShootDir * InstantConfig.WeaponRange;			// 特效结束位置
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;		// 特效结束位置, 有命中目标返回ImpactPoint为特效结束位置，没有就返回 EndTrace 为终止位置
	SpawnTrailEffects(EndPoint);
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

void AShooterWeapon_Instant::SpawnTrailEffects(const FVector& EndPoint)
{
	if (TraiFX)
	{
		const FVector Origin = GetMuzzleLocation();		// 枪口位置

		UParticleSystemComponent* TrailPSC =  UGameplayStatics::SpawnEmitterAtLocation(this, TraiFX, Origin);
		if (TrailPSC)
		{
			TrailPSC->SetVectorParameter(TrailTargetParma, EndPoint);
		}
	}
}

void AShooterWeapon_Instant::SpawnImpactEffects(const FHitResult& Impact)
{
	if (Impact.bBlockingHit && ImpactTemplate)
	{
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);			// 世界空间中的 旋转 与 位置
		AShooterImpactEffect* EffectActor =  GetWorld()->SpawnActorDeferred<AShooterImpactEffect>(ImpactTemplate, SpawnTransform);
		if (EffectActor)
		{
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}
}
