// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"


class USoundCue;
class AShooterCharacter;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 设置武器当前的Pawn
	void SetPawnOwner(AShooterCharacter* pawnOwner);

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


private:
	AShooterCharacter* PawnOwner;

};
