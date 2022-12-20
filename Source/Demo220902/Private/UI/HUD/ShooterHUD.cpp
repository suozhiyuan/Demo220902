// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShooterHUD.h"

AShooterHUD::AShooterHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/UI/HUD/HUDMain"));

	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 43, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 88, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Up] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 371, 9, 25);
	Crosshair[EShooterCrosshairDirection::Down] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 415, 9, 25);
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 73, 403, 7, 7);
}

// ����HUD
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;
	DrawCrosshair();

}

// ����ʮ��׼��
void AShooterHUD::DrawCrosshair()
{
	float CenterX = Canvas->ClipX / 2;			// X���ĵ�
	float CenterY = Canvas->ClipY / 2;			// Y���ĵ�
	Canvas->DrawIcon(Crosshair[EShooterCrosshairDirection::Center], 
					 CenterX - Crosshair[EShooterCrosshairDirection::Center].UL/2,
					CenterY - Crosshair[EShooterCrosshairDirection::Center].VL/2, ScaleUI);
}
