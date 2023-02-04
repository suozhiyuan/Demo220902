// Copyright Epic Games, Inc. All Rights Reserved.


#include "Online/Demo220902GameMode.h"
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
	DefaultPawnClass = PlayerPawnObject.Class;

	/*
	 * 配置 Controller，配置控制器到 GameMode
	 */
	PlayerControllerClass =	AShooterPlayerController::StaticClass();

	/*
	 * 配置 PlayerState 到 GameMode
	 */
	PlayerStateClass = AShooterPlayerState::StaticClass();

	/*
	 *配置 SpectatorPawn 到 GameMode
	 */
	SpectatorClass = AShooterSpectatorPawn::StaticClass();

	/*
	 * 配置 GameState 到 GameMode
	 */
	GameStateClass = AShooterGameState::StaticClass();

	/*
	 * 配置 HUD 到 GameMode
	 */
	HUDClass = AShooterHUD::StaticClass();
}

void ADemo220902GameMode::StartPlay()
{
	Super::StartPlay();
}

AShooterAIController* ADemo220902GameMode::CreateBotController()
{

}
