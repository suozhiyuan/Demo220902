// Fill out your copyright notice in the Description page of Project Settings.


#include "Demo220902.h"
#include "Player/ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	 * ��ɫ���
	 */
	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera1P"));										/*CreateDefaultSubobject ����һ��������Ӷ���
	*/
	Camera1P->SetupAttachment(GetCapsuleComponent());														/*�����ڸ�������Ǹ���������
	*/
	Camera1P->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));									/*���������λ�ã�����ڸ��������BaseEyeHeightΪ�۾��߶�
	*/
	Camera1P->bUsePawnControlRotation = true;																		/*�������������õ�һ��Pawn�ϣ��Ƿ�ʹ����� Pawn ���ӽǿؼ���ת (��ʼֵ��������True)
	*/
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh1P"));
	Mesh1P->SetupAttachment(Camera1P);																				/*����λ��
	*/
	Mesh1P->bOnlyOwnerSee = true;																					/*bOnlyOwnerSee ���ΪTrue��ֻ����������ߵ���ͼ�ɼ���
	*/
	Mesh1P->bOwnerNoSee = false;																					/*����ֻ��� bOwnerNoSee ���ΪTrue��ֻ����������ߵ���ͼ���ɼ���
	*/
	Mesh1P->bReceivesDecals = false;																				/*�Ƿ�������������ף�
	*/
	Mesh1P->bCastDynamicShadow = false;																				/*�Ƿ�Ͷ����Ӱ
	*/
	/* 
	 * �÷�����ö����4.21����
	 * MeshComponentUpdateFlag ��Ϊ�� VisibilityBasedAnimTickOption
	 * EMeshComponentUpdateFlag ��Ϊ�� EVisibilityBasedAnimTickOption	 */
	// Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;				/* �ù�������Mesh��Poseֻ����Ⱦ��ʱ�����
	*/
	Mesh1P->PrimaryComponentTick.TickGroup = TG_EndPhysics;															/*���ø�Mesh�ĸ����飬 TG_EndPhysics ��������ģ����������顣
	*/
	// ���ڵ����˳� Mesh �о������ײ���ã�����ѡ��رյ�һ�˳Ƶ���ײ
	Mesh1P->SetCollisionObjectType(ECC_Pawn);																	/*���� Mesh ����ײ����
	*/
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);												/*���� Mesh ������ģ�⣬�Լ����Կռ��ѯ(����Ͷ�䣬ɨ�裬�ص�),���ڵ����˳��Ѿ����������� NoCollision
	*/
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);															/* ���Ĵ�ԭ�����������ResponseToChannels����Ϊ�������
	*/
	Mesh1P->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f - 86.0f));

	/*
	 * Mesh ����
	 */
	GetMesh()->bOnlyOwnerSee = false;																				/*bOnlyOwnerSee ���ΪTrue��ֻ����������ߵ���ͼ�ɼ���
	*/
	GetMesh()->bOwnerNoSee = true;																					/*����ֻ��� bOwnerNoSee ���ΪTrue��ֻ����������ߵ���ͼ���ɼ���
	*/
	GetMesh()->bReceivesDecals = false;																				/*�Ƿ�������������ף�
	*/
	GetMesh()->SetCollisionObjectType(ECC_Pawn);																/*���� Mesh ����ײ����
	*/
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);										/*���� Mesh ������ģ�⣬�Լ����Կռ��ѯ(����Ͷ�䣬ɨ�裬�ص�)
	*/
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);									/*���� Mesh ��������ײͨ������Ӧ
	*/
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Block);

	/*
	 * ���ý������������ײͨ������Ӧ,
	 *  ECR_Ignore ����
	 *  ECR_Overlap	�ص�
	 *  ECR_Block �赲
	 */
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);		
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);		//  ���ý���ֻ�Ե�ҩ�赲
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Ignore);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::AddControllerYawInput);			// Yaw ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);		// Pitch ����
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		/*
		 * �����鿴�ƶ������Ƿ񱻺��ԡ����û��Pawn����UpdatedComponent������true���������Pawn��IsMoveInputIgnored()ʵ��
		 * �Ƿ�������
		 */
		bool bLimitRotation = (GetCharacterMovement()->IsMoveInputIgnored() || GetCharacterMovement()->IsFalling());
		FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr)
	{
		bool bLimitRotation = (GetCharacterMovement()->IsMoveInputIgnored() || GetCharacterMovement()->IsFalling());
		FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}