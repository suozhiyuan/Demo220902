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
	 *	������Ϸ��Ŀ�е�Ĭ�����ã�Ĭ��Pawn��HUD��Controller��GameState��PlayerState��
	 */

	/*
	 *���� Pawn����ȡ�� AShooterCharater ���͵� Pawn, ���ҽ���ȡ���� Pawn��ͨ���������ɵĶ��� ������Ŀ�����е� DefaultPawnClass*/ 
	static ConstructorHelpers::FClassFinder<AShooterCharacter> PlayerPawnObject(TEXT("/Game/Blueprints/Pawns/PlayerPawn"));
	DefaultPawnClass = PlayerPawnObject.Class;

	/*
	 * ���� Controller�����ÿ������� GameMode
	 */
	PlayerControllerClass =	AShooterPlayerController::StaticClass();

	/*
	 * ���� PlayerState �� GameMode
	 */
	PlayerStateClass = AShooterPlayerState::StaticClass();

	/*
	 *���� SpectatorPawn �� GameMode
	 */
	SpectatorClass = AShooterSpectatorPawn::StaticClass();

	/*
	 * ���� GameState �� GameMode
	 */
	GameStateClass = AShooterGameState::StaticClass();

	/*
	 * ���� HUD �� GameMode
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
