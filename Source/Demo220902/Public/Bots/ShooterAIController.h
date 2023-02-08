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
	// �ҵ��ɼ��ģ�����������ĵ���  ��LOS��Line of Sight ���ߣ�
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindRecentEnemyLOS(AShooterCharacter* ExcludeEnemy);

	// ���߷����Ƿ��е��� ��LOS��Line of Sight ���ߣ�
	bool HasLOSToEnemy(AActor* InEnemyActor, bool bAnyEnemy) const;




	
};
