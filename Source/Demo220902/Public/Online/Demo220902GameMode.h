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

	// 预初始化组件
	virtual void PreInitializeComponents() override;

	// 当状态转换为 InProgress 时调用
	virtual void HandleMatchHasStarted() override;

	// 获取 Pawn 类的默认 Controller
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// 创建机器人控制器
	AShooterAIController* CreateBotController();

	// 开始创建机器人，利用AGameModeBase::RestartPlayer (…)接口
	void StartBots();

	// 电脑 Pawn 类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	TSubclassOf<APawn> BotPawnClass;

	// 默认计时器
	void DefaultTimer();
	

protected:
	// 可用于区分具有相同委托的计时器的唯一句柄。
	FTimerHandle TimerHandle_DefaultTimer;

	// 每局时间
	UPROPERTY(Config)	// (虚幻引擎反射说明符，属性将是可配置的。当前值可以保存到与类关联的ini文件中（DefaultGame.ini），并在创建时加载。不能在默认属性中给定值。如同 BlueprintReadOnly
	int32 RoundTime;
	
};
