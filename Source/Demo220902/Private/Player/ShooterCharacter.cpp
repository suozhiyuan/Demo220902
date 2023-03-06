// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterCharacter.h"
#include "Demo220902.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ShooterPlayerController.h"
#include "Weapon/ShooterWeapon.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	 * 角色相机	（2023/02/01 考虑到AI调用该类，摄像机有些多余，摄像机将会放置到相机管理器中）
	 */
	//Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera1P"));		/*CreateDefaultSubobject 创建一个组件或子对象。*/
	//Camera1P->SetupAttachment(GetCapsuleComponent());									/*挂载在根组件，那个胶囊体上*/
	//Camera1P->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));				/*设置组件的位置（相对于父组件），BaseEyeHeight为眼睛高度*/
	//Camera1P->bUsePawnControlRotation = true;											/*如果该组件被放置到一个Pawn上，是否使用这个 Pawn 的视角控件旋转 (初始值本来就是True)*/


	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh1P"));					// 创建组件
	Mesh1P->SetupAttachment(GetCapsuleComponent());											/*挂载位置*/
	Mesh1P->bOnlyOwnerSee = true;																	/*bOnlyOwnerSee 如果为True，只对组件所有者的视图可见。
	*/
	Mesh1P->bOwnerNoSee = false;																	/*很奇怪还有 bOwnerNoSee 如果为True，只对组件所有者的视图不可见。
	*/
	Mesh1P->bReceivesDecals = false;																/*是否接受贴花（弹孔）
	*/
	Mesh1P->bCastDynamicShadow = false;																/*是否投射阴影
	*/
	/* 
	 * Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	 * MeshComponentUpdateFlag 该方法和枚举在4.21废弃
	 * MeshComponentUpdateFlag 改为了 VisibilityBasedAnimTickOption
	 * EMeshComponentUpdateFlag 改为了 EVisibilityBasedAnimTickOption	 */
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;				/* 该骨骼动画Mesh的Pose只在渲染的时候更新
	*/
	Mesh1P->PrimaryComponentTick.TickGroup = TG_EndPhysics;															/*设置该Mesh的更新组， TG_EndPhysics 结束物理模拟的特殊标记组。
	*/
	// 由于第三人称 Mesh 有具体的碰撞设置，这里选择关闭第一人称的碰撞
	Mesh1P->SetCollisionObjectType(ECC_Pawn);																	/*设置 Mesh 的碰撞类型
	*/
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);												/*设置 Mesh 的碰撞启用类型，以及可以空间查询(光线投射，扫描，重叠),由于第三人称已经有了这里用 NoCollision
	*/
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);															/* 更改此原生组件的所有ResponseToChannels容器为传入参数
	*/
	//Mesh1P->SetRelativeLocation(FVector(0.0f - 5.0f, 0.0f, 0.0f - 150.0f));				// 设置相对位置便宜

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
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);										/*设置 Mesh 的碰撞启用类型，以及可以空间查询(光线投射，扫描，重叠)
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
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);							//  设置胶囊只对弹药阻挡
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Ignore);

	WalkSpeed = 250.f;										// 行走速度
	RunSpeed =  150.f;										// 奔跑额外增加速度
	TargetingSpeed = -160.f;								// 开镜时速度惩罚
	IsSpeedUp = false;										// 初始化加速状态
	IsTargeting = false;									// 瞄准状态初始化
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 速度初始化

	HPMax = 300;
	HP = HPMax;
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
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);						// 前后
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);							// 左右
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::AddControllerYawInput);					// Yaw 鼠标左右
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);				// Pitch 鼠标上下

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterCharacter::OnStartFire);				// 按下左键开火
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShooterCharacter::OnEndFire);				// 放开左键停火

	PlayerInputComponent->BindAction(TEXT("Target"), IE_Pressed, this, &AShooterCharacter::OnStartTargeting);		// 按下右键瞄准
	PlayerInputComponent->BindAction(TEXT("Target"), IE_Released, this, &AShooterCharacter::OnEndTargeting);		// 放开右键瞄准

	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Pressed, this, &AShooterCharacter::OnStartSpeedUp);			// 按下加速
	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Released, this, &AShooterCharacter::OnEndSpeedUp);			// 放开还原


	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::OnReload);					// 换弹
	PlayerInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, this, &AShooterCharacter::OnNextWeapon);			// 换枪
}

// 构建组件初始化
void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//FActorSpawnParameters SpawnInfo;				// FActorSpawnParameters 是一个传递给 SpawnActor 函数的可选参数的结构，用于指定创建时的各项参数
	//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;				// AlwaysSpawn: Actor将在所需的位置生成，无论碰撞与否
	//CurrentWeapon = GetWorld()->SpawnActor<AShooterWeapon>(ShooterWeaponClass, SpawnInfo);					// 在GetWorld调用SpawnActor方法创建Actor，模板指定其创建类型，SpawnActor的参数为指定的类和创建的方式
	//if (CurrentWeapon)
	//{
	//	CurrentWeapon->SetPawnOwner(this);			// 设置组件创建时的Pawn
	//	//CurrentWeapon->AttachMeshToPawn();		// 创建 Mesh 到 Pawn
	//	CurrentWeapon->OnEquip(nullptr);
	//}

	SpawnDefaultInventory();
}

// 根据摄像机的角度变换 去 同步 Mesh 的角度变换
void AShooterCharacter::OnCanmeraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	// SkeletalMeshComponent用于创建动画SkeletalMesh资产的实例。
	USkeletalMeshComponent* DefMesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("PawnMesh1P")));

	// 第一人称Mesh相对于根组件的矩阵，并赋值第一人称手臂在局部空间的位置信息
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->GetRelativeRotation(), DefMesh1P->GetRelativeLocation());

	// 根组件相对于世界空间的矩阵
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	// 相机的 Yaw 和 Pitch
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);
	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);

	// 相机相对于根组件的，左右旋转变化，Inverse 取反
	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();

	// Mesh 相对于相机矩阵
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();

	// 利用相机相对于根组件的上下旋转变化，得出 Mesh1P 到根组件的上下旋转变换
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;

	// 计算出 Mesh1P 相对于根组件的矩阵
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

	// 设置相对于根组件的位置以及旋转，GetOrigin 会返回矩阵的0点
	Mesh1P->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
}


// 前后移动
void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		/*
		 * 帮助查看移动输入是否被忽略。如果没有Pawn或者UpdatedComponent，返回true，否则根据Pawn的IsMoveInputIgnored()实现
		 * 是否在下落
		 */
		//bool bLimitRotation = (GetCharacterMovement()->IsMoveInputIgnored() || GetCharacterMovement()->IsFalling());
		//FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		//FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		//AddMovementInput(Direction, Value);

		FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// 左右移动
void AShooterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr)
	{
		//bool bLimitRotation = (GetCharacterMovement()->IsMoveInputIgnored() || GetCharacterMovement()->IsFalling());
		//FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		//FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		//AddMovementInput(Direction, Value);
		FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// 获取第一人称Mesh
USkeletalMeshComponent* AShooterCharacter::GetMesh1P()
{
	return Mesh1P;
}

// 获取武器挂点
FName AShooterCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

// 获取瞄准方向
FRotator AShooterCharacter::GetAimOffsets() const
{
	// 获取世界坐标
	FVector AimDirectionW = GetBaseAimRotation().Vector();
	// 世界坐标转换自身坐标
	FVector AimDirectionL = ActorToWorld().InverseTransformVectorNoScale(AimDirectionW);

	return AimDirectionL.Rotation();
}

// 开火
void AShooterCharacter::OnStartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

// 停止开火
void AShooterCharacter::OnEndFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}


// 开始瞄准
void AShooterCharacter::OnStartTargeting()
{
	if (AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller))		// 用控制器区分是角色还是Ai
	{
		SetIsTargeting(true);
		SetSpeed(TargetingSpeed);
	}
}

// 结束瞄准
void AShooterCharacter::OnEndTargeting()
{
	SetIsTargeting(false);
	SetSpeed(TargetingSpeed * -1);
}

