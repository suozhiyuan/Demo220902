// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/ShooterWeapon.h"
#include "ShooterCharacter.generated.h"


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

	virtual void PostInitializeComponents() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

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
	AShooterWeapon* CurrentWeapon;
};
