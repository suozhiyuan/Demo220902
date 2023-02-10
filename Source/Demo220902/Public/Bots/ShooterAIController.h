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

	//virtual void Possess(APawn* InPawn) override;		// Possess 是旧名字
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;;

	// 找到可见的，并且是最近的敌人  （LOS：Line of Sight 视线）
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy);

	// 视线方向的敌人是否可见 （LOS：Line of Sight 视线）
	bool HasLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const;

private:
	UBlackboardComponent* BlackboardComp;		// 黑板数据
	//UBehaviorTree* BehaviorComp;				// 行为树
	UBehaviorTreeComponent* BehaviorComp;		// 行为树

	int32 EnemyKeyID;							// 敌人ID
	
};
