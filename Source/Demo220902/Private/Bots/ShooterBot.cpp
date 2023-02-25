// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooterBot.h"
#include "Bots/ShooterAIController.h"
#include "Weapon/ShooterWeapon.h"

AShooterBot::AShooterBot()
{
	AIControllerClass = AShooterAIController::StaticClass();

	WalkSpeed = 450.f;								// 行走速度
	TargetingSpeed = 0;								// 开镜时速度惩罚
}


