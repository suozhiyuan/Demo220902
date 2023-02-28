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

	// ����Ѫ��
	void DrawHpUI();
	
	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);

	// ����ƥ�䶨ʱ����λ��
	void DrawMatchTimerAndPosition();


	UPROPERTY()
	FCanvasIcon Crosshair[5];

	// UI���ű���ֵ
	float ScaleUI;

	// Ĭ����׼���ؾ���
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float AimArr;

	// Ѫ������
	UPROPERTY()
	FCanvasIcon HpUIBg;

	// Ѫ��
	UPROPERTY()
	FCanvasIcon HpUI;

	// Ѫ����
	UPROPERTY()
	FCanvasIcon HpIcon;

	// �̶�ƫ��
	float Offset;

	//
	FCanvasIcon TimerBg;

	FCanvasIcon TimerIcon;

};