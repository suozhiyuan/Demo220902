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
		Equiping		// 装备
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


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 设置武器当前的Pawn
	void SetPawnOwner(AShooterCharacter* pawnOwner);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int AmmoCountMax;

	int AmmoCount;

private:
	AShooterCharacter* PawnOwner;

	// 是否装备武器
	bool bIsEquip;

	// 是否装子弹
	bool bIsReload;

	// 是否开火
	bool bIsFire;

	// 是否换武器
	bool bIsExchangeWeapon;

};
