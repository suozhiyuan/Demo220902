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
		Equiping		// ��������
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 AmmoPerClip;					// ��ϻ����

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 InitalClip;					// ��ϻ����

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float TimeBetweenShots;				// �������ʱ��

	FWeaponData()
	{
		AmmoPerClip = 30;
		InitalClip = 4;
		TimeBetweenShots = 0.1f;
	}
};

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

	// ͣ��
	void StopFire();

	// ���𣬴��������ӵ��Լ��ӵ�����λ�úͷ���
	virtual void FireWeapon();

	// ģ�⿪��
	void SimulateWeaponFire();

	// ֹͣģ�⿪��
	void StopSimulateWeaponFire();

	// ��Ч���
	UAudioComponent* PlayWeaponSound(USoundBase* Sound);

	// ��ȡǹ��λ��
	FVector GetMuzzleLocation();

	//// ��ȡ��ǰ�ӵ�����
	//int GetAmmoCount();

	//// ��ȡ����ӵ�����
	//int GetAmmoCountMax();

	// ȷ������״̬
	void WeaponState();

	// �Ƿ���Կ���
	bool CanFire() const;

	// �Ƿ���Ի������ж��ӵ���������ϻ��ǰ�Ƿ�����ҩ���Լ����õ�ҩ����0
	bool CanReload() const;

	// ��������״̬
	void SetWeaponState(EWeaponState::Type NewState);

	// ��������״̬������Ӧ�¼�
	void HandleCurrentState();

	// ����ʼ����״̬
	void HandleStartFireState();

	// �����������״̬
	void HandleEndFireState();

	// ����װ��״̬
	void HandleStartReloadState();

	// �������װ��״̬
	void HandleEndReloadState();

	// ����װ��״̬
	void HandleStartEquipState();

	// ���������װ��״̬
	void HandleEndEquipState();

	// ��Ӧ����װ��
	void OnEquip(const AShooterWeapon* _LastWeapon);

	// ����������ǰ��Pawn
	void SetPawnOwner(AShooterCharacter* pawnOwner);

	// װ��
	void StartReload();

	// ֹͣװ��
	void StopReload();

	// ���������ӵ�
	void ReloadWeapon();

	// ������״̬
	void HandleFiring();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ȡ��ǰ�ӵ�����
	int32 GetCurrentAmmo() const;

	// ��ȡ��ϻ��ǰ�ӵ�����
	int32 GetCurrentAmmoClip() const;

	// ��ȡ��ϻ����
	int32 GetAmmoPerClip() const;

	// ��ϻ�ӵ����� ���ӵ���������
	void UseAmmo();

	// �����˺�����
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ��Ӧװ�����
	void OnEquipFinish();

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

	// ��ǹ��Ч
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* EquipSound;

	// ������Ч
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ReloadSound;

	//// ����ӵ�����
	//UPROPERTY(EditDefaultsOnly, Category = Ammo)
	//int AmmoCountMax;

	//// ��ǰ�ӵ�����
	//int AmmoCount;

	// ������������
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	// ��ǰ�����ӵ�����δʹ�õ� + ��ϻ��ʣ�µģ�
	UPROPERTY(Transient)
	int32 CurrentAmmo;

	// ��ǰ��ϻ�����ӵ�����
	UPROPERTY(Transient)
	int32 CurrentAmmoClip;

	AShooterCharacter* PawnOwner;

private:

	// �Ƿ�װ������
	bool bIsEquipWeapon;

	// �Ƿ����װ��
	bool bIsReload;

	// �Ƿ���п���
	bool bIsFire;

	// �Ƿ���л�����
	bool bIsExchangeWeapon;

	// ��״̬
	EWeaponState::Type NewState;

	// ��ǰ״̬
	EWeaponState::Type State;

	// ��ǰ������������ nullptr ʱ, ���Ქ�Ż�ǹ����
	AShooterWeapon* LastWeapon;

	// ��������ʱ��
	FTimerHandle TimerHandler_OnEquipFinish;

	// ����ϻ,�ӵ����¶�ʱ��
	FTimerHandle TimerHandler_ReloadWeapon;

	// ֹͣ������ʱ��
	FTimerHandle TimerHandler_StopReload;

	// ������������ʱ��
	FTimerHandle TimerHandler_HandleFiring;

	// ����ʱ�����ж��ٴ����
	bool bRefiring;

	// 
};