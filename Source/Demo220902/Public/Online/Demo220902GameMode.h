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

	// Ԥ��ʼ�����
	virtual void PreInitializeComponents() override;

	// ��״̬ת��Ϊ InProgress ʱ����
	virtual void HandleMatchHasStarted() override;

	// ��ȡ Pawn ���Ĭ�� Controller
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// ���������˿�����
	AShooterAIController* CreateBotController();

	// ��ʼ���������ˣ�����AGameModeBase::RestartPlayer (��)�ӿ�
	void StartBots();

	// ���� Pawn ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	TSubclassOf<APawn> BotPawnClass;

	// Ĭ�ϼ�ʱ��
	void DefaultTimer();
	

protected:
	// ���������־�����ͬί�еļ�ʱ����Ψһ�����
	FTimerHandle TimerHandle_DefaultTimer;

	// ÿ��ʱ��
	UPROPERTY(Config)	// (������淴��˵���������Խ��ǿ����õġ���ǰֵ���Ա��浽���������ini�ļ��У�DefaultGame.ini�������ڴ���ʱ���ء�������Ĭ�������и���ֵ����ͬ BlueprintReadOnly
	int32 RoundTime;
	
};
