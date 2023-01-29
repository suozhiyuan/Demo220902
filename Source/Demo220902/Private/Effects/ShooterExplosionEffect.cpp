// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/ShooterExplosionEffect.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterExplosionEffect::AShooterExplosionEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionFX = nullptr;
}

// Called when the game starts or when spawned
void AShooterExplosionEffect::BeginPlay()
{
	Super::BeginPlay();

	if (ExplosionFX)
	{
		// �ڸ�����λ�ú���ת�ϲ���ָ����Ч�������ź�ɾ������Ч�����ʱ��ϵͳ����ʧ��������
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), GetActorRotation());
	}
}

// Called every frame
void AShooterExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

