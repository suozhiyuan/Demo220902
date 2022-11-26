// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShooterWeapon.generated.h"


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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 设置武器当前的Pawn
	void SetPawnOwner(AShooterCharacter* PawnOwner);

	// SkeletalMeshComponent用于创建动画SkeletalMesh资产的实例，当要创建一个网络体时用到USkeletalMeshComponent。
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh1P;

private:
	AShooterCharacter* PawnOwner;

};
