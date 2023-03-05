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

	const FVector ShooterDirection = GetAdjustAim();							// �۾�����
	const FVector Origin = GetMuzzleLocation();									// ǹ��λ��
	const FTransform SpawnTM(ShooterDirection.Rotation(), Origin);				// ͨ�����������û�ȡ�����λ�úͷ���

	 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	  * BeginDeferredActorSpawnFromClass��Ϊ���ò���ȫ����ʼ����ϣ��ӳ����ɲ����ߵ�ʵ��
	  * ����1:�����ģ�Ҫ����ʵ���Ķ���
	  * ����2:�����ӵ�����ȥ���ɾ���Ķ�����һ��TSubclassOf<AActor> ActorClass
	  * ����3:���ɵ�λ�úͳ�����Ϣ����һ��const FTransform& SpawnTransform
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));
	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());								// ����ͳһ��Instigator
		Projectile->SetOwner(this);												// �����ӵ�ӵ���ߣ��ڼ����ӵ��˺�ʱ���ӵ��������ȡ����
		Projectile->InitVelocity(ShooterDirection);								// �����ӵ��ٶȣ�����ȥ���Ƿ���Ƕȣ����ٶ���ɶ��ϵ
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);				// ��ӦBeginDeferredActorSpawnFromClass���ӳٽ���
	}
}

// �����������ӵ�����
const void AShooterWeapon_Projectile::GetWeaponConfig(FProjectileWeaponData& Data)
{
	Data = ProjectileConfig;
}
