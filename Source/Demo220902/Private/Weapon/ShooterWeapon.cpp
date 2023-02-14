// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"

// Sets default values
AShooterWeapon::AShooterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1PComponent"));
	WeaponMesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	WeaponMesh1P->CastShadow = false;													// ��������������Ƿ�Ͷ����Ӱ
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ������������ײ���������ͣ�������
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);						// ������������ײ�Ķ�������
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// �����������������ײͨ������ӦΪ ECR_Ignore ����

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//�������������ת�õ���SetRelativeTransform

	FireSound = nullptr;
}

// ����������ǰ��Pawn
void AShooterWeapon::SetPawnOwner(AShooterCharacter* pawnOwner)
{
	if (PawnOwner != pawnOwner)
	{
		SetInstigator(pawnOwner);
		PawnOwner = pawnOwner;
	}
}

// ����Mesh��Pawn�����ô������״̬
void AShooterWeapon::AttachMeshToPawn()
{
	if (PawnOwner)
	{
		FName AttachPoint = PawnOwner->GetWeaponAttachPoint();
		if (PawnOwner->GetMesh1P())
		{
			// ����HiddenlnGame��ֵ,�Ƿ�����Ϸ������
			WeaponMesh1P->SetHiddenInGame(false);
			// ������ϴ������������1:�����Mesh   ����2:���ӹ���  ����3:Ҫ�����ڸ�����Ľڵ�
			WeaponMesh1P->AttachToComponent(PawnOwner->GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
	}
}

// ��ȡ�۾�����
FVector AShooterWeapon::GetAdjustAim()
{
	FVector FinalAim = FVector::ZeroVector;

	// ��ȡ��ǰ������
	AShooterPlayerController* PlayerController = GetInstigator() ? Cast<AShooterPlayerController>(GetInstigator()->GetController()) : nullptr;
	if (PlayerController)
	{
		FVector CamerLocation;
		FRotator CamerRotator;
		PlayerController->GetPlayerViewPoint(CamerLocation, CamerRotator);		// ��ȡ�۾������λ��
		FinalAim = CamerRotator.Vector();
	}
	return FinalAim;
}

// ���� 
void AShooterWeapon::StartFire()
{
	SimulateWeaponFire();
	FireWeapon();
}

// ����ʱ�����������ӵ��Լ��ӵ�����λ�úͷ���
void AShooterWeapon::FireWeapon()
{
	//������ʵ��
}

// ����ʱ�������Լ�������Ч�Ĵ���
void AShooterWeapon::SimulateWeaponFire()
{
	if (FireSound)
	{
		PlayWeaponSound(FireSound);
	}
}

// �����������
UAudioComponent* AShooterWeapon::PlayWeaponSound(USoundBase* Sound)
{
	UAudioComponent* AC = nullptr;

	if (Sound)
	{
		//AC = UGameplayStatics::SpawnSoundAttached(Cast<USoundBase>(Sound), PawnOwner->GetRootComponent());	// USoundCue ����
		// SpawnSoundAttached: ���Ÿ�����ָ������ϵ�����������һ��һ���Բ��ŵ���������ʱҲ�������ơ�
		AC = UGameplayStatics::SpawnSoundAttached(Sound, WeaponMesh1P);	
	}
	return AC;
}

// ��ȡǹ��λ��
FVector AShooterWeapon::GetMuzzleLocation()
{
	if (WeaponMesh1P)
	{
		return	WeaponMesh1P->GetSocketLocation(MuzzleAttachPoint);
	}
	return FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AShooterWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


