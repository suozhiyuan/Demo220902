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

	// ��ײ����ĳ�ʼ��
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));			// ����������Ӷ���ģ������Ϊ�������ͣ�����Ϊ�������
	CollisionComp->InitSphereRadius(5.0f);													// �ڲ�������Ⱦ��������µ���������õ�����뾶
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// ������ײ���ͣ�Ϊ������������������ģ��
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);							// �����ƶ�ʱ����ײ��Ӧͨ��
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);							// ��Ӧͨ��, ECR_Ignore ����
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);		// �Դ��ھ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);		// �Դ��ڶ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);				// ��Pawn������ײ
	RootComponent = CollisionComp;			// ����Ϊ�����

	// һ��ר������Ͷ��������ƶ������ʼ��
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;									// �����ĸ�Component
	MovementComp->InitialSpeed = 2000.0f;											// ��ʼ���ٶ�
	MovementComp->MaxSpeed = 2000.0f;												// ����ٶ�
	MovementComp->bRotationFollowsVelocity = true;									// �Ƿ���ת
	MovementComp->ProjectileGravityScale = 0.0f;									// ������Ӱ����׹

	// �������ӷ�����
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;								// bAutoActivate: ������ڴ���ʱ����Ǳ�����ʽ���trueΪ����ʱ�ͼ����
	ParticleComp->bAutoDestroy = false;								// bAutoDestroy: �Ƿ��Զ�����
	ParticleComp->SetupAttachment(RootComponent);					// �������������λ��

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

// ��ʼ���ٶ�
void AShooterProjectile::InitVelocity(FVector ShooterDirection)
{
	// Velocity: ��������ĵ�ǰ�ٶ�
	MovementComp->Velocity = ShooterDirection * MovementComp->InitialSpeed;
}

// ��ײ��
void AShooterProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*
	 * ��ײ��
	 * OnProjectileStop���������Ѿ�ֹͣ(�ٶȵ���ģ����ֵ�����������ã���ǿ��ֹͣ)ʱ���á�
	 * AddDynamic��		��һ��UObjectʵ����һ����ԱUFUNCTION�󶨵�һ����̬������ת��ί�С�
	 */
	MovementComp->OnProjectileStop.AddDynamic(this, &AShooterProjectile::OnImpact);
	/*
	 * MoveIgnoreActors����ʱ���Զ���
	 * ��MoveComponent()�н������ɨ��ʱҪ���Ե�һ������ߡ�������ƶ�������ص�ʱ����Щ������ӵ�е�����������������ԡ�
	 * ��һ��Actor�ϵ�������ƶ�ʱҲ������Ҫ����֪����ͬ���Ĳ�������ģ������ʱ��Ӱ���������˶���
	 */
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	// �õ��������ӵ�ʵ����������������
	AShooterWeapon_Projectile* OwnerWeapon = Cast<AShooterWeapon_Projectile>(GetOwner());
	if (OwnerWeapon)
	{
		OwnerWeapon->GetWeaponConfig(WeaponConfig);
	}

	MyController = GetInstigatorController();
}

// ��ײ��������¼�
void AShooterProjectile::OnImpact(const FHitResult &ImpactResult)
{
	Explode(ImpactResult);
}

// ��ըЧ������
void AShooterProjectile::Explode(const FHitResult& ImpactResult)
{
	// ����һ������
	if (ParticleComp)
	{
		ParticleComp->Deactivate();
	}

	/*��ըλ��
	 *ImpactPoint���켣��״����Ӱ�����������ռ��е�ʵ�ʽӴ�λ�ã������ʼ���Ѿ��Ӵ���᷵�ص�ǰ����λ��
	 *
	 */
	const FVector NudgedImpactLocation = ImpactResult.ImpactPoint + ImpactResult.ImpactNormal + 10.0f;

	if (WeaponConfig.ExplosionDmg > 0.0 && WeaponConfig.ExplosionRad > 0.0)		// �˺��Ͱ뾶����0
	{
		/*
		 * ApplyRadialDamage���˺��뾶�ڵ��˺����㡣ֻ������赲�ɼ�ͨ���������
		 * @param WorldContextObject			- �н������ģ��������ĸ�����
		 * @param BaseDamage					- �����˺�����ըλ�õ��˺�
		 * @param Origin						- ��ըλ��
		 * @param DamageRadius					- ��ը�뾶
		 * @param DamageTypeClass				- ��ը�˺�����
		 * @param IgnoreActors					- �˺�����Actor�б���һ��ģ�����
		 * @param DamageCauser					- ��������˺��ĵ�λ(���籬ը������)�������ɫ�����ܵ��˺���Ҳ�����赲�˺�����NULLĬ��ֵ
		 * @param InstigatedByController		- ������������˺��Ŀ�����(���磬Ͷ�����񵯵����).��NULLĬ��ֵ
		 * @param bFullDamage					- ���Ϊtrue���˺��������ԭ��Զ�����١�Ĭ��ֵΪ��bool bDoFullDamage = false
		 * @param DamagePreventionChannel		- �����ԭ����ܺ���֮����ʲô������ס�����ͨ�����˺������ᱻӦ�õ��ܺ������ϡ�Ĭ��ֵΪ��ECollisionChannel DamagePreventionChannel = ECC_Visibility
		 * @return								- ����˺�����������һ�������ߣ���Ϊtrue
		*/
		UGameplayStatics::ApplyRadialDamage(this, WeaponConfig.ExplosionDmg, NudgedImpactLocation, WeaponConfig.ExplosionRad, WeaponConfig.DmgType, TArray<AActor*>(), this, MyController.Get());
	}

	// ���ɱ�ըЧ��
	if (ExplosionTemplate)
	{
		// ImpactResult.ImpactNormal.Rotation() ��ײʱ��λ����ת
		// NudgedImpactLocation ��ײʱ��λ��
		const FTransform SpawnTransform(ImpactResult.ImpactNormal.Rotation(), NudgedImpactLocation);
		AShooterExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<AShooterExplosionEffect>(ExplosionTemplate, SpawnTransform);
	}
}

