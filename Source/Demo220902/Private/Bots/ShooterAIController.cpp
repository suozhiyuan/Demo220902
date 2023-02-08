// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooterAIController.h"
#include "Player/ShooterCharacter.h"


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
			
		}

	}
}

// 视线方向是否有敌人 （LOS：Line of Sight 视线）
bool AShooterAIController::HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const
{
	// LineTraceSingleByChannel 从机器人位置，发射特定的通道跟踪光线，并返回第一个阻塞命中
	//GetWorld()->LineTraceSingleByChannel();
	return true;
}
