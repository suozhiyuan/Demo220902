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



	// Called every frame
	virtual void Tick(float DeltaTime) override;


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

	// ��װ����Ч
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* EquipSound;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int AmmoCountMax;

	int AmmoCount;

private:
	AShooterCharacter* PawnOwner;

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

	// ��ǰ����
	AShooterWeapon* LastWeapon;

	// ��������ʱ��
	FTimerHandle TimerHandler_OnEquipFinish;

	// ����ϻ,�ӵ����¶�ʱ��
	FTimerHandle TimerHandler_ReloadWeapon;

	// ֹͣ������ʱ��
	FTimerHandle TimerHandler_StopReload;
};