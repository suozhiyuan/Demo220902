// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UBehaviorTreeComponent;
class AShooterCharacter;
/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AShooterAIController();

	//virtual void Possess(APawn* InPawn) override;		// Possess �Ǿ�����
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;;

	// �ҵ��ɼ��ģ�����������ĵ���  ��LOS��Line of Sight ���ߣ�
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy);

	// ���߷���ĵ����Ƿ�ɼ� ��LOS��Line of Sight ���ߣ�
	bool HasLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const;

	// AI�������
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void ShootEnemy();

	// ��ȡ����
	AShooterCharacter* GetEnemy();

	// ƥ�����
	void SetEnemy(APawn* InPawn);
	

private:
	UBlackboardComponent* BlackboardComp;		// �ڰ�����
	UBehaviorTreeComponent* BehaviorComp;		// ��Ϊ��
	int32 EnemyKeyID;							// ����ID
	
};
