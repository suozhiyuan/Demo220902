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
	WeaponMesh1P->CastShadow = false;													// 设置网络体组件是否投射阴影
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置网络体碰撞的启用类型，不启用
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);						// 设置网络体碰撞的对象类型
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// 设置网络体对所有碰撞通道的响应为 ECR_Ignore 忽略

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//设置网络体的旋转用到的SetRelativeTransform

	FireSound = nullptr;
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


