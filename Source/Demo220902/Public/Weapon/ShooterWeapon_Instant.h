// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Instant.generated.h"

class AShooterImpactEffect;


// 即时武器的配置
USTRUCT()
struct FInstantWeaponDate
{
	GENERATED_BODY()
		
	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float WeaponSpread;						// 扩散角度

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float WeaponRange;						// 射击距离

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	TSubclassOf<UDamageType> DamageType;	// 伤害类型

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	int32 HitDamage;						// 伤害
};

/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterWeapon_Instant : public AShooterWeapon
{
	GENERATED_BODY()

public:
	virtual void FireWeapon() override;

	/**
	 * @brief	
	 * @param Impact			碰撞返回信息
	 * @param Orign				起始位置
	 * @param ShootDir			碰撞结束位置
	 * @param RandonSeed		随机种子
	 * @param ReticleSpreed		扩散角度，圆锥开口方向
	 */
	void ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed);

	/**
	 * @brief
	 * @param Impact			碰撞返回信息
	 * @param Orign				起始位置
	 * @param ShootDir			碰撞结束位置
	 * @param RandonSeed		随机种子
	 * @param ReticleSpreed		扩散角度，圆锥开口方向
	 */
	void ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed);

	/** 伤害计算
	 * @brief 
	 * @param Impact		被伤害单位
	 * @param ShootDir		方向
	 */
	void DealDamage(const FHitResult& Impact, const FVector ShootDir);

	// 生成轨迹特效
	void SpawnTrailEffects(const FVector& EndPoint);

	// 生成碰撞特效
	void SpawnImpactEffects(const FHitResult& Impact);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FInstantWeaponDate InstantConfig;

	// 粒子发射器
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* TraiFX;

	// 用来配置特效的追踪名称，与特效中的 ParameterName 匹配
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName TrailTargetParma;

	// 
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<AShooterImpactEffect> ImpactTemplate;
};
