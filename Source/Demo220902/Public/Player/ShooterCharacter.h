// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "ShooterCharacter.generated.h"


class AShooterWeapon;
UCLASS()
class DEMO220902_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 构建组件初始化
	virtual void PostInitializeComponents() override;
	// 前后移动
	void MoveForward(float Value);
	// 左右移动
	void MoveRight(float Value);
	// 加速
	void SpeedUp(float Value);

	// 获取第一人称Mesh
	USkeletalMeshComponent* GetMesh1P();

	// 获取武器挂点
	FName GetWeaponAttachPoint() const;

	// 获取瞄准方向
	UFUNCTION(BlueprintCallable, Category = Weapon)
	FRotator GetAimOffsets() const;

	// 开火
	void OnStartFire();

	// 停止开火
	void OnEndFire();

	// 开始瞄准
	void OnStartTargeting();

	// 结束瞄准
	void OnEndTargeting();

	// 获取瞄准状态
	UFUNCTION(BlueprintCallable, Category = Targeting)
	bool GetIsTargeting() const;

	// 设置瞄准状态
	void SetIsTargeting(bool NewIsTargeting);

	// 设置速度
	void SetSpeed(float UpSpeed);

	// 开始加速
	void OnStartSpeedUp();

	// 结束加速
	void OnEndSpeedUp();

	// 取伤害
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 设置显示瞄准十字状态
	void SetIsShowDrawCrosshair(bool ToSet);

	// 获取显示瞄准十字状态
	bool GetIsShowDrawCrosshair();


protected:
	// 摄像机
	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* Camera1P;

	// SkeletalMeshComponent用于创建动画SkeletalMesh资产的实例。
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	// 为了暴露到蓝图当中去，这里用一个TSubclassOf
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AShooterWeapon> ShooterWeaponClass;

	// 当前武器
	//AShooterWeapon* CurrentWeapon;
	AShooterWeapon* CurrentWeapon;

	// 武器挂点名称
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName WeaponAttachPoint;

	// 瞄准状态
	bool IsTargeting;

	// 行走速度
	UPROPERTY(EditAnywhere, Category = Speed)
	float WalkSpeed;

	// 奔跑速度
	UPROPERTY(EditAnywhere, Category = Speed)
	float RunSpeed;

	//开镜时移动速度惩罚
	UPROPERTY(EditAnywhere, Category = Speed)
	float TargetingSpeed;

	// 加速状态
	bool IsSpeedUp;

	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attr)
	float HP;

	//是否显示瞄准十字HUD
	bool IsShowDrawCrosshair;



};
