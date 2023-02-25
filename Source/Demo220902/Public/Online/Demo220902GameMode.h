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

	//
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// 创建机器人控制器
	AShooterAIController* CreateBotController();

	// 开始创建机器人，利用AGameModeBase::RestartPlayer (…)接口
	void StartBots();

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	TSubclassOf<APawn> BotPawnClass;

};
