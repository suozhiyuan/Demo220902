// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooterAIController.h"
#include "Player/ShooterCharacter.h"


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
			
		}

	}
}

// ���߷����Ƿ��е��� ��LOS��Line of Sight ���ߣ�
bool AShooterAIController::HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const
{
	// LineTraceSingleByChannel �ӻ�����λ�ã������ض���ͨ�����ٹ��ߣ������ص�һ����������
	//GetWorld()->LineTraceSingleByChannel();
	return true;
}
