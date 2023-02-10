// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShooterHUD.h"

#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"

AShooterHUD::AShooterHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));

	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 43, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 88, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Up] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 371, 9, 25);
	Crosshair[EShooterCrosshairDirection::Down] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 415, 9, 25);
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 73, 403, 7, 7);

	AimArr = 5.0f;		// 默认瞄准像素精度
}

// 绘制HUD
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;		// 整体的缩放比例

	DrawCrosshair();
}

// 绘制十字准星
void AShooterHUD::DrawCrosshair()
{
	AShooterPlayerController* PCOwner = Cast<AShooterPlayerController>(PlayerOwner);
	if (PCOwner)
	{
		float CenterX = Canvas->ClipX / 2;			// X中心点
		float CenterY = Canvas->ClipY / 2;			// Y中心点

		//Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Center],
		//	CenterX - Crosshair[EShooterCrosshairDirection::Center].UL * ScaleUI / 2,
		//	CenterY - Crosshair[EShooterCrosshairDirection::Center].VL * ScaleUI / 2, ScaleUI);

		Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Left],
			CenterX - Crosshair[EShooterCrosshairDirection::Center].UL * ScaleUI / 2 - AimArr * ScaleUI - Crosshair[EShooterCrosshairDirection::Left].UL * ScaleUI,
			CenterY - Crosshair[EShooterCrosshairDirection::Left].VL * ScaleUI / 2, ScaleUI);

		Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Right],
			CenterX + Crosshair[EShooterCrosshairDirection::Center].UL * ScaleUI / 2 + AimArr * ScaleUI,
			CenterY - Crosshair[EShooterCrosshairDirection::Right].VL * ScaleUI / 2, ScaleUI);

		Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Up],
			CenterX - Crosshair[EShooterCrosshairDirection::Up].UL * ScaleUI / 2,
			CenterY - Crosshair[EShooterCrosshairDirection::Center].UL * ScaleUI / 2 - AimArr * ScaleUI - Crosshair[EShooterCrosshairDirection::Up].VL * ScaleUI, ScaleUI);

		Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Down],
			CenterX - Crosshair[EShooterCrosshairDirection::Down].UL * ScaleUI / 2,
			CenterY + Crosshair[EShooterCrosshairDirection::Center].UL * ScaleUI / 2 + AimArr * ScaleUI, ScaleUI);
	
}
