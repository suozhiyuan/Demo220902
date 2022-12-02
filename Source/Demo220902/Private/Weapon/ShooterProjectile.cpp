// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Demo220902.h"

// Sets default values
AShooterProjectile::AShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 碰撞组件的初始化
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));			// 创建组件或子对象，模板类型为创建类型，参数为组件名称
	CollisionComp->InitSphereRadius(5.0f);													// 在不触发渲染或物理更新的情况下设置的球体半径
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// 设置碰撞类型，为减少运算量不做物理模拟
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);							// 物体移动时的碰撞响应通道
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);							// 响应通道, ECR_Ignore 忽略
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);		// 对处于静态类型的物体产生碰撞
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);		// 对处于动态类型的物体产生碰撞
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);				// 对Pawn产生碰撞
	RootComponent = CollisionComp;		// 设置为根组件

	// 一个专门用于投射物体的移动组件初始化
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;						// 控制哪个Component
	MovementComp->InitialSpeed = 2000.0f;								// 初始化速度
	MovementComp->MaxSpeed = 2000.0f;									// 最大速度
	MovementComp->bRotationFollowsVelocity = true;						// 是否旋转
	MovementComp->ProjectileGravityScale = 0.0f;						// 重量，影响下坠
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

// 初始化速度
void AShooterProjectile::InitVelocity(FVector ShooterDirection)
{
	// Velocity: 更新组件的当前速度
	MovementComp->Velocity = ShooterDirection * MovementComp->InitialSpeed;
}

// 碰撞绑定
void AShooterProjectile::PostInitializeComponents()
{
	/*
	 * 碰撞绑定
	 * OnProjectileStop：当弹丸已经停止(速度低于模拟阈值，反弹被禁用，或被强制停止)时调用。
	 * AddDynamic：		将一个UObject实例和一个成员UFUNCTION绑定到一个动态多类型转换委托。参数:UserObjectUObject instanceFuncName指向成员UFUNCTION的函数指针，通常形式为&UClassName::FunctionName
	 */
	MovementComp->OnProjectileStop.AddDynamic(this, &AShooterProjectile::OnImpact);
	/*
	 * MoveIgnoreActors：动时忽略对象
	 * 在MoveComponent()中进行组件扫描时要忽略的一组参与者。
	 * 当组件移动或更新重叠时，这些参与者拥有的所有组件都将被忽略。
	 * 另一个Actor上的组件在移动时也可能需要被告知进行同样的操作。
	 * 在模拟物理时不影响该组件的运动。
	 */
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());
}

void AShooterProjectile::OnImpact(const FHitResult& 
)
{
	//0
}

