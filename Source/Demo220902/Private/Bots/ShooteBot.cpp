// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/ShooteBot.h"

#include "Bots/ShooterAIController.h"

AShooteBot::AShooteBot()
{
	AIControllerClass = AShooterAIController::StaticClass();
}