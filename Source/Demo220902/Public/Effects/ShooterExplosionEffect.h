// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterExplosionEffect.generated.h"

UCLASS()
class DEMO220902_API AShooterExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterExplosionEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*	一个粒子系统是一个完整的粒子效果，包含任意数量的粒子发射器。
	 *	通过在一个系统中允许多个发射器，设计师可以在一个系统中创建精细的粒子效果。
	 *	一旦使用Cascade创建，就可以将ParticleSystem插入到关卡中或在脚本中创建。*/
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	UParticleSystem* ExplosionFX;
};
