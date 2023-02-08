// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class AShooterCharacter;
/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	// 找到可见的，并且是最近的敌人  （LOS：Line of Sight 视线）
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy);

	// 视线方向是否有敌人 （LOS：Line of Sight 视线）
	bool HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const;




	
};
