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

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 AmmoPerClip;					// 弹匣容量

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 InitalClip;					// 弹匣数量

	UPROPERTY(EditDefaultsOnly, Category = WeaponState)
	float TimeBetweenShots;				// 单发间隔时间

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


	// 创建 Mesh 到 Pawn
	void AttachMeshToPawn();


	// 获取眼睛方向
	FVector GetAdjustAim();

	// 开火
	void StartFire();

	// 停火
	void StopFire();

	// 开火，创建武器子弹以及子弹出现位置和方向
	virtual void FireWeapon();

	// 模拟开火
	void SimulateWeaponFire();

	// 停止模拟开火
	void StopSimulateWeaponFire();

	// 音效组件
	UAudioComponent* PlayWeaponSound(USoundBase* Sound);

	// 获取枪口位置
	FVector GetMuzzleLocation();

	//// 获取当前子弹数量
	//int GetAmmoCount();

	//// 获取最大子弹数量
	//int GetAmmoCountMax();

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

	// 处理开火状态
	void HandleFiring();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 获取当前子弹数量
	int32 GetCurrentAmmo() const;

	// 获取弹匣当前子弹数量
	int32 GetCurrentAmmoClip() const;

	// 获取弹匣容量
	int32 GetAmmoPerClip() const;

	// 弹匣子弹数量 总子弹数量更新
	void UseAmmo();

	// 返回伤害对象
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

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

	// 换枪音效
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* EquipSound;

	// 换弹音效
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ReloadSound;

	//// 最大子弹数量
	//UPROPERTY(EditDefaultsOnly, Category = Ammo)
	//int AmmoCountMax;

	//// 当前子弹数量
	//int AmmoCount;

	// 武器属性数据
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	// 当前可用子弹数（未使用的 + 弹匣里剩下的）
	UPROPERTY(Transient)
	int32 CurrentAmmo;

	// 当前弹匣可用子弹数量
	UPROPERTY(Transient)
	int32 CurrentAmmoClip;

	AShooterCharacter* PawnOwner;

private:

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

	// 当前武器，当等于 nullptr 时, 不会播放换枪动画
	AShooterWeapon* LastWeapon;

	// 换武器定时器
	FTimerHandle TimerHandler_OnEquipFinish;

	// 换弹匣,子弹更新定时器
	FTimerHandle TimerHandler_ReloadWeapon;

	// 停止换弹定时器
	FTimerHandle TimerHandler_StopReload;

	// 连发射击间隔定时器
	FTimerHandle TimerHandler_HandleFiring;

	// 连发时用来判断再次射击
	bool bRefiring;

	// 
};