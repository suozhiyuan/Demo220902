// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Instant.generated.h"

// ��ʱ����������
USTRUCT()
struct FInstantWeaponDate
{
	GENERATED_BODY()
		
	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float WeaponSpread;						// ��ɢ�Ƕ�

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float WeaponRange;						// �������

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	TSubclassOf<UDamageType> DamageType;	// �˺�����

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	int32 HitDamage;						// �˺�
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
	 * @param Impact			��ײ������Ϣ
	 * @param Orign				��ʼλ��
	 * @param ShootDir			��ײ����λ��
	 * @param RandonSeed		�������
	 * @param ReticleSpreed		��ɢ�Ƕȣ�Բ׶���ڷ���
	 */
	void ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed);

	/**
	 * @brief
	 * @param Impact			��ײ������Ϣ
	 * @param Orign				��ʼλ��
	 * @param ShootDir			��ײ����λ��
	 * @param RandonSeed		�������
	 * @param ReticleSpreed		��ɢ�Ƕȣ�Բ׶���ڷ���
	 */
	void ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandonSeed, float ReticleSpreed);

	/** �˺�����
	 * @brief 
	 * @param Impact		���˺���λ
	 * @param ShootDir		����
	 */
	void DealDamage(const FHitResult& Impact, const FVector ShootDir);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FInstantWeaponDate InstantConfig;

};
