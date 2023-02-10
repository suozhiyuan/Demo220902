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

	/*	һ������ϵͳ��һ������������Ч���������������������ӷ�������
	 *	ͨ����һ��ϵͳ�������������������ʦ������һ��ϵͳ�д�����ϸ������Ч����
	 *	һ��ʹ��Cascade�������Ϳ��Խ�ParticleSystem���뵽�ؿ��л��ڽű��д�����*/
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	UParticleSystem* ExplosionFX;
};
