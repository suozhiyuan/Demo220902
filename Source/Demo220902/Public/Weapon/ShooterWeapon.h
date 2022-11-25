// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

UCLASS()
class DEMO220902_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterWeapon();

	void AttachMeshToPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;						// SkeletalMeshComponent���ڴ�������SkeletalMesh�ʲ���ʵ������Ҫ����һ��������ʱ�õ�USkeletalMeshComponent��

};
