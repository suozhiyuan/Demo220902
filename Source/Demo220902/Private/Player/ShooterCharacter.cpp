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
	 * ��ɫ���	��2023/02/01 ���ǵ�AI���ø��࣬�������Щ���࣬�����������õ�����������У�
	 */
	//Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera1P"));		/*CreateDefaultSubobject ����һ��������Ӷ���*/
	//Camera1P->SetupAttachment(GetCapsuleComponent());									/*�����ڸ�������Ǹ���������*/
	//Camera1P->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));				/*���������λ�ã�����ڸ��������BaseEyeHeightΪ�۾��߶�*/
	//Camera1P->bUsePawnControlRotation = true;											/*�������������õ�һ��Pawn�ϣ��Ƿ�ʹ����� Pawn ���ӽǿؼ���ת (��ʼֵ��������True)*/


	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh1P"));					// �������
	Mesh1P->SetupAttachment(GetCapsuleComponent());											/*����λ��*/
	Mesh1P->bOnlyOwnerSee = true;																	/*bOnlyOwnerSee ���ΪTrue��ֻ����������ߵ���ͼ�ɼ���
	*/
	Mesh1P->bOwnerNoSee = false;																	/*����ֻ��� bOwnerNoSee ���ΪTrue��ֻ����������ߵ���ͼ���ɼ���
	*/
	Mesh1P->bReceivesDecals = false;																/*�Ƿ�������������ף�
	*/
	Mesh1P->bCastDynamicShadow = false;																/*�Ƿ�Ͷ����Ӱ
	*/
	/* 
	 * Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	 * MeshComponentUpdateFlag �÷�����ö����4.21����
	 * MeshComponentUpdateFlag ��Ϊ�� VisibilityBasedAnimTickOption
	 * EMeshComponentUpdateFlag ��Ϊ�� EVisibilityBasedAnimTickOption	 */
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;				/* �ù�������Mesh��Poseֻ����Ⱦ��ʱ�����
	*/
	Mesh1P->PrimaryComponentTick.TickGroup = TG_EndPhysics;															/*���ø�Mesh�ĸ����飬 TG_EndPhysics ��������ģ����������顣
	*/
	// ���ڵ����˳� Mesh �о������ײ���ã�����ѡ��رյ�һ�˳Ƶ���ײ
	Mesh1P->SetCollisionObjectType(ECC_Pawn);																	/*���� Mesh ����ײ����
	*/
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);												/*���� Mesh ����ײ�������ͣ��Լ����Կռ��ѯ(����Ͷ�䣬ɨ�裬�ص�),���ڵ����˳��Ѿ����������� NoCollision
	*/
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);															/* ���Ĵ�ԭ�����������ResponseToChannels����Ϊ�������
	*/
	//Mesh1P->SetRelativeLocation(FVector(0.0f - 5.0f, 0.0f, 0.0f - 150.0f));				// �������λ�ñ���

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
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);										/*���� Mesh ����ײ�������ͣ��Լ����Կռ��ѯ(����Ͷ�䣬ɨ�裬�ص�)
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
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);							//  ���ý���ֻ�Ե�ҩ�赲
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Ignore);

	WalkSpeed = 250.f;										// �����ٶ�
	RunSpeed =  150.f;										// ���ܶ��������ٶ�
	TargetingSpeed = -160.f;								// ����ʱ�ٶȳͷ�
	IsSpeedUp = false;										// ��ʼ������״̬
	IsTargeting = false;									// ��׼״̬��ʼ��
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// �ٶȳ�ʼ��

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
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);						// ǰ��
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);							// ����
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::AddControllerYawInput);					// Yaw �������
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);				// Pitch �������

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterCharacter::OnStartFire);				// �����������
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShooterCharacter::OnEndFire);				// �ſ����ͣ��

	PlayerInputComponent->BindAction(TEXT("Target"), IE_Pressed, this, &AShooterCharacter::OnStartTargeting);		// �����Ҽ���׼
	PlayerInputComponent->BindAction(TEXT("Target"), IE_Released, this, &AShooterCharacter::OnEndTargeting);		// �ſ��Ҽ���׼

	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Pressed, this, &AShooterCharacter::OnStartSpeedUp);			// ���¼���
	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Released, this, &AShooterCharacter::OnEndSpeedUp);			// �ſ���ԭ


	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::OnReload);					// ����
	PlayerInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, this, &AShooterCharacter::OnNextWeapon);			// ��ǹ
}

// ���������ʼ��
void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//FActorSpawnParameters SpawnInfo;				// FActorSpawnParameters ��һ�����ݸ� SpawnActor �����Ŀ�ѡ�����Ľṹ������ָ������ʱ�ĸ������
	//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;				// AlwaysSpawn: Actor���������λ�����ɣ�������ײ���
	//CurrentWeapon = GetWorld()->SpawnActor<AShooterWeapon>(ShooterWeaponClass, SpawnInfo);					// ��GetWorld����SpawnActor��������Actor��ģ��ָ���䴴�����ͣ�SpawnActor�Ĳ���Ϊָ������ʹ����ķ�ʽ
	//if (CurrentWeapon)
	//{
	//	CurrentWeapon->SetPawnOwner(this);			// �����������ʱ��Pawn
	//	//CurrentWeapon->AttachMeshToPawn();		// ���� Mesh �� Pawn
	//	CurrentWeapon->OnEquip(nullptr);
	//}

	SpawnDefaultInventory();
}

// ����������ĽǶȱ任 ȥ ͬ�� Mesh �ĽǶȱ任
void AShooterCharacter::OnCanmeraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	// SkeletalMeshComponent���ڴ�������SkeletalMesh�ʲ���ʵ����
	USkeletalMeshComponent* DefMesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("PawnMesh1P")));

	// ��һ�˳�Mesh����ڸ�����ľ��󣬲���ֵ��һ�˳��ֱ��ھֲ��ռ��λ����Ϣ
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->GetRelativeRotation(), DefMesh1P->GetRelativeLocation());

	// ��������������ռ�ľ���
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	// ����� Yaw �� Pitch
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);
	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);

	// �������ڸ�����ģ�������ת�仯��Inverse ȡ��
	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();

	// Mesh ������������
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();

	// �����������ڸ������������ת�仯���ó� Mesh1P ���������������ת�任
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;

	// ����� Mesh1P ����ڸ�����ľ���
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

	// ��������ڸ������λ���Լ���ת��GetOrigin �᷵�ؾ����0��
	Mesh1P->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
}


// ǰ���ƶ�
void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		/*
		 * �����鿴�ƶ������Ƿ񱻺��ԡ����û��Pawn����UpdatedComponent������true���������Pawn��IsMoveInputIgnored()ʵ��
		 * �Ƿ�������
		 */
		//bool bLimitRotation = (GetCharacterMovement()->IsMoveInputIgnored() || GetCharacterMovement()->IsFalling());
		//FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		//FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		//AddMovementInput(Direction, Value);

		FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// �����ƶ�
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

