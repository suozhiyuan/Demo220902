// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeapon.h"

// Sets default values
AShooterWeapon::AShooterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1PComponent"));
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->CastShadow = false;													// ��������������Ƿ�Ͷ����Ӱ
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ������������ײ���������ͣ�������
	Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);						// ������������ײ�Ķ�������
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// �����������������ײͨ������ӦΪ ECR_Ignore ����

	RootComponent = Mesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	Mesh1P->SetRelativeTransform(NewTransform);									//�������������ת�õ���SetRelativeTransform
}

void AShooterWeapon::AttachMeshToPawn()
{
	// �������
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

// ����������
void AShooterWeapon::SetPawnOwner(AShooterCharacter* PawnOwner)
{

}

