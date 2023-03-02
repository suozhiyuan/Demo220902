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
	WeaponMesh1P->CastShadow = false;													// 设置网络体组件是否投射阴影
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);					// 设置网络体碰撞的启用类型，不启用
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);								// 设置网络体碰撞的对象类型
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// 设置网络体对所有碰撞通道的响应为 ECR_Ignore 忽略

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//设置网络体的旋转用到的SetRelativeTransform

	FireSound = nullptr;
	AmmoCountMax = 5;
	AmmoCount = AmmoCountMax;
}

// 设置武器当前的Pawn
void AShooterWeapon::SetPawnOwner(AShooterCharacter* pawnOwner)
{
	if (PawnOwner != pawnOwner)
	{
		SetInstigator(pawnOwner);
		PawnOwner = pawnOwner;
	}
}

// 创建Mesh到Pawn，设置创建后的状态
void AShooterWeapon::AttachMeshToPawn()
{
	if (PawnOwner)
	{
		FName AttachPoint = PawnOwner->GetWeaponAttachPoint();
		if (PawnOwner->GetMesh1P())
		{
			// 更改HiddenlnGame的值,是否在游戏中隐藏
			WeaponMesh1P->SetHiddenInGame(false);
			// 在组件上创建组件，参数1:父组件Mesh   参数2:附加规则  参数3:要创建在父组件的节点
			WeaponMesh1P->AttachToComponent(PawnOwner->GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
	}
}

// 获取眼睛方向
FVector AShooterWeapon::GetAdjustAim()
{
	FVector FinalAim = FVector::ZeroVector;

	// 获取当前控制器
	AShooterPlayerController* PlayerController = GetInstigator() ? Cast<AShooterPlayerController>(GetInstigator()->GetController()) : nullptr;
	if (PlayerController)
	{
		FVector CamerLocation;
		FRotator CamerRotator;
		PlayerController->GetPlayerViewPoint(CamerLocation, CamerRotator);		// 获取眼睛方向和位置
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

// 开火 
void AShooterWeapon::StartFire()
{
	SimulateWeaponFire();
	FireWeapon();
}

// 开火时，创建武器子弹以及子弹出现位置和方向
void AShooterWeapon::FireWeapon()
{
	//在子类实现
}

// 开火时，声音以及粒子特效的处理
void AShooterWeapon::SimulateWeaponFire()
{
	if (FireSound)
	{
		PlayWeaponSound(FireSound);
	}
}

// 播放声音组件
UAudioComponent* AShooterWeapon::PlayWeaponSound(USoundBase* Sound)
{
	UAudioComponent* AC = nullptr;

	if (Sound)
	{
		//AC = UGameplayStatics::SpawnSoundAttached(Cast<USoundBase>(Sound), PawnOwner->GetRootComponent());	// USoundCue 报错
		// SpawnSoundAttached: 播放附加在指定组件上的声音。这是一个一次性播放的声音。此时也不处理复制。
		AC = UGameplayStatics::SpawnSoundAttached(Sound, WeaponMesh1P);	
	}
	return AC;
}

// 获取枪口位置
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
	// 判断子弹数量，当前是否满弹药，以及备用弹药大于0
	return true;
}

void AShooterWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	OldState = State;
	State = NewState;
}

void AShooterWeapon::HandleCurrentState()
{
	using namespace  EWeaponState;

	if (OldState == Idle && State == Firing)
	{
		// 处理开火
		HandleStartFireState();
	}
	else if (OldState == Firing && (State == Idle || State == Reloading))
	{
		// 处理结束开火
		HandleEndFireState();
	}

	if ((OldState == Idle || OldState == Firing) && State == Reloading)
	{
		// 处理装弹
		HandleStartReloadState();
	}
	else if (OldState == Reloading && State == Idle)
	{
		// 处理停止装弹
		HandleEndReloadState();
	}

	if (OldState == Idle && State == Equiping)
	{
		// 处理换武器
		HandleStartEquipState();
	}
	else if (OldState == Equiping && State == Idle)
	{
		// 处理停止换武器
		HandleEndEquipState();
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

void AShooterWeapon::HandleStartReloadState()
{
	//to do ...
}

void AShooterWeapon::HandleEndReloadState()
{
	//to do ...
}

void AShooterWeapon::HandleStartEquipState()
{
}

void AShooterWeapon::HandleEndEquipState()
{
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


