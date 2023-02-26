// Copyright Epic Games, Inc. All Rights Reserved.


#include "Online/Demo220902GameMode.h"

#include "Bots/ShooterAIController.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "Player/ShooterSpectatorPawn.h"
#include "Online/ShooterGameState.h"
#include "UI/HUD/ShooterHUD.h"


ADemo220902GameMode::ADemo220902GameMode()
{
	/*
	 *	配置游戏项目中的默认设置（默认Pawn、HUD、Controller、GameState、PlayerState）
	 */

	/*
	 *配置 Pawn，获取到 AShooterCharater 类型的 Pawn, 并且将获取到的 Pawn（通过反射生成的对象） 给到项目设置中的 DefaultPawnClass*/ 
	static ConstructorHelpers::FClassFinder<AShooterCharacter> PlayerPawnObject(TEXT("/Game/Blueprints/Pawns/PlayerPawn"));
	DefaultPawnClass = PlayerPawnObject.Class;		// 赋值 PlayerPawn 的反射信息对象
	
	static ConstructorHelpers::FClassFinder<AShooterCharacter> BotPawnObject(TEXT("/Game/Blueprints/Pawns/BotPawn"));
	BotPawnClass = BotPawnObject.Class;

	/*
	 * 配置 Controller，配置控制器到 GameMode	 */
	PlayerControllerClass =	AShooterPlayerController::StaticClass();

	/*
	 * 配置 PlayerState 到 GameMode	 */
	PlayerStateClass = AShooterPlayerState::StaticClass();

	/*
	 *配置 SpectatorPawn 到 GameMode	 */
	SpectatorClass = AShooterSpectatorPawn::StaticClass();

	/*
	 * 配置 GameState 到 GameMode	 */
	GameStateClass = AShooterGameState::StaticClass();

	/*
	 * 配置 HUD 到 GameMode	 */
	HUDClass = AShooterHUD::StaticClass();
}

void ADemo220902GameMode::StartPlay()
{
	Super::StartPlay();
	CreateBotController();				// 创建机器人控制器
	StartBots();
}

// 获取 Pawn 类的默认 Controller（没理解）
UClass* ADemo220902GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController->IsA<AShooterAIController>())		// 判断当前是否为 AI 控制器
	{
		return BotPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// 创建机器人控制器
AShooterAIController* ADemo220902GameMode::CreateBotController()
{
	// 传递给SpawnActor函数的可选参数结构体
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Instigator = nullptr;
	/*
	 *	msuucttacuispowiraialeleis 方法解决碰撞时的 Spawn 类型。未定义意味着没有覆盖，使用参与者的设置。赋值永远不会被使用
	 *	ESpawnActorCollisionHandlingMethod 定义可用的策略，用于处理以穿透阻塞碰撞的方式生成参与者的情况。
	 *	AlwaysSpawn Actor将在期望的位置生成，无论是否发生碰撞。
	*/
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 生成Actor的ULevel，即Actor的外部。如果为NULL，则使用Outer of Owner。如果Owner为NULL，则使用 persistent level（不太理解）
	SpawnInfo.OverrideLevel = nullptr;

	AShooterAIController* AIC =  GetWorld()->SpawnActor<AShooterAIController>(SpawnInfo);

	return AIC;
}

// 开始创建机器人，利用AGameModeBase::RestartPlayer (…)接口
void ADemo220902GameMode::StartBots()
{
	UWorld* World = GetWorld();

	// 对场景中所有的控制器进行遍历，如果为 AShooterAIController 则创建一个 Pawn
	for(FConstControllerIterator It = World->GetControllerIterator(); It; It++)
	{
		AShooterAIController* AIC = Cast<AShooterAIController>(*It);
		if (AIC)
		{
			// RestartPlayer 尝试在FindPlayerStart返回的位置生成玩家的Pawn
			RestartPlayer(AIC);
		}
	}
}
