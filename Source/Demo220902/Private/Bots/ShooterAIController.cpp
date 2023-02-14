// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooterAIController.h"

#include "Demo220902.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bots/ShooterBot.h"
#include "Player/ShooterCharacter.h"


AShooterAIController::AShooterAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	EnemyKeyID = 0;
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AShooterBot* Bot = Cast<AShooterBot>(InPawn);
	if (Bot && Bot->BotBehavior)								// BotBehavior 行为树
	{
		if (Bot->BotBehavior->BlackboardAsset)					// BlackboardAsset 黑板数据
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);			// 用于使用给定数据的设置黑板，如果指定的数据正确初始化，则返回true
		}
		EnemyKeyID = BlackboardComp->GetKeyID(("Enemy"));
		BehaviorComp->StartTree(*Bot->BotBehavior);
	}
}

void AShooterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

// 找到可见的，并且是最近的敌人  （LOS：Line of Sight 视线）
void AShooterAIController::FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy)
{
	if (APawn* MyBot = GetPawn())
	{
		float BestDistSq = MAX_FLT;								// 记录遍历到的最近距离
		const FVector MyLoc = MyBot->GetActorLocation();		// 获取当前角色位置
		AShooterCharacter* BestPawn = nullptr;					// 保存距离最近的目标

		/*
		 * 模仿迭代器类，当TArray<TWeakObjectPtr<APawn>>::Iterator 被定义时，试图提供FConstPawnIterator之前所做的基本接口，
		 * 一般来说，你不应该使用这个迭代器，而应该使用TActorIterator<APawn>,或TActorRange<APawn> (或所需的派生类型)。在未来的版本中，这个迭代器可能会被弃用。
		 */
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
		{
			AShooterCharacter* TestPawn = Cast<AShooterCharacter>(*It);			// 遍历获取一个目标
			if (TestPawn && TestPawn != ExcludeEnemy)
			{
				if (HasLOSToEnemy(TestPawn, true))						// 查找视线内的敌人
				{
					const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();		// SizeSquared 向量长度的平方
					if (DistSq < BestDistSq)
					{
						BestDistSq = DistSq;
						BestPawn = TestPawn;
					}
				}
			}
		}
		// 记录到黑板数据
		if (BestPawn)
		{
			// to do
		}
	}
}

// 视线方向的敌人是否可见 （LOS：Line of Sight 视线）
bool AShooterAIController::HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const
{
	bool bHasLOS = false;
	static FName LosTag = FName(TEXT("AIWeaponLosTrace"));

	/**
	 *  碰撞函数的查询参数
	 *  @param  InTraceTag          射线碰撞查询名称
	 *  @param  InTraceComplex      是否进行精确复杂的碰撞查询
	 *  @param  InignoreActor		忽略的目标
	 */
	FCollisionQueryParams TraceParams(LosTag, true, GetPawn());
	TraceParams.bReturnPhysicalMaterial = true;							// 是否只返回带有物理材质的目标
	//TraceParams.bTraceAsyncScene = true;								// 进行碰撞计算时，如果切换场景的时候，后台异步加载时避免出现错误？？？，这个参数已经没有了

	AShooterBot* MyBot = Cast<AShooterBot>(GetPawn());
	FVector StartLocation = MyBot->GetActorLocation();
	StartLocation.Z += MyBot->BaseEyeHeight;							// 获取机器人眼睛位置
	const FVector EndLocation = InEnemyActor->GetActorLocation();		// 目标位置

	FHitResult Hit(ForceInit);											// 结构体中包含碰撞到的信息，例如撞击点和该点的表面法线。

	/**
	 *  从机器人位置，发射特定的通道跟踪光线，并返回第一个碰撞命中
	 *  @param  OutHit					第一个碰撞的信息
	 *  @param  Start					发射起始点
	 *  @param  End						发射结束点
	 *  @param  TraceChannel			射线所在的通道
	 *  @param  Params					用于跟踪的其他碰撞查询参数
	 *  @param 	ResponseParam			ResponseContainer 用于此跟踪，默认参数 FCollisionResponseParams::DefaultResponseParam
	 *  @return 如果命中返回True
	 */
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, COLLISION_WEAPON, TraceParams);

	if (Hit.bBlockingHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor == InEnemyActor)
			{
				bHasLOS = true;
			}
		}
	}
	return bHasLOS;
}