// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"


class USoundCue;
class AShooterCharacter;

namespace EWeaponState
{
	enum Type
	{
		Idle,			// ����
		Firing,			// ����
		Reloading,		// ����װ��
		Equiping		// װ��
	};
}

UCLASS()
class DEMO220902_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterWeapon();

	// ���� Mesh �� Pawn
	void AttachMeshToPawn();

	// ��ȡ�۾�����
	FVector GetAdjustAim();

	// ����
	void StartFire();

	// ���𣬴��������ӵ��Լ��ӵ�����λ�úͷ���
	virtual void FireWeapon();

	//ģ�⿪��
	void SimulateWeaponFire();

	//��Ч���
	UAudioComponent* PlayWeaponSound(USoundBase* Sound);

	// ��ȡǹ��λ��
	FVector GetMuzzleLocation();

	// ��ȡ��ǰ�ӵ�����
	int GetAmmoCount();

	// ��ȡ����ӵ�����
	int GetAmmoCountMax();

	// ȷ������״̬
	void WeaponState();

	// �Ƿ���Կ���
	bool CanFire() const;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ����������ǰ��Pawn
	void SetPawnOwner(AShooterCharacter* pawnOwner);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	// SkeletalMeshComponent���ڴ�������SkeletalMesh�ʲ���ʵ������Ҫ����һ��������ʱ�õ�USkeletalMeshComponent��
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh1P;

	// �ӵ���ǹ�ڵ�λ��
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName MuzzleAttachPoint;

	//������Ч
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int AmmoCountMax;

	int AmmoCount;

private:
	AShooterCharacter* PawnOwner;

	// �Ƿ�װ������
	bool bIsEquip;

	// �Ƿ�װ�ӵ�
	bool bIsReload;

	// �Ƿ񿪻�
	bool bIsFire;

	// �Ƿ�����
	bool bIsExchangeWeapon;

};
