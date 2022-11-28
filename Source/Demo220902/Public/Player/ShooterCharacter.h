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
	// 获取第一人称Mesh
	USkeletalMeshComponent* GetMesh1P();

	// 获取武器挂点
	FName GetWeaponAttachPoint() const;

	// 获取瞄准方向
	UFUNCTION(BlueprintCallable, Category = Weapon)
	FRotator GetAimOffsets() const;

	// 开始瞄准
	void OnStartTargeting();

	// 结束瞄准
	void OnStopTargeting();

	// 获取瞄准状态
	UFUNCTION(BlueprintCallable, Category = Targeting)
	bool GetIsTargeting() const;

	// 设置瞄准状态
	bool SetIsTargeting(bool bNewIsTargeting) const;

protected:
	// 摄像机
	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* CaTargetmera1P;

	// SkeletalMeshComponent用于创建动画SkeletalMesh资产的实例。
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	// 为了暴露到蓝图当中去，这里用一个TSubclassOf
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AShooterWeapon> ShooterWeaponClass;

	// 当前武器
	AShooterWeapon* CurrentWeapon;

	// 武器挂点名称
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName WeaponAttachPoint;

	// 瞄准状态
	bool IsTargeting;
};
