// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerCameraManager.h"

AShooterPlayerController::AShooterPlayerController()
{
	// ͨ��ÿ�����Դ��� StaticClass ��������������������һ�������
	PlayerCameraManagerClass = AShooterPlayerCameraManager::StaticClass();
}
