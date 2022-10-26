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
	 * 角色相机
	 */
	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera1P"));										/*CreateDefaultSubobject 创建一个组件或子对象。
	*/
	Camera1P->SetupAttachment(GetCapsuleComponent());														/*挂载在根组件，那个胶囊体上
	*/
	Camera1P->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));									/*设置组件的位置（相对于父组件），BaseEyeHeight为眼睛高度
	*/
	Camera1P->bUsePawnControlRotation = true;																		/*如果该组件被放置到一个Pawn上，是否使用这个 Pawn 的视角控件旋转 (初始值本来就是True)
	*/
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh1P"));
	Mesh1P->SetupAttachment(Camera1P);																				/*挂载位置
	*/
	Mesh1P->bOnlyOwnerSee = true;																					/*bOnlyOwnerSee 如果为True，只对组件所有者的视图可见。
	*/
	Mesh1P->bOwnerNoSee = false;																					/*很奇怪还有 bOwnerNoSee 如果为True，只对组件所有者的视图不可见。
	*/
	Mesh1P->bReceivesDecals = false;																				/*是否接受贴花（弹孔）
	*/
	Mesh1P->bCastDynamicShadow = false;																				/*是否投射阴影
	*/
	/* 
	 * 该方法和枚举在4.21废弃
	 * MeshComponentUpdateFlag 改为了 VisibilityBasedAnimTickOption
	 * EMeshComponentUpdateFlag 改为了 EVisibilityBasedAnimTickOption	 */
	// Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;				/* 该骨骼动画Mesh的Pose只在渲染的时候更新
	*/
	Mesh1P->PrimaryComponentTick.TickGroup = TG_EndPhysics;															/*设置该Mesh的更新组， TG_EndPhysics 结束物理模拟的特殊标记组。
	*/
	// 由于第三人称 Mesh 有具体的碰撞设置，这里选择关闭第一人称的碰撞
	Mesh1P->SetCollisionObjectType(ECC_Pawn);																	/*设置 Mesh 的碰撞类型
	*/
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);												/*设置 Mesh 的物理模拟，以及可以空间查询(光线投射，扫描，重叠),由于第三人称已经有了这里用 NoCollision
	*/
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);															/* 更改此原生组件的所有ResponseToChannels容器为传入参数
	*/
	Mesh1P->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f - 86.0f));

	/*
	 * Mesh 设置
	 */
	GetMesh()->bOnlyOwnerSee = false;																				/*bOnlyOwnerSee 如果为True，只对组件所有者的视图可见。
	*/
	GetMesh()->bOwnerNoSee = true;																					/*很奇怪还有 bOwnerNoSee 如果为True，只对组件所有者的视图不可见。
	*/
	GetMesh()->bReceivesDecals = false;																				/*是否接受贴花（弹孔）
	*/
	GetMesh()->SetCollisionObjectType(ECC_Pawn);																/*设置 Mesh 的碰撞类型
	*/
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);										/*设置 Mesh 的物理模拟，以及可以空间查询(光线投射，扫描，重叠)
	*/
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);									/*设置 Mesh 对三个碰撞通道的响应
	*/
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Block);

	/*
	 * 设置胶囊体对三个碰撞通道的响应,
	 *  ECR_Ignore 忽略
	 *  ECR_Overlap	重叠
	 *  ECR_Block 阻挡
	 */
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);		
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);		//  设置胶囊只对弹药阻挡
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
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::AddControllerYawInput);			// Yaw 左右
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);		// Pitch 上下
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		/*
		 * 帮助查看移动输入是否被忽略。如果没有Pawn或者UpdatedComponent，返回true，否则根据Pawn的IsMoveInputIgnored()实现
		 * 是否在下落
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