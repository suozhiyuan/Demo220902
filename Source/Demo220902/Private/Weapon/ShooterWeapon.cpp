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
	WeaponMesh1P->CastShadow = false;													// 设置网络体组件是否投射阴影
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);					// 设置网络体碰撞的启用类型，不启用
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);								// 设置网络体碰撞的对象类型
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// 设置网络体对所有碰撞通道的响应为 ECR_Ignore 忽略

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//设置网络体的旋转用到的SetRelativeTransform

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
	// 追踪通道的名字
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	// 追踪查询参数
	FCollisionQueryParams TraceParams(WeaponFireTag, true, GetInstigator());

	// 是否应该在异步场景中执行跟踪, 4.26 已经没有这个参数了
	//TraceParams.bTraceAsyncScene = true;

	// 追踪物理材质的内容
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);							// 一个包含撞击信息的结构

	// 使用特定的通道跟踪光线，并返回第一个阻塞命中
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

EWeaponState::Type AShooterWeapon::GetCurrentState() const
{
	return NewState;
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

void AShooterWeapon::DetachMeshFromPawn()
{
	//WeaponMesh1P->DetachFromComponent(FAttachmentTransformRules::KeepRelativeTransform);
	WeaponMesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);		// 将该组件从它所连接的任何组件中分离。自动解除焊接在一起的组件
	WeaponMesh1P->SetHiddenInGame(false);														// 改变bHiddenInGame的值，如果为false，将在游戏过程中不可见
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

//int AShooterWeapon::GetAmmoCount()
//{
//	return AmmoCount;
//}
//
//int AShooterWeapon::GetAmmoCountMax()
//{
//	return AmmoCountMax;
//}

// 确定武器状态
void AShooterWeapon::DetermineWeaponState()
{
	EWeaponState::Type _NewState = EWeaponState::Idle;

	if (bIsEquipWeapon)		// 是否装备了武器
	{
		if (bPendingReload)		// 是否进行装弹
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
		if (bPendingEquip)	// 是否更换武器
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
	GetWorldTimerManager().ClearTimer(TimerHandler_HandleFiring);
	bRefiring = false;

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

	if (bPendingReload)
	{
		bPendingReload = false;
	}
	GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
	//GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
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

	bPendingEquip = true;
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
	if (!bPendingEquip)
	{
		bPendingEquip = true;
		DetermineWeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
	}
}

void AShooterWeapon::OnUnEquip()
{
	// 此函数不应纳入状态机流程管理

	// 卸载 Mesh
	DetachMeshFromPawn();

	bIsEquipWeapon = false;

	StopFire();

	if (bPendingReload)
	{
		bPendingReload = false;

		// 停止播放装弹动画   to do...

		GetWorldTimerManager().ClearTimer(TimerHandler_StopReload);
		GetWorldTimerManager().ClearTimer(TimerHandler_ReloadWeapon);
	}

	if (bPendingEquip)
	{
		bPendingEquip = false;
		// 停止播放换武器动画 to do ...
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler_OnEquipFinish);
	}
	DetermineWeaponState();
}

// 响应装备完成
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
	DetermineWeaponState();			// 确定武器状态
	HandleCurrentState();	// 根据当前状态处理事件
}

void AShooterWeapon::StartReload()
{
	if (!bPendingReload && CanReload())
	{
		bPendingReload = true;
		DetermineWeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
	}
}

void AShooterWeapon::StopReload()
{
	if (bPendingReload)
	{
		bPendingReload = false;
		DetermineWeaponState();			// 确定武器状态
		HandleCurrentState();	// 根据当前状态处理事件
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
		// to do  提示子弹数量不够，调用击针空击的音效
		return;
	}

	SimulateWeaponFire();			// 开火时，声音以及粒子特效的处理
	FireWeapon();					// 开火时，创建武器子弹以及子弹出现位置和方向，在子类中实现
	UseAmmo();						// 弹匣子弹数量 总子弹数量更新

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
