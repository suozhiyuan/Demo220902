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

	// ����������ĽǶȱ任 ȥ ͬ�� Mesh �ĽǶȱ任
	void OnCanmeraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation);

	// ǰ���ƶ�
	void MoveForward(float Value);

	// �����ƶ�
	void MoveRight(float Value);

	// ����
	void SpeedUp(float Value);

	// ��ȡ��һ�˳�Mesh
	USkeletalMeshComponent* GetMesh1P();

	// ��ȡ�����ҵ�
	virtual FName GetWeaponAttachPoint() const;

	// ��ȡ��׼����
	UFUNCTION(BlueprintCallable, Category = Weapon)
	FRotator GetAimOffsets() const;

	// ����
	void OnStartFire();

	// ֹͣ����
	void OnEndFire();

	// ��ʼ��׼
	void OnStartTargeting();

	// ������׼
	void OnEndTargeting();

	// ��ȡ��׼״̬
	UFUNCTION(BlueprintCallable, Category = Targeting)
	bool GetIsTargeting() const;

	// ������׼״̬
	void SetIsTargeting(bool NewIsTargeting);

	// �����ٶ�
	void SetSpeed(float UpSpeed);

	// ��ʼ����
	void OnStartSpeedUp();

	// ��������
	void OnEndSpeedUp();

	// ȡ�˺�
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// ��ȡ���ص�����
	AShooterWeapon* GetCurrentWeapon();

	// ��ȡ��ǰѪ��
	int32 GetHp() const;

	// ��ȡ���Ѫ��
	int32 GetMaxHp() const;

	// �Ƿ���Կ���
	bool CanFire() const;

	// �Ƿ���
	bool IsAlive() const;

	// ��������
	void OnReload();

protected:

	//// �������2023/02/01 ���ǵ�AI���ø��࣬�������Щ���࣬�����������õ�����������У�
	//UPROPERTY(EditAnywhere, Category = Camera)
	//UCameraComponent* Camera1P;


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

	// �����ٶ�
	UPROPERTY(EditAnywhere, Category = Speed)
	float WalkSpeed;

	// �����ٶ�
	UPROPERTY(EditAnywhere, Category = Speed)
	float RunSpeed;

	// ����ʱ�ƶ��ٶȳͷ�
	UPROPERTY(EditAnywhere, Category = Speed)
	float TargetingSpeed;

	// ����״̬
	bool IsSpeedUp;

	// ��ǰѪ��
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attr)
	float HP;

	// ���Ѫ��
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attr)
	float HPMax;

};
