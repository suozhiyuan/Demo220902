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

	// ���� Mesh �� Pawn
	void AttachMeshToPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ����������ǰ��Pawn
	void SetPawnOwner(AShooterCharacter* PawnOwner);

	// SkeletalMeshComponent���ڴ�������SkeletalMesh�ʲ���ʵ������Ҫ����һ��������ʱ�õ�USkeletalMeshComponent��
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh1P;

private:
	AShooterCharacter* PawnOwner;

};
