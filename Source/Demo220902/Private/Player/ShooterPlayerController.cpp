// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerCameraManager.h"

AShooterPlayerController::AShooterPlayerController()
{
	// 通过每个类自带的 StaticClass 返回类名，给到控制器一个摄像机
	PlayerCameraManagerClass = AShooterPlayerCameraManager::StaticClass();
}
