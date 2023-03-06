// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"

#include "Demo220902.h"
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
	//AmmoCountMax = 5;
	//AmmoCount = AmmoCountMax;
	
	NewState = EWeaponState::Idle;
	State = EWeaponState::Idle;
	bPendingEquip = false;
	bIsEquipWeapon = false;
	bPendingReload = false;
	bIsFire = false;
	bRefiring = false;

	CurrentAmmo = 0;
	CurrentAmmoClip = 0;
}

// Called when the game starts or when spawned
void AShooterWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponConfig.InitalClip > 0)
	{
		CurrentAmmoClip = WeaponConfig.AmmoPerClip;
		CurrentAmmo = WeaponConfig.AmmoPerClip * WeaponConfig.InitalClip;
	}
}

// Called every frame
void AShooterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AShooterWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AShooterWeapon::GetCurrentAmmoClip() const
{
	return CurrentAmmoClip;
}

int32 AShooterWeapon::GetAmmoPerClip() const
{
	return WeaponConfig.AmmoPerClip;
}

void AShooterWeapon::UseAmmo()
{
	CurrentAmmoClip--;
	CurrentAmmo--;
}

FHitResult AShooterWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	// ׷��ͨ��������
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	// ׷�ٲ�ѯ����
	FCollisionQueryParams TraceParams(WeaponFireTag, true, GetInstigator());

	// �Ƿ�Ӧ�����첽������ִ�и���, 4.26 �Ѿ�û�����������
	//TraceParams.bTraceAsyncScene = true;

	// ׷��������ʵ�����
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);							// һ������ײ����Ϣ�Ľṹ

	// ʹ���ض���ͨ�����ٹ��ߣ������ص�һ����������
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

EWeaponState::Type AShooterWeapon::GetCurrentState() const
{
	return NewState;
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

void AShooterWeapon::DetachMeshFromPawn()
{
	//WeaponMesh1P->DetachFromComponent(FAttachmentTransformRules::KeepRelativeTransform);
	WeaponMesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);		// ����������������ӵ��κ�����з��롣�Զ����������һ������
	WeaponMesh1P->SetHiddenInGame(false);														// �ı�bHiddenInGame��ֵ�����Ϊfalse��������Ϸ�����в��ɼ�
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
		DetermineWeaponState();
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
		DetermineWeaponState();
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

//int AShooterWeapon::GetAmmoCount()
//{
//	return AmmoCount;
//}
//
//int AShooterWeapon::GetAmmoCountMax()
//{
//	return AmmoCountMax;
//}

// ȷ������״̬
void AShooterWeapon::DetermineWeaponState()
{
	EWeaponState::Type _NewState = EWeaponState::Idle;

	if (bIsEquipWeapon)		// �Ƿ�װ��������
	{
		if (bPendingReload)		// �Ƿ����װ��
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
		if (bPendingEquip)	// �Ƿ��������
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
	bool bGotAmmo = (CurrentAmmoClip < WeaponConfig.AmmoPerClip) && (CurrentAmmo - CurrentAmmoClip > 0);
	bool bStateOKToReload = (NewState == EWeaponState::Idle) || (NewState == EWeaponState::Firing);

	return bGotAmmo && bStateOKToReload;
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
	GetWorldTimerManager().ClearTimer(TimerHandler_HandleFiring);
	bRefiring = false;

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

	if (bPendingReload)
	{
		bPendingReload = false;
	}
	GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
	//GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
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

	bPendingEquip = true;
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
	if (!bPendingEquip)
	{
		bPendingEquip = true;
		DetermineWeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

void AShooterWeapon::OnUnEquip()
{
	// �˺�����Ӧ����״̬�����̹���

	// ж�� Mesh
	DetachMeshFromPawn();

	bIsEquipWeapon = false;

	StopFire();

	if (bPendingReload)
	{
		bPendingReload = false;

		// ֹͣ����װ������   to do...

		GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
		GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
	}

	if (bPendingEquip)
	{
		bPendingEquip = false;
		// ֹͣ���Ż��������� to do ...
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler_OnEquipFinish);
	}
	DetermineWeaponState();
}

// ��Ӧװ�����
void AShooterWeapon::OnEquipFinish()
{
	if (!bIsEquipWeapon)
	{
		bIsEquipWeapon = true;
	}

	if (!bPendingEquip)
	{
		bPendingEquip = false;
	}
	DetermineWeaponState();			// ȷ������״̬
	HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
}

void AShooterWeapon::StartReload()
{
	if (!bPendingReload && CanReload())
	{
		bPendingReload = true;
		DetermineWeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

void AShooterWeapon::StopReload()
{
	if (bPendingReload)
	{
		bPendingReload = false;
		DetermineWeaponState();			// ȷ������״̬
		HandleCurrentState();	// ���ݵ�ǰ״̬�����¼�
	}
}

void AShooterWeapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(WeaponConfig.AmmoPerClip - CurrentAmmoClip, CurrentAmmo - CurrentAmmoClip);
	if (ClipDelta > 0)
	{
		CurrentAmmoClip += ClipDelta;
	}
	CurrentAmmo = FMath::Max(CurrentAmmo, CurrentAmmoClip);
}

void AShooterWeapon::HandleFiring()
{
	if (CurrentAmmo <= 0)
	{
		// to do  ��ʾ�ӵ��������������û���ջ�����Ч
		return;
	}

	SimulateWeaponFire();			// ����ʱ�������Լ�������Ч�Ĵ���
	FireWeapon();					// ����ʱ�����������ӵ��Լ��ӵ�����λ�úͷ�����������ʵ��
	UseAmmo();						// ��ϻ�ӵ����� ���ӵ���������

	if (CurrentAmmoClip <= 0)
	{
		StartReload();
	}

	bRefiring = (NewState == EWeaponState::Firing) && WeaponConfig.TimeBetweenShots > 0.0f;
	if (bRefiring)
	{
		GetWorldTimerManager().SetTimer(TimerHandler_HandleFiring, this, &AShooterWeapon::HandleFiring, WeaponConfig.TimeBetweenShots, false);
	}
}
