// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterGameState : public AGameState
{
	GENERATED_BODY()

public:
	AShooterGameState();

	// ʣ��ʱ�䣬Transient ��ʱ
	UPROPERTY(Transient)
	int32 RemainingTime;
	
};
