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
		Idle,			// 闲置
		Firing,			// 发射
		Reloading,		// 重新装填
		Equiping		// 更换武器
	};
}

UCLASS()
class DEMO220902_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterWeapon();


	// 创建 Mesh 到 Pawn
	void AttachMeshToPawn();


	// 获取眼睛方向
	FVector GetAdjustAim();

	// 开火
	void StartFire();

	// 开火，创建武器子弹以及子弹出现位置和方向
	virtual void FireWeapon();

	//模拟开火
	void SimulateWeaponFire();

	//音效组件
	UAudioComponent* PlayWeaponSound(USoundBase* Sound);

	// 获取枪口位置
	FVector GetMuzzleLocation();

	// 获取当前子弹数量
	int GetAmmoCount();

	// 获取最大子弹数量
	int GetAmmoCountMax();

	// 确定武器状态
	void WeaponState();

	// 是否可以开火
	bool CanFire() const;

	// 是否可以换弹，判断子弹数量，弹匣当前是否满弹药，以及备用弹药大于0
	bool CanReload() const;

	// 设置武器状态
	void SetWeaponState(EWeaponState::Type NewState);

	// 根据武器状态处理响应事件
	void HandleCurrentState();

	// 处理开始开火状态
	void HandleStartFireState();

	// 处理结束开火状态
	void HandleEndFireState();

	// 处理装弹状态
	void HandleStartReloadState();

	// 处理结束装弹状态
	void HandleEndReloadState();

	// 处理换装备状态
	void HandleStartEquipState();

	// 处理结束换装备状态
	void HandleEndEquipState();

	// 响应更换装备
	void OnEquip(const AShooterWeapon* _LastWeapon);

	// 设置武器当前的Pawn
	void SetPawnOwner(AShooterCharacter* pawnOwner);

	// 装弹
	void StartReload();

	// 停止装弹
	void StopReload();

	// 武器更新子弹
	void ReloadWeapon();



	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 响应装备完成
	void OnEquipFinish();

protected:
	// SkeletalMeshComponent用于创建动画SkeletalMesh资产的实例，当要创建一个网络体时用到USkeletalMeshComponent。
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh1P;

	// 子弹在枪口的位置
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName MuzzleAttachPoint;

	//开火音效
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* FireSound;

	// 换装备音效
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* EquipSound;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int AmmoCountMax;

	int AmmoCount;

private:
	AShooterCharacter* PawnOwner;

	// 是否装备武器
	bool bIsEquipWeapon;

	// 是否进行装弹
	bool bIsReload;

	// 是否进行开火
	bool bIsFire;

	// 是否进行换武器
	bool bIsExchangeWeapon;

	// 新状态
	EWeaponState::Type NewState;

	// 当前状态
	EWeaponState::Type State;

	// 当前武器
	AShooterWeapon* LastWeapon;

	// 换武器定时器
	FTimerHandle TimerHandler_OnEquipFinish;

	// 换弹匣,子弹更新定时器
	FTimerHandle TimerHandler_ReloadWeapon;

	// 停止换弹定时器
	FTimerHandle TimerHandler_StopReload;
};