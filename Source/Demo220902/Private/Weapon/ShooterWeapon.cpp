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
	WeaponMesh1P->CastShadow = false;													// ��������������Ƿ�Ͷ����Ӱ
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);					// ������������ײ���������ͣ�������
	WeaponMesh1P->SetCollisionObjectType(ECC_WorldDynamic);								// ������������ײ�Ķ�������
	WeaponMesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);						// �����������������ײͨ������ӦΪ ECR_Ignore ����

	RootComponent = WeaponMesh1P;
	FTransform NewTransform(FRotator(0.0f, 0.0f, -90.0f));
	WeaponMesh1P->SetRelativeTransform(NewTransform);									//�������������ת�õ���SetRelativeTransform
}

// ����������ǰ��Pawn
void AShooterWeapon::SetPawnOwner(AShooterCharacter* PawnOwner1)
{
	PawnOwner = PawnOwner1;
}

// ����Mesh��Pawn�����ô������״̬
void AShooterWeapon::AttachMeshToPawn()
{
	if (PawnOwner)
	{
		//USkeletalMeshComponent* PawnMesh1P = PawnOwner->GetMesh1P();
		FName AttachPoint = PawnOwner->GetWeaponAttachPoint();
		if (PawnOwner->GetMesh1P())
		{
			// ����HiddenlnGame��ֵ,�Ƿ�����Ϸ������
			WeaponMesh1P->SetHiddenInGame(false);
			// ������ϴ������������1:�����Mesh   ����2:���ӹ���  ����3:Ҫ�����ڸ�����Ľڵ�
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


