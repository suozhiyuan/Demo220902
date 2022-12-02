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

	// ��ײ����ĳ�ʼ��
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));			// ����������Ӷ���ģ������Ϊ�������ͣ�����Ϊ�������
	CollisionComp->InitSphereRadius(5.0f);													// �ڲ�������Ⱦ��������µ���������õ�����뾶
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// ������ײ���ͣ�Ϊ������������������ģ��
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);							// �����ƶ�ʱ����ײ��Ӧͨ��
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);							// ��Ӧͨ��, ECR_Ignore ����
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);		// �Դ��ھ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);		// �Դ��ڶ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);				// ��Pawn������ײ
	RootComponent = CollisionComp;		// ����Ϊ�����

	// һ��ר������Ͷ��������ƶ������ʼ��
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;						// �����ĸ�Component
	MovementComp->InitialSpeed = 2000.0f;								// ��ʼ���ٶ�
	MovementComp->MaxSpeed = 2000.0f;									// ����ٶ�
	MovementComp->bRotationFollowsVelocity = true;						// �Ƿ���ת
	MovementComp->ProjectileGravityScale = 0.0f;						// ������Ӱ����׹
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
	/*
	 * ��ײ��
	 * OnProjectileStop���������Ѿ�ֹͣ(�ٶȵ���ģ����ֵ�����������ã���ǿ��ֹͣ)ʱ���á�
	 * AddDynamic��		��һ��UObjectʵ����һ����ԱUFUNCTION�󶨵�һ����̬������ת��ί�С�����:UserObjectUObject instanceFuncNameָ���ԱUFUNCTION�ĺ���ָ�룬ͨ����ʽΪ&UClassName::FunctionName
	 */
	MovementComp->OnProjectileStop.AddDynamic(this, &AShooterProjectile::OnImpact);
	/*
	 * MoveIgnoreActors����ʱ���Զ���
	 * ��MoveComponent()�н������ɨ��ʱҪ���Ե�һ������ߡ�
	 * ������ƶ�������ص�ʱ����Щ������ӵ�е�����������������ԡ�
	 * ��һ��Actor�ϵ�������ƶ�ʱҲ������Ҫ����֪����ͬ���Ĳ�����
	 * ��ģ������ʱ��Ӱ���������˶���
	 */
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());
}

void AShooterProjectile::OnImpact(const FHitResult& 
)
{
	//0
}

