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

	// ��ʼ���ٶ�
	void InitVelocity(FVector ShooterDirection);

	// ��ײ�󶨣�����actor�����������ʼ������c++�˳�ʼ���Լ�������GamePlay�ڼ����
	void PostInitializeComponents() override;

	// ��ײ��������¼�
	void OnImpact(const FHitResult& ImpactResult);

	// ��ըЧ������
	void Explode(const FHitResult& ImpactResult);


protected:
	// һ��������ײ���
	UPROPERTY(EditDefaultsOnly,Category = Projectile)
	USphereComponent* CollisionComp;

	// һ��ר������Ͷ��������ƶ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

	// �ӵ���������������
	struct FProjectileWeaponData WeaponConfig;

	// ��ſ�������AController�Ǽ̳�Object������Ҫ��TWeakObjectPtr
	TWeakObjectPtr<AController> MyController;
};
