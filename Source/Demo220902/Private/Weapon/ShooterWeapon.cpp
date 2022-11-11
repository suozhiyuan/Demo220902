// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"

// Sets default values
AShooterWeapon::AShooterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1PComponent"));
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->CastShadow = false;													// 组件是否投射阴影
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);				// 设置碰撞的启用类型，不启用
	Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);							// 碰撞的对象类型
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// 设置对所有碰撞通道的响应为 ECR_Ignore 忽略

	RootComponent = Mesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	Mesh1P->SetRelativeTransform(NewTransform);

}

void AShooterWeapon::AttachMeshToPawn()
{
	//AttachToComponent();
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

