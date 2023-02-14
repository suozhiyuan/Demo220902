// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooterBot.h"
#include "Bots/ShooterAIController.h"
#include "Weapon/ShooterWeapon.h"

AShooterBot::AShooterBot()
{
	AIControllerClass = AShooterAIController::StaticClass();
}


