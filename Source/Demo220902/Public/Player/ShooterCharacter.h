// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "ShooterCharacter.generated.h"


class AShooterWeapon;
UCLASS()
class DEMO220902_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ���������ʼ��
	virtual void PostInitializeComponents() override;
	// ǰ���ƶ�
	void MoveForward(float Value);
	// �����ƶ�
	void MoveRight(float Value);
	// ��ȡ��һ�˳�Mesh
	USkeletalMeshComponent* GetMesh1P();

	// ��ȡ�����ҵ�
	FName GetWeaponAttachPoint() const;

	// ��ȡ��׼����
	UFUNCTION(BlueprintCallable, Category = Weapon)
	FRotator GetAimOffsets() const;

	// ��ʼ��׼
	void OnStartTargeting();

	// ������׼
	void OnStopTargeting();

	// ��ȡ��׼״̬
	UFUNCTION(BlueprintCallable, Category = Targeting)
	bool GetIsTargeting() const;

	// ������׼״̬
	bool SetIsTargeting(bool bNewIsTargeting) const;

protected:
	// �����
	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* CaTargetmera1P;

	// SkeletalMeshComponent���ڴ�������SkeletalMesh�ʲ���ʵ����
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	// Ϊ�˱�¶����ͼ����ȥ��������һ��TSubclassOf
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AShooterWeapon> ShooterWeaponClass;

	// ��ǰ����
	AShooterWeapon* CurrentWeapon;

	// �����ҵ�����
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName WeaponAttachPoint;

	// ��׼״̬
	bool IsTargeting;
};
