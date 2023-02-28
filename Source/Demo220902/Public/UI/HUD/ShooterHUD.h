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

	// 绘制HUD
	virtual void DrawHUD() override;

	// 绘制十字准星
	void DrawCrosshair();

	// 绘制血条
	void DrawHpUI();

	// 绘制匹配定时器和位置
	void DrawMatchTimerAndPosition();

	// 绘制武器图标
	void DrawWeaponHUD();

	// 获取时间字符串
	FString GetTimeString(float TimeSeconds);

	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);

	UPROPERTY()
	FCanvasIcon Crosshair[5];

	// UI缩放比例值
	float ScaleUI;

	// 默认瞄准像素精度
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	float AimArr;

	// 血条背景
	UPROPERTY()
	FCanvasIcon HpUIBg;

	// 血条
	UPROPERTY()
	FCanvasIcon HpUI;

	// 血符号
	UPROPERTY()
	FCanvasIcon HpIcon;

	// 固定偏移
	float Offset;

	FCanvasIcon TimerBg;

	FCanvasIcon TimerIcon;

	UPROPERTY()
	UFont* BigFont;

	FFontRenderInfo ShadowedFont;

	FColor HUDDark;

	UPROPERTY()
	FCanvasIcon WeaponBg_1;

	UPROPERTY()
	FCanvasIcon WeaponIcon_1;

	UPROPERTY()
	FCanvasIcon AmmoClipIcon_1;
};