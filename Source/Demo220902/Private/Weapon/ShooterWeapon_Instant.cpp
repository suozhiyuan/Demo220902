// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon_Instant.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/ShooterCharacter.h"
#include "Effects/ShooterImpactEffect.h"



void AShooterWeapon_Instant::FireWeapon()
{
	Super::FireWeapon();

	// �����ӵ����䷽��
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandStream(RandomSeed);

	const float CurrentSpread = InstantConfig.WeaponSpread;							 // ��ɢ�Ƕ�
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5);		 // Բ׶���,   DegreesToRadians ���Ƕ�ת��Ϊ����
	const FVector AimDir = GetAdjustAim();											 // ��ȡ��׼�����۾�����

	FVector ShootDir =  WeaponRandStream.VRandCone(AimDir, ConeHalfAngle, ConeHalfAngle);	// ����һ�������λ���������ȷֲ�����ָ����Բ׶�ڡ�

	// �����˺��Ķ���
	const FVector StartTrace = GetMuzzleLocation();									// �ӵ��ķ���λ��
	const float WeaponRange = InstantConfig.WeaponRange;							// ����ɱ�˾���
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;					// ����׷��λ��

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// �˺�����
	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);

}

void AShooterWeapon_Instant::ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir,	int32 RandonSeed, float ReticleSpreed)
{
	ProcessInstantHit_Confirm(Impact, Orign, ShootDir, RandonSeed, ReticleSpreed);
}

void AShooterWeapon_Instant::ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed)
{
	// ����ֵ����
	DealDamage(Impact, ShootDir);

	// ������Ч
	const FVector EndTrace = Orign + ShootDir * InstantConfig.WeaponRange;			// ��Ч����λ��
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;		// ��Ч����λ��, ������Ŀ�귵��ImpactPointΪ��Ч����λ�ã�û�оͷ��� EndTrace Ϊ��ֹλ��
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
		const FVector Origin = GetMuzzleLocation();		// ǹ��λ��

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
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);			// ����ռ��е� ��ת �� λ��
		AShooterImpactEffect* EffectActor =  GetWorld()->SpawnActorDeferred<AShooterImpactEffect>(ImpactTemplate, SpawnTransform);
		if (EffectActor)
		{
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}
}
