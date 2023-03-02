// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"

#include "Bots/ShooterAIController.h"
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
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);					// ������������ײ���������ͣ�������
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);								// ������������ײ�Ķ�������
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// �����������������ײͨ������ӦΪ ECR_Ignore ����

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//�������������ת�õ���SetRelativeTransform

	FireSound = nullptr;
	AmmoCountMax = 5;
	AmmoCount = AmmoCountMax;
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
	else if (GetInstigator())
	{
		AShooterAIController* AIController = PawnOwner ? Cast<AShooterAIController>(PawnOwner->GetController()) : nullptr;
		if (AIController)
		{
			FinalAim = AIController->GetControlRotation().Vector();
		}
		else
		{
			FinalAim = GetInstigator()->GetBaseAimRotation().Vector();
		}
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

int AShooterWeapon::GetAmmoCount()
{
	return AmmoCount;
}

int AShooterWeapon::GetAmmoCountMax()
{
	return AmmoCountMax;
}

void AShooterWeapon::WeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;

	if (bIsEquipWeapon)
	{
		if (bIsReload)
		{
			if (CanReload())
			{
				NewState = EWeaponState::Reloading;
			}
		}
		else
		{
			if (bIsFire && CanFire())
			{
				NewState = EWeaponState::Firing;
			}
			else
			{
				NewState = State;
			}
		}
	}
	else
	{
		if (bIsExchangeWeapon)
		{
			NewState = EWeaponState::Equiping;
		}
	}
	SetWeaponState(NewState);
}

bool AShooterWeapon::CanFire() const
{
	bool bCanFire = PawnOwner && PawnOwner->CanFire();

	return bCanFire;
}

bool AShooterWeapon::CanReload() const
{
	// to do ...
	// �ж��ӵ���������ǰ�Ƿ�����ҩ���Լ����õ�ҩ����0
	return true;
}

void AShooterWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	OldState = State;
	State = NewState;
}

void AShooterWeapon::HandleCurrentState()
{
	if (OldState == EWeaponState::Idle && State == EWeaponState::Firing)
	{
		// ������
		HandleStartFireState();
	}
	else if (OldState == EWeaponState::Firing && (State == EWeaponState::Idle || State == EWeaponState::Reloading))
	{
		// ����ͣ��
		HandleEndFireState();
	}

}

void AShooterWeapon::HandleStartFireState()
{
	//to do ...
}

void AShooterWeapon::HandleEndFireState()
{
	//to do ...
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


