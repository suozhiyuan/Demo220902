// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void InitVelocity(FVector ShooterDirection);

protected:
	// һ��������ײ���
	UPROPERTY(EditDefaultsOnly,Category = Projectile)
	USphereComponent* CollisionComp;

	// һ��ר������Ͷ��������ƶ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

};
