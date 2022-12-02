// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterWeapon_Projectile.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class DEMO220902_API AShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 初始化速度
	void InitVelocity(FVector ShooterDirection);

	// 碰撞绑定，允许actor在所有组件初始化后在c++端初始化自己，仅在GamePlay期间调用
	void PostInitializeComponents() override;

	// 碰撞后产生的事件
	void OnImpact(const FHitResult& ImpactResult);

	// 爆炸效果处理
	void Explode(const FHitResult& ImpactResult);


protected:
	// 一个球形碰撞组件
	UPROPERTY(EditDefaultsOnly,Category = Projectile)
	USphereComponent* CollisionComp;

	// 一个专门用于投射物体的移动组件
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

	// 子弹所属武器的配置
	struct FProjectileWeaponData WeaponConfig;

	// 存放控制器，AController是继承Object，所以要用TWeakObjectPtr
	TWeakObjectPtr<AController> MyController;
};
