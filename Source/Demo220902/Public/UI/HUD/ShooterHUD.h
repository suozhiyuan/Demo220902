// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

enum EShooterCrosshairDirection
{
	Left,
	Right,
	Center,
	Up,
	Down
};



/**
 * 
 */
UCLASS()
class DEMO220902_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
public:
	AShooterHUD();

	// ����HUD
	virtual void DrawHUD() override;

	// ����ʮ��׼��
	void DrawCrosshair();

	UPROPERTY()
	FCanvasIcon Crosshair[5];

	float ScaleUI;
	
};
