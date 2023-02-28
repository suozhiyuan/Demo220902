// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShooterHUD.h"
#include <ddraw.h>

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

	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02Ob(TEXT("/Game/UI/HUD/HUDAssets02"));
	HpUIBg		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 162, 372, 50);
	HpUI		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 212, 372, 50);
	HpIcon		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 76, 262, 28, 28);

	Offset = 20.f;
}

// 绘制HUD
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;		// 整体的缩放比例

	DrawCrosshair();
	DrawHpUI();
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
}

void AShooterHUD::DrawHpUI()
{
	Canvas->SetDrawColor(FColor::White);									// 调整颜色混合

	// 右下角剪辑区域
	const float HpUIPosX = (Canvas->ClipX - HpUIBg.UL * ScaleUI) / 2;		// 血条的横向坐标
	const float HpUIPosY = Canvas->ClipY - HpUIBg.VL * ScaleUI;				// 血条的纵向坐标
	Canvas->DrawIcon(HpUIBg, HpUIPosX, HpUIPosY, ScaleUI);

	// 血条
	AShooterCharacter* MyPawn = Cast<AShooterCharacter>(GetOwningPawn());
	const float HpAmount = FMath::Min(1.0f, (float)MyPawn->GetHp() / (float)MyPawn->GetMaxHp());			// FMath::Min  返回最低的值，让返回的值不大于1
	FCanvasTileItem TileItem(FVector2D(HpUIPosX, HpUIPosY), HpUI.Texture->Resource, FVector2D(HpUI.UL * HpAmount * ScaleUI, HpUI.VL * ScaleUI), FLinearColor::White);
	MakeUV(HpUI, TileItem.UV0, TileItem.UV1, HpUI.U, HpUI.V, HpUI.UL * HpAmount, HpUI.VL);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

	Canvas->DrawIcon(HpIcon, HpUIPosX + Offset * ScaleUI, HpUIPosY + (HpUI.VL - HpIcon.VL) * ScaleUI / 2, ScaleUI);
	//Canvas->DrawIcon(HpIcon, HpUIPosX, HpUIPosY, ScaleUI);
}

/**
 * @brief			
 * @param Icon		FCanvasIcon 的 UI资源
 * @param UV0		UV 坐标 0 (Left/Top)
 * @param UV1		UV 坐标 0 (Right/Bottom)
 * @param U			UI的起始位置横坐标
 * @param V			UI的其实位置高坐标
 * @param UL		UI的宽度
 * @param VL		UI的高度
 */
void AShooterHUD::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{
	if (Icon.Texture)
	{
		float Width = Icon.Texture->GetSurfaceWidth();
		float Height = Icon.Texture->GetSurfaceHeight();

		UV0 = FVector2D(U / Width, V / Height);
		UV1 = FVector2D((U + UL) / Width, (V + VL) / Height);
	}
}