// 获取瞄准状态
bool AShooterCharacter::GetIsTargeting() const
{
	return IsTargeting;
}

// 设置瞄准状态
void AShooterCharacter::SetIsTargeting(bool NewIsTargeting)
{
	IsTargeting = NewIsTargeting;
}

// 设置速度
void AShooterCharacter::SetSpeed(float UpSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed += UpSpeed;
}

// 开始加速
void AShooterCharacter::OnStartSpeedUp()
{
	if (!IsSpeedUp)
	{
		SetSpeed(RunSpeed);
		IsSpeedUp = true;
	}
}

// 结束加速
void AShooterCharacter::OnEndSpeedUp()
{
	if (IsSpeedUp)
	{
		SetSpeed(RunSpeed * -1);
		IsSpeedUp = false;
	}
}

// 取伤害
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActuaDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActuaDamage > 0.0)
	{
		HP -= ActuaDamage;
	}
	return ActuaDamage;
}

// 获取挂载的武器
AShooterWeapon* AShooterCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 AShooterCharacter::GetHp() const
{
	return HP;
}

int32 AShooterCharacter::GetMaxHp() const
{
	// GetClass()->GetDefaultObject<AShooterCharacter>()->HP;    // 通过 GetDefaultObject 获取到Hp的最初值
	return HPMax;
}

bool AShooterCharacter::CanFire() const
{
	bool bCanFire = IsAlive();

	return bCanFire;
}

bool AShooterCharacter::IsAlive() const
{
	return HP > 0;
}

void AShooterCharacter::OnReload()
{
	
	if (AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller))
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StartReload();
		}
	}
}

void AShooterCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClass.Num();
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClass[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AShooterWeapon* NewWeapon =  GetWorld()->SpawnActor<AShooterWeapon>(DefaultInventoryClass[i], SpawnInfo);
			if (NewWeapon)
			{
				AddWeapon(NewWeapon);
			}
		}
	}
	if (Inventory.Num() > 0)
	{
		CurrentWeapon = Inventory[0];
		CurrentWeapon->OnEquip(nullptr);
	}
}

void AShooterCharacter::AddWeapon(AShooterWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->SetPawnOwner(this);
		Inventory.AddUnique(Weapon);
	}
}

void AShooterCharacter::OnNextWeapon()
{
	AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller);
	if (MyPC)
	{
		if (Inventory.Num() > 0 && CurrentWeapon->GetCurrentState() != EWeaponState::Equiping)
		{
			const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
			const int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Inventory.Num();
			AShooterWeapon* NextWeapon = Inventory[NextWeaponIndex];
			EquipWeapon(NextWeapon);
		}
	}
}

void AShooterCharacter::EquipWeapon(AShooterWeapon* Weapon)
{
	if (Weapon && CurrentWeapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void AShooterCharacter::SetCurrentWeapon(AShooterWeapon* NewWeapon, AShooterWeapon* LastWeapon)
{
	AShooterWeapon* LocalLastWeapon = nullptr;			// 存储的上一把武器

	if (LastWeapon)										// 初始时 LastWeapon 一定是空的，如果不为空，那么这把武器将会成为 LocalLastWeapon
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)				// 如果 LastWeapon 为空，那么要看新武器与当前武器是不是一样，如果不一样就会进行替换，吧当前武器给到旧武器
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();					// 卸载旧武器
	}

	CurrentWeapon = NewWeapon;							// 将新武器赋给当前武器

	if (NewWeapon)
	{
		NewWeapon->SetPawnOwner(this);
		NewWeapon->OnEquip(LastWeapon);
	}
}

