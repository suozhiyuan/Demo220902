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

	// ��ײ���������
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));			// ����������Ӷ���ģ������Ϊ�������ͣ�����Ϊ�������
	CollisionComp->InitSphereRadius(5.0f);													// �ڲ�������Ⱦ��������µ���������õ�����뾶
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// ������ײ���ͣ�Ϊ������������������ģ��
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);							// �����ƶ�ʱ����ײ��Ӧͨ��
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);							// ��Ӧͨ��, ECR_Ignore ����
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);		// �Դ��ھ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);		// �Դ��ڶ�̬���͵����������ײ
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);				// ��Pawn������ײ
	RootComponent = CollisionComp;		// ����Ϊ�����

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

