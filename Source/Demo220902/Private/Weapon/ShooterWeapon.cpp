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
			// 更改 HiddenlnGame 的值,是否在游戏中隐藏
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

// 开火时，创建武器子弹以及子弹出现位置和方向
void AShooterWeapon::FireWeapon()
{
	//在子类实现
}

// 开火时，声音以及粒子特效的处理
void AShooterWeapon::SimulateWeaponFire()
{
	// 播放开火动画
	// to do ...

	if (FireSound)
	{
		PlayWeaponSound(FireSound);
	}
}

void AShooterWeapon::StopSimulateWeaponFire()
{
	// 停止播放开火动画
	// to do ...
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

// 确定武器状态
void AShooterWeapon::WeaponState()
{
	EWeaponState::Type _NewState = EWeaponState::Idle;

	if (bIsEquipWeapon)		// 是否装备了武器
	{
		if (bIsReload)		// 是否进行装弹
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
		if (bIsExchangeWeapon)	// 是否更换武器
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
	// 判断子弹数量，当前是否满弹药，以及备用弹药大于0
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
		// 处理开火
		HandleStartFireState();
	}
	else if (State == Firing && (NewState == Idle || NewState == Reloading))
	{
		// 处理结束开火
		HandleEndFireState();
	}

	if ((State == Idle || State == Firing) && NewState == Reloading)
	{
		// 处理装弹
		HandleStartReloadState();
	}
	else if (State == Reloading && NewState == Idle)
	{
		// 处理停止装弹
		HandleEndReloadState();
	}

	if (State == Idle && NewState == Equiping)
	{
		// 处理换武器
		HandleStartEquipState();
	}
	else if (State == Equiping && NewState == Idle)
	{
		// 处理停止换武器
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
	// 播放装子弹的动画
	// 播放装子弹的声音
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
	//停止播放换子弹的动画
	// to do ...
	GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
	GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
}

// 处理换武器
void AShooterWeapon::HandleStartEquipState()
{
	AttachMeshToPawn();

	if (LastWeapon != nullptr)		// 换枪
	{
		// 播放换枪动画
		float EquipTime = 2.0f;		// 换枪时间
		/*
		 * InOutHandle		如果传入的句柄引用了一个现有的计时器，那么在添加新计时器之前，它将被清除。在这两种情况下，都会返回新计时器的新句柄。对象调用定时器函数。
		 * InObj			对象调用定时器函数。
		 * InTimerMethod	在计时器触发时调用的方法。
		 * InRate			设置和触发之间的时间量(秒)。如果<= 0.f，清除现有定时器。
		 * Inbloop		 true表示以速率间隔继续射击，false表示只触发一次，默认值false。
		 * InFirstDelay		循环计时器第一次迭代的时间(秒)，默认值-1
		 */
		GetWorldTimerManager().SetTimer(TimerHandler_OnEquipFinish, this, &AShooterWeapon::OnEquipFinish, EquipTime, false);
	}
	else							//角色开始创建的情形
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
	GetWorldTimerManager().ClearTimer(TimerHandler_OnEquipFinish);		// 清除计时器

	// 停止武器动画的播放
	// to do...
}

// 响应更换装备
void AShooterWeapon::OnEquip(const AShooterWeapon* _LastWeapon)
{
	LastWeapon = (AShooterWeapon*)_LastWeapon;
	if (!bIsExchangeWeapon)
	{
		bIsExchangeWeapon = true;
		WeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
	}
}

// 响应装备完成
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
	WeaponState();			// 确定武器状态
	HandleCurrentState();	// 根据当前状态处理事件
}

void AShooterWeapon::StartReload()
{
	if (!bIsReload && CanReload())
	{
		bIsReload = true;
		WeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
	}
}

void AShooterWeapon::StopReload()
{
	if (bIsReload)
	{
		bIsReload = false;
		WeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
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
		// to do  提示子弹数量不够
	}
	else
	{
		SimulateWeaponFire();			// 开火时，声音以及粒子特效的处理
		FireWeapon();					// 开火时，创建武器子弹以及子弹出现位置和方向，在子类中实现
		// to do...  更新子弹数量
	}
}
