// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Demo220902GameMode.generated.h"

class AShooterAIController;
/**
 * 
 */
UCLASS()
class DEMO220902_API ADemo220902GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADemo220902GameMode();

	// 游戏开始运行 GameMode 时调用的
	void StartPlay() override;

	AShooterAIController* CreateBotController();

};
