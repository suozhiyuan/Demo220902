// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/ShooterImpactEffect.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterImpactEffect::AShooterImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DefaultFX)
	{
		// 在给定的位置和旋转上播放指定的效果，生成和移除。当效果完成时将消失，不复制。
		UGameplayStatics::SpawnEmitterAtLocation(this, DefaultFX, GetActorLocation(), GetActorRotation());
	}

	if (DefaultSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DefaultSound, GetActorLocation());
	}
}

// Called every frame
void AShooterImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

