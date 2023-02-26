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
	 *	������Ϸ��Ŀ�е�Ĭ�����ã�Ĭ��Pawn��HUD��Controller��GameState��PlayerState��
	 */

	/*
	 *���� Pawn����ȡ�� AShooterCharater ���͵� Pawn, ���ҽ���ȡ���� Pawn��ͨ���������ɵĶ��� ������Ŀ�����е� DefaultPawnClass*/ 
	static ConstructorHelpers::FClassFinder<AShooterCharacter> PlayerPawnObject(TEXT("/Game/Blueprints/Pawns/PlayerPawn"));
	DefaultPawnClass = PlayerPawnObject.Class;		// ��ֵ PlayerPawn �ķ�����Ϣ����
	
	static ConstructorHelpers::FClassFinder<AShooterCharacter> BotPawnObject(TEXT("/Game/Blueprints/Pawns/BotPawn"));
	BotPawnClass = BotPawnObject.Class;

	/*
	 * ���� Controller�����ÿ������� GameMode	 */
	PlayerControllerClass =	AShooterPlayerController::StaticClass();

	/*
	 * ���� PlayerState �� GameMode	 */
	PlayerStateClass = AShooterPlayerState::StaticClass();

	/*
	 *���� SpectatorPawn �� GameMode	 */
	SpectatorClass = AShooterSpectatorPawn::StaticClass();

	/*
	 * ���� GameState �� GameMode	 */
	GameStateClass = AShooterGameState::StaticClass();

	/*
	 * ���� HUD �� GameMode	 */
	HUDClass = AShooterHUD::StaticClass();
}

void ADemo220902GameMode::StartPlay()
{
	Super::StartPlay();
	CreateBotController();				// ���������˿�����
	StartBots();
}

// ��ȡ Pawn ���Ĭ�� Controller��û��⣩
UClass* ADemo220902GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController->IsA<AShooterAIController>())		// �жϵ�ǰ�Ƿ�Ϊ AI ������
	{
		return BotPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// ���������˿�����
AShooterAIController* ADemo220902GameMode::CreateBotController()
{
	// ���ݸ�SpawnActor�����Ŀ�ѡ�����ṹ��
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Instigator = nullptr;
	/*
	 *	msuucttacuispowiraialeleis ���������ײʱ�� Spawn ���͡�δ������ζ��û�и��ǣ�ʹ�ò����ߵ����á���ֵ��Զ���ᱻʹ��
	 *	ESpawnActorCollisionHandlingMethod ������õĲ��ԣ����ڴ����Դ�͸������ײ�ķ�ʽ���ɲ����ߵ������
	 *	AlwaysSpawn Actor����������λ�����ɣ������Ƿ�����ײ��
	*/
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ����Actor��ULevel����Actor���ⲿ�����ΪNULL����ʹ��Outer of Owner�����OwnerΪNULL����ʹ�� persistent level����̫��⣩
	SpawnInfo.OverrideLevel = nullptr;

	AShooterAIController* AIC =  GetWorld()->SpawnActor<AShooterAIController>(SpawnInfo);

	return AIC;
}

// ��ʼ���������ˣ�����AGameModeBase::RestartPlayer (��)�ӿ�
void ADemo220902GameMode::StartBots()
{
	UWorld* World = GetWorld();

	// �Գ��������еĿ��������б��������Ϊ AShooterAIController �򴴽�һ�� Pawn
	for(FConstControllerIterator It = World->GetControllerIterator(); It; It++)
	{
		AShooterAIController* AIC = Cast<AShooterAIController>(*It);
		if (AIC)
		{
			// RestartPlayer ������FindPlayerStart���ص�λ��������ҵ�Pawn
			RestartPlayer(AIC);
		}
	}
}
