// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"
#include "Player/ShooterCharacter.h"

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
}

// 设置武器当前的Pawn
void AShooterWeapon::SetPawnOwner(AShooterCharacter* PawnOwner1)
{
	PawnOwner = PawnOwner1;
}

// 创建Mesh到Pawn，设置创建后的状态
void AShooterWeapon::AttachMeshToPawn()
{
	if (PawnOwner)
	{
		//USkeletalMeshComponent* PawnMesh1P = PawnOwner->GetMesh1P();
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


