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

	// ��Ϸ��ʼ���� GameMode ʱ���õ�
	void StartPlay() override;

	//
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// ���������˿�����
	AShooterAIController* CreateBotController();

	// ��ʼ���������ˣ�����AGameModeBase::RestartPlayer (��)�ӿ�
	void StartBots();

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	TSubclassOf<APawn> BotPawnClass;

};
