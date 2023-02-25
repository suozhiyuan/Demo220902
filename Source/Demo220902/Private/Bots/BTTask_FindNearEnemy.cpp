// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/BTTask_FindNearEnemy.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Bots/ShooterAIController.h"
#include "Player/ShooterCharacter.h"


EBTNodeResult::Type UBTTask_FindNearEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AShooterAIController* MyController = Cast<AShooterAIController>(OwnerComp.GetAIOwner()); 	// �õ�AI������
	if (MyController)
	{
		return EBTNodeResult::Failed;
	}
	APawn* MyBot = MyController->GetPawn();
	AShooterCharacter* Enemy = MyController->GetEnemy();
	if(MyBot && Enemy)
	{
		// ������Ѱ���������ϣ���������Ŀ��·���ϵ�һ�㣬����������ʼ�㣬�������600����λ��һ��
		const FVector SearchOrigin = Enemy->GetActorLocation() + 600.0f * (MyBot->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		const float SearchRadius = 200.0f;		// ��Ѱ�뾶

		// ����Ŀ�ĵ�
		const FVector Loc = UNavigationSystemV1::GetRandomReachablePointInRadius(MyController, SearchOrigin, SearchRadius);
		if (Loc != FVector::ZeroVector)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
