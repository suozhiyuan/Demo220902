// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Demo220902.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


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
	RootComponent = CollisionComp;			// 设置为根组件

	// 一个专门用于投射物体的移动组件初始化
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;									// 控制哪个Component
	MovementComp->InitialSpeed = 2000.0f;											// 初始化速度
	MovementComp->MaxSpeed = 2000.0f;												// 最大速度
	MovementComp->bRotationFollowsVelocity = true;									// 是否旋转
	MovementComp->ProjectileGravityScale = 0.0f;									// 重量，影响下坠

	// 创建粒子发射器
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;								// bAutoActivate: 组件是在创建时激活还是必须显式激活。true为创建时就激活播放
	ParticleComp->bAutoDestroy = false;								// bAutoDestroy: 是否自动销毁
	ParticleComp->SetupAttachment(RootComponent);					// 发射器组件挂载位置

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
	Super::PostInitializeComponents();
	/*
	 * 碰撞绑定
	 * OnProjectileStop：当弹丸已经停止(速度低于模拟阈值，反弹被禁用，或被强制停止)时调用。
	 * AddDynamic：		将一个UObject实例和一个成员UFUNCTION绑定到一个动态多类型转换委托。
	 */
	MovementComp->OnProjectileStop.AddDynamic(this, &AShooterProjectile::OnImpact);
	/*
	 * MoveIgnoreActors：动时忽略对象
	 * 在MoveComponent()中进行组件扫描时要忽略的一组参与者。当组件移动或更新重叠时，这些参与者拥有的所有组件都将被忽略。
	 * 另一个Actor上的组件在移动时也可能需要被告知进行同样的操作。在模拟物理时不影响该组件的运动。
	 */
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	// 拿到并保存子弹实例所属的武器配置
	AShooterWeapon_Projectile* OwnerWeapon = Cast<AShooterWeapon_Projectile>(GetOwner());
	if (OwnerWeapon)
	{
		OwnerWeapon->GetWeaponConfig(WeaponConfig);
	}

	MyController = GetInstigatorController();
}

// 碰撞后产生的事件
void AShooterProjectile::OnImpact(const FHitResult &ImpactResult)
{
	Explode(ImpactResult);
}

// 爆炸效果处理
void AShooterProjectile::Explode(const FHitResult& ImpactResult)
{
	// 创建一个粒子
	if (ParticleComp)
	{
		ParticleComp->Deactivate();
	}

	/*爆炸位置
	 *ImpactPoint：轨迹形状与受影响对象在世界空间中的实际接触位置，如果初始就已经接触则会返回当前对象位置
	 *
	 */
	const FVector NudgedImpactLocation = ImpactResult.ImpactPoint + ImpactResult.ImpactNormal + 10.0f;

	if (WeaponConfig.ExplosionDmg > 0.0 && WeaponConfig.ExplosionRad > 0.0)		// 伤害和半径大于0
	{
		/*
		 * ApplyRadialDamage：伤害半径内的伤害计算。只会击中阻挡可见通道的组件。
		 * @param WorldContextObject			- 承接上下文，来自于哪个对象
		 * @param BaseDamage					- 基础伤害，爆炸位置的伤害
		 * @param Origin						- 爆炸位置
		 * @param DamageRadius					- 爆炸半径
		 * @param DamageTypeClass				- 爆炸伤害类型
		 * @param IgnoreActors					- 伤害忽略Actor列表，是一个模板参数
		 * @param DamageCauser					- 真正造成伤害的单位(例如爆炸的手榴弹)。这个角色不会受到伤害，也不会阻挡伤害。有NULL默认值
		 * @param InstigatedByController		- 负责造成这种伤害的控制者(例如，投掷手榴弹的玩家).有NULL默认值
		 * @param bFullDamage					- 如果为true，伤害不会距离原点远而减少。默认值为：bool bDoFullDamage = false
		 * @param DamagePreventionChannel		- 如果在原点和受害者之间有什么东西挡住了这个通道，伤害将不会被应用到受害者身上。默认值为：ECollisionChannel DamagePreventionChannel = ECC_Visibility
		 * @return								- 如果伤害作用于至少一个参与者，则为true
		*/
		UGameplayStatics::ApplyRadialDamage(this, WeaponConfig.ExplosionDmg, NudgedImpactLocation, WeaponConfig.ExplosionRad, WeaponConfig.DmgType, TArray<AActor*>(), this, MyController.Get());
	}

	// 生成爆炸效果
	if (ExplosionTemplate)
	{
		// ImpactResult.ImpactNormal.Rotation() 碰撞时的位置旋转
		// NudgedImpactLocation 碰撞时的位置
		const FTransform SpawnTransform(ImpactResult.ImpactNormal.Rotation(), NudgedImpactLocation);
		AShooterExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<AShooterExplosionEffect>(ExplosionTemplate, SpawnTransform);
	}
}

