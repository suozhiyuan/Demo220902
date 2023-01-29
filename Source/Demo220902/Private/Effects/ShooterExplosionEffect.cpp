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
		// 在给定的位置和旋转上播放指定的效果，播放和删除。当效果完成时，系统将消失。不复制
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), GetActorRotation());
	}
}

// Called every frame
void AShooterExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

