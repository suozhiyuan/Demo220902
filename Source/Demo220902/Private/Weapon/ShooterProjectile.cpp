// Fill out your copyright notice in the Description page of Project Settings.


#include "Demo220902.h"
#include "Weapon/ShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AShooterProjectile::AShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 碰撞组件的设置
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));			// 创建组件或子对象，模板类型为创建类型，参数为组件名称
	CollisionComp->InitSphereRadius(5.0f);													// 在不触发渲染或物理更新的情况下设置的球体半径
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// 设置碰撞类型，为减少运算量不做物理模拟
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);							// 物体移动时的碰撞响应通道
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);							// 响应通道, ECR_Ignore 忽略
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);		// 对处于静态类型的物体产生碰撞
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);		// 对处于动态类型的物体产生碰撞
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);				// 对Pawn产生碰撞
	RootComponent = CollisionComp;		// 设置为根组件

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

}

// Called when the game starts or when spawned
void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterProjectile::InitVelocity(FVector ShooterDirection)
{
	// 
}