// ��ȡ��һ�˳�Mesh
USkeletalMeshComponent* AShooterCharacter::GetMesh1P()
{
	return Mesh1P;
}

// ��ȡ�����ҵ�
FName AShooterCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

// ��ȡ��׼����
FRotator AShooterCharacter::GetAimOffsets() const
{
	// ��ȡ��������
	FVector AimDirectionW = GetBaseAimRotation().Vector();
	// ��������ת����������
	FVector AimDirectionL = ActorToWorld().InverseTransformVectorNoScale(AimDirectionW);

	return AimDirectionL.Rotation();
}

// ����
void AShooterCharacter::OnStartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

// ֹͣ����
void AShooterCharacter::OnEndFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}


// ��ʼ��׼
void AShooterCharacter::OnStartTargeting()
{
	if (AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller))		// �ÿ����������ǽ�ɫ����Ai
	{
		SetIsTargeting(true);
		SetSpeed(TargetingSpeed);
	}
}

// ������׼
void AShooterCharacter::OnEndTargeting()
{
	SetIsTargeting(false);
	SetSpeed(TargetingSpeed * -1);
}

// ��ȡ��׼״̬
bool AShooterCharacter::GetIsTargeting() const
{
	return IsTargeting;
}

// ������׼״̬
void AShooterCharacter::SetIsTargeting(bool NewIsTargeting)
{
	IsTargeting = NewIsTargeting;
}

// �����ٶ�
void AShooterCharacter::SetSpeed(float UpSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed += UpSpeed;
}

// ��ʼ����
void AShooterCharacter::OnStartSpeedUp()
{
	if (!IsSpeedUp)
	{
		SetSpeed(RunSpeed);
		IsSpeedUp = true;
	}
}

// ��������
void AShooterCharacter::OnEndSpeedUp()
{
	if (IsSpeedUp)
	{
		SetSpeed(RunSpeed * -1);
		IsSpeedUp = false;
	}
}

// ȡ�˺�
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActuaDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActuaDamage > 0.0)
	{
		HP -= ActuaDamage;
	}
	return ActuaDamage;
}

// ��ȡ���ص�����
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
	// GetClass()->GetDefaultObject<AShooterCharacter>()->HP;    // ͨ�� GetDefaultObject ��ȡ��Hp�����ֵ
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
	AShooterWeapon* LocalLastWeapon = nullptr;			// �洢����һ������

	if (LastWeapon)										// ��ʼʱ LastWeapon һ���ǿյģ������Ϊ�գ���ô������������Ϊ LocalLastWeapon
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)				// ��� LastWeapon Ϊ�գ���ôҪ���������뵱ǰ�����ǲ���һ���������һ���ͻ�����滻���ɵ�ǰ��������������
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();					// ж�ؾ�����
	}

	CurrentWeapon = NewWeapon;							// ��������������ǰ����

	if (NewWeapon)
	{
		NewWeapon->SetPawnOwner(this);
		NewWeapon->OnEquip(LastWeapon);
	}
}

