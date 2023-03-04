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
	
	NewState = EWeaponState::Idle;
	State = EWeaponState::Idle;
	bIsExchangeWeapon = false;
	bIsEquipWeapon = false;
	bIsReload = false;
	bIsFire = false;
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
			// ���� HiddenlnGame ��ֵ,�Ƿ�����Ϸ������
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
	if (!bIsFire)
	{
		bIsFire = true;
		WeaponState();
		HandleCurrentState();
	}
	//SimulateWeaponFire();
	//FireWeapon();
}

void AShooterWeapon::StopFire()
{
	if (bIsFire)
	{
		bIsFire = false;
		WeaponState();
		HandleCurrentState();
	}
}

// ����ʱ�����������ӵ��Լ��ӵ�����λ�úͷ���
void AShooterWeapon::FireWeapon()
{
	//������ʵ��
}

// ����ʱ�������Լ�������Ч�Ĵ���
void AShooterWeapon::SimulateWeaponFire()
{
	// ���ſ��𶯻�
	// to do ...

	if (FireSound)
	{
		PlayWeaponSound(FireSound);
	}
}

void AShooterWeapon::StopSimulateWeaponFire()
{
	// ֹͣ���ſ��𶯻�
	// to do ...
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

// ȷ������״̬
void AShooterWeapon::WeaponState()
{
	EWeaponState::Type _NewState = EWeaponState::Idle;

	if (bIsEquipWeapon)		// �Ƿ�װ��������
	{
		if (bIsReload)		// �Ƿ����װ��
		{
			if (CanReload())
			{
				_NewState = EWeaponState::Reloading;
			}
		}
		else
		{
			if (bIsFire && CanFire())	
			{
				_NewState = EWeaponState::Firing;
			}
			else
			{
				_NewState = EWeaponState::Idle;
			}
		}
	}
	else
	{
		if (bIsExchangeWeapon)	// �Ƿ��������
		{
			_NewState = EWeaponState::Equiping;
		}
	}
	SetWeaponState(_NewState);
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

void AShooterWeapon::SetWeaponState(EWeaponState::Type _NewState)
{
	State = NewState;
	NewState = _NewState;
}

void AShooterWeapon::HandleCurrentState()
{
	using namespace  EWeaponState;

	if (State == Idle && NewState == Firing)
	{
		// ������
		HandleStartFireState();
	}
	else if (State == Firing && (NewState == Idle || NewState == Reloading))
	{
		// �����������
		HandleEndFireState();
	}

	if ((State == Idle || State == Firing) && NewState == Reloading)
	{
		// ����װ��
		HandleStartReloadState();
	}
	else if (State == Reloading && NewState == Idle)
	{
		// ����ֹͣװ��
		HandleEndReloadState();
	}

	if (State == Idle && NewState == Equiping)
	{
		// ��������
		HandleStartEquipState();
	}
	else if (State == Equiping && NewState == Idle)
	{
		// ����ֹͣ������
		HandleEndEquipState();
	}
}

void AShooterWeapon::HandleStartFireState()
{
	HandleFiring();
}

void AShooterWeapon::HandleEndFireState()
{
	StopSimulateWeaponFire();
}

void AShooterWeapon::HandleStartReloadState()
{
	// ����װ�ӵ��Ķ���
	// ����װ�ӵ�������
	//to do ...

	float AnimTime = 0.0f;
	if (AnimTime <= 0.0f)
	{
		AnimTime = 0.5f;
	}
	GetWorldTimerManager().SetTimer(TimerHandler_StopReload, this, &AShooterWeapon::StopReload, AnimTime, false);
	GetWorldTimerManager().SetTimer(TimerHandler_ReloadWeapon, this, &AShooterWeapon::ReloadWeapon, FMath::Max(0.1f, AnimTime - 0.1f), false);

	if (PawnOwner)
	{
		PlayWeaponSound(ReloadSound);
	}
	
}

void AShooterWeapon::HandleEndReloadState()
{
	//ֹͣ���Ż��ӵ��Ķ���
	// to do ...
	GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
	GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
}

// ��������
void AShooterWeapon::HandleStartEquipState()
{
	AttachMeshToPawn();

	if (LastWeapon != nullptr)		// ��ǹ
	{
		// ���Ż�ǹ����
		float EquipTime = 2.0f;		// ��ǹʱ��
		/*
		 * InOutHandle		�������ľ��������һ�����еļ�ʱ������ô������¼�ʱ��֮ǰ�������������������������£����᷵���¼�ʱ�����¾����������ö�ʱ��������
		 * InObj			������ö�ʱ��������
		 * InTimerMethod	�ڼ�ʱ������ʱ���õķ�����
		 * InRate			���úʹ���֮���ʱ����(��)�����<= 0.f��������ж�ʱ����
		 * Inbloop		 true��ʾ�����ʼ�����������false��ʾֻ����һ�Σ�Ĭ��ֵfalse��
		 * InFirstDelay		ѭ����ʱ����һ�ε�����ʱ��(��)��Ĭ��ֵ-1
		 */
		GetWorldTimerManager().SetTimer(TimerHandler_OnEquipFinish, this, &AShooterWeapon::OnEquipFinish, EquipTime, false);
	}
	else							//��ɫ��ʼ����������
	{
		OnEquipFinish();
	}

	if (PawnOwner && EquipSound)
	{
		PlayWeaponSound(EquipSound);
	}

	bIsExchangeWeapon = true;
}

void AShooterWeapon::HandleEndEquipState()
{
	GetWorldTimerManager().ClearTimer(TimerHandler_OnEquipFinish);		// �����ʱ��

	// ֹͣ���������Ĳ���
	// to do...
}

// ��Ӧ����װ��
void AShooterWeapon::OnEquip(const AShooterWeapon* _LastWeapon)
{
	LastWeapon = (AShooterWeapon*)_LastWeapon;
	if (!bIsExchangeWeapon)
	{
		bIsExchangeWeapon = true;
		WeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

// ��Ӧװ�����
void AShooterWeapon::OnEquipFinish()
{
	if (!bIsEquipWeapon)
	{
		bIsEquipWeapon = true;
	}

	if (!bIsExchangeWeapon)
	{
		bIsExchangeWeapon = false;
	}
	WeaponState();			// ȷ������״̬
	HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
}

void AShooterWeapon::StartReload()
{
	if (!bIsReload && CanReload())
	{
		bIsReload = true;
		WeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

void AShooterWeapon::StopReload()
{
	if (bIsReload)
	{
		bIsReload = false;
		WeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

void AShooterWeapon::ReloadWeapon()
{
	//to do ...
}

void AShooterWeapon::HandleFiring()
{
	if (AmmoCount <0)
	{
		// to do  ��ʾ�ӵ���������
	}
	else
	{
		SimulateWeaponFire();			// ����ʱ�������Լ�������Ч�Ĵ���
		FireWeapon();					// ����ʱ�����������ӵ��Լ��ӵ�����λ�úͷ�����������ʵ��
		// to do...  �����ӵ�����
	}
}
