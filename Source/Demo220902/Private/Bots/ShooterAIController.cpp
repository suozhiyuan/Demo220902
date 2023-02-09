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
	BehaviorComp = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorComp"));

	EnemyKeyID = 0;
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// to do...
}

void AShooterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	// to do...
}

// �ҵ��ɼ��ģ�����������ĵ���  ��LOS��Line of Sight ���ߣ�
void AShooterAIController::FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy)
{
	if (APawn* MyBot = GetPawn())
	{
		float BestDistSq = MAX_FLT;								// ��¼���������������
		const FVector MyLoc = MyBot->GetActorLocation();		// ��ȡ��ǰ��ɫλ��
		AShooterCharacter* BestPawn = nullptr;					// ������������Ŀ��

		/*
		 * ģ�µ������࣬��TArray<TWeakObjectPtr<APawn>>::Iterator ������ʱ����ͼ�ṩFConstPawnIterator֮ǰ�����Ļ����ӿڣ�
		 * һ����˵���㲻Ӧ��ʹ���������������Ӧ��ʹ��TActorIterator<APawn>,��TActorRange<APawn> (���������������)����δ���İ汾�У�������������ܻᱻ���á�
		 */
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
		{
			AShooterCharacter* TestPawn = Cast<AShooterCharacter>(*It);			// ������ȡһ��Ŀ��
			if (TestPawn && TestPawn != ExcludeEnemy)
			{
				if (HasLOSToEnemy(TestPawn, true))						// ���������ڵĵ���
				{
					const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();		// SizeSquared �������ȵ�ƽ��
					if (DistSq < BestDistSq)
					{
						BestDistSq = DistSq;
						BestPawn = TestPawn;
					}
				}
			}
		}
		// ��¼���ڰ�����
		if (BestPawn)
		{
			// to do
		}
	}
}

// ���߷���ĵ����Ƿ�ɼ� ��LOS��Line of Sight ���ߣ�
bool AShooterAIController::HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const
{
	bool bHasLOS = false;
	static FName LosTag = FName(TEXT("AIWeaponLosTrace"));

	/**
	 *  ��ײ�����Ĳ�ѯ����
	 *  @param  InTraceTag          ������ײ��ѯ����
	 *  @param  InTraceComplex      �Ƿ���о�ȷ����ײ��ѯ
	 *  @param  InignoreActor		���Ե�Ŀ��
	 */
	FCollisionQueryParams TraceParams(LosTag, true, GetPawn());
	TraceParams.bReturnPhysicalMaterial = true;							// �Ƿ�ֻ���ش���������ʵ�Ŀ��
	//TraceParams.bTraceAsyncScene = true;								// ������ײ����ʱ������л�������ʱ�򣬺�̨�첽����ʱ������ִ��󣿣�������������Ѿ�û����

	AShooterBot* MyBot = Cast<AShooterBot>(GetPawn());
	FVector StartLocation = MyBot->GetActorLocation();
	StartLocation.Z += MyBot->BaseEyeHeight;							// ��ȡ�������۾�λ��
	const FVector EndLocation = InEnemyActor->GetActorLocation();		// Ŀ��λ��

	FHitResult Hit(ForceInit);											// �ṹ���а�����ײ������Ϣ������ײ����͸õ�ı��淨�ߡ�

	/**
	 *  �ӻ�����λ�ã������ض���ͨ�����ٹ��ߣ������ص�һ����ײ����
	 *  @param  OutHit					��һ����ײ����Ϣ
	 *  @param  Start					������ʼ��
	 *  @param  End						���������
	 *  @param  TraceChannel			�������ڵ�ͨ��
	 *  @param  Params					���ڸ��ٵ�������ײ��ѯ����
	 *  @param 	ResponseParam			ResponseContainer ���ڴ˸��٣�Ĭ�ϲ��� FCollisionResponseParams::DefaultResponseParam
	 *  @return ������з���True
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