// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShooterHUD.h"
#include <ddraw.h>

#include "Online/ShooterGameState.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Weapon/ShooterWeapon.h"

AShooterHUD::AShooterHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));
	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 43, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 88, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Up] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 371, 9, 25);
	Crosshair[EShooterCrosshairDirection::Down] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 74, 415, 9, 25);
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTextureOb.Object, 73, 403, 7, 7);
	AimArr = 5.0f;													// 默认瞄准像素精度

	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02Ob(TEXT("/Game/UI/HUD/HUDAssets02"));
	HpUIBg			= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 162, 372, 50);
	HpUI			= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 212, 372, 50);
	HpIcon			= UCanvas::MakeIcon(HUDAssets02Ob.Object, 76, 262, 28, 28);

	TimerPlaceBg			= UCanvas::MakeIcon(HUDMainTextureOb.Object, 262, 16, 255, 62);
	TimerIcon		= UCanvas::MakeIcon(HUDMainTextureOb.Object, 381, 93, 24, 24);
	Offset = 20.f;
	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	BigFont = BigFontOb.Object;
	ShadowedFont.bEnableShadow = true;								// 是否开启阴影
	HUDDark = FColor(110, 124, 131, 255);

	WeaponBg_1		= UCanvas::MakeIcon(HUDMainTextureOb.Object, 543, 17, 441, 81);
	WeaponIcon_1	= UCanvas::MakeIcon(HUDMainTextureOb.Object, 282, 389, 147, 67);
	AmmoClipIcon_1	= UCanvas::MakeIcon(HUDMainTextureOb.Object, 148, 151, 62, 51);
}

// 绘制HUD
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;		// 整体的缩放比例

	DrawCrosshair();
	DrawHpUI();
	DrawMatchTimerAndPosition();
	DrawWeaponHUD();
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

// 绘制血条
void AShooterHUD::DrawHpUI()
{
	Canvas->SetDrawColor(FColor::White);									// 调整颜色混合

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

// 绘制定时器
void AShooterHUD::DrawMatchTimerAndPosition()
{
	AShooterGameState* const MyGameState =  GetWorld()->GetGameState<AShooterGameState>();

	if (MyGameState && MyGameState->RemainingTime)
	{
		const float TimerPosX = Canvas->ClipX - (TimerPlaceBg.UL + Offset) * ScaleUI;
		const float TimerPosY = Canvas->OrgY + Offset * ScaleUI;

		Canvas->DrawIcon(TimerPlaceBg, TimerPosX, TimerPosY, ScaleUI);
		Canvas->DrawIcon(TimerIcon, TimerPosX + Offset * ScaleUI, TimerPosY + (TimerPlaceBg.VL - TimerIcon.VL) * ScaleUI / 2, ScaleUI);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
		TextItem.EnableShadow(FLinearColor::Black);									// 设置阴影偏移量和颜色。

		float SizeX, SizeY;
		float TextScale = 0.57f;												// 文本规格
		TextItem.FontRenderInfo = ShadowedFont;										// 自定义字体渲染信息。开启阴影
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);		// 文字的尺度
		FString Text = GetTimeString(MyGameState->RemainingTime);					// 获取剩余时间字符串
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);								// 计算字符串的长度，考虑到文本换行。

		TextItem.SetColor(HUDDark);
		TextItem.Text = FText::FromString(Text);
		TextItem.Position = FVector2D(TimerPosX + Offset * 1.5 * ScaleUI + TimerIcon.UL * ScaleUI, TimerPosY + (TimerPlaceBg.VL - SizeY * TextScale) * ScaleUI / 2);
		Canvas->DrawItem(TextItem);
	}
}

void AShooterHUD::DrawWeaponHUD()
{
	// 拿到武器对象
	AShooterCharacter* MyPawn = Cast<AShooterCharacter>(GetOwningPawn());
	AShooterWeapon* MyWeapon = MyPawn->GetCurrentWeapon();

	if (MyPawn && MyWeapon)
	{
		const float PriWeaponOffsetY = 65;
		const float PriWeaponBoxWidth = 150;
		const float PrimaryClipIconOffset = 25;
		const float AmmoIconsCount = 4.0f;

		const float PrimaryWeaponBgPosY = Canvas->ClipY - Canvas->OrgY - (Offset + PriWeaponOffsetY + WeaponBg_1.VL) * ScaleUI;						// 主武器背景 Y 方向位置
		const float PriWeaponPosX = Canvas->ClipX - Canvas->OrgX - (2 * Offset + (WeaponIcon_1.UL + PriWeaponBoxWidth) / 2) * ScaleUI;				// 主武器图标 X 位置
		const float PriWeaponPosY = Canvas->ClipY - Canvas->OrgY - (Offset + PriWeaponOffsetY + (WeaponBg_1.VL + WeaponIcon_1.VL) / 2) * ScaleUI;	// 主武器图标 Y 位置

		const float ClipWidth = AmmoClipIcon_1.UL + PrimaryClipIconOffset * (AmmoIconsCount - 1);
		const float BoxWidth = 65.0f;
		const float PriClipPosX = PriWeaponPosX - (BoxWidth + ClipWidth) * ScaleUI;																	// 弹夹图标 X 位置
		const float PriClipPosY = Canvas->ClipY - Canvas->OrgY - (Offset + PriWeaponOffsetY + (WeaponBg_1.VL + AmmoClipIcon_1.VL) / 2) * ScaleUI;	// 弹夹图标 Y 位置

		// 绘制三角部分
		const float LeftCornerWidth = 60.0f;
		FCanvasTileItem TiltItem(FVector2D(PriClipPosX - Offset * ScaleUI, PrimaryWeaponBgPosY), WeaponBg_1.Texture->Resource, 
			FVector2D(LeftCornerWidth * ScaleUI, WeaponBg_1.VL * ScaleUI), FLinearColor::White);
		MakeUV(WeaponBg_1, TiltItem.UV0, TiltItem.UV1, WeaponBg_1.U, WeaponBg_1.V, LeftCornerWidth, WeaponBg_1.VL);
		TiltItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TiltItem);

		// 绘制背景矩形部分
		const float RestWidth = Canvas->ClipX - PriClipPosX - (LeftCornerWidth - Offset - 2 * Offset) * ScaleUI;
		TiltItem.Position = FVector2D(PriClipPosX + (LeftCornerWidth - Offset) * ScaleUI, PrimaryWeaponBgPosY);
		TiltItem.Size = FVector2D(RestWidth, WeaponBg_1.VL * ScaleUI);
		MakeUV(WeaponBg_1, TiltItem.UV0, TiltItem.UV1, WeaponBg_1.U + (WeaponBg_1.UL - RestWidth / ScaleUI), WeaponBg_1.V, RestWidth / ScaleUI, WeaponBg_1.VL);
		Canvas->DrawItem(TiltItem);

		// 绘制武器图标部分
		Canvas->DrawIcon(WeaponIcon_1, PriWeaponPosX, PriWeaponPosY, ScaleUI);

		// 绘制当前剩余子弹数量
		float SizeX, SizeY;
		FString Text = FString::FromInt(MyWeapon->GetCurrentAmmoClip());
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
		TextItem.EnableShadow(FLinearColor::Black);
		const float TextOffset = 12;
		const float TopTextScale = 0.73f;
		const float TopTextPosX = Canvas->ClipX - Canvas->OrgX - (2 * Offset + PriWeaponBoxWidth + (BoxWidth + SizeX * TopTextScale) / 2) * ScaleUI;
		const float TopTextPosY = Canvas->ClipY - Canvas->OrgY - (Offset + PriWeaponOffsetY + WeaponBg_1.VL - TextOffset / 2.0f) * ScaleUI;
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TopTextScale * ScaleUI, TopTextScale * ScaleUI);
		TextItem.FontRenderInfo = ShadowedFont;
		Canvas->DrawItem(TextItem, TopTextPosX, TopTextPosY);

		// 绘制剩余子弹文字
		float TopTextHeight;
		TopTextHeight = SizeY * TopTextScale;
		Text = FString::FromInt(MyWeapon->GetCurrentAmmo() - MyWeapon->GetCurrentAmmoClip());
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		const float BottomTextScale = 0.49f;
		const float BottomTextPosX = Canvas->ClipX - Canvas->OrgX - (PriWeaponBoxWidth + Offset * 2 + (BoxWidth + SizeX * BottomTextScale) / 2.0f) * ScaleUI;
		const float BottomTextPosY = TopTextPosY + (TopTextHeight - 0.8f * TextOffset) * ScaleUI;
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(BottomTextScale * ScaleUI, BottomTextScale * ScaleUI);
		TextItem.FontRenderInfo = ShadowedFont;
		Canvas->DrawItem(TextItem, BottomTextPosX, BottomTextPosY);

		// 绘制子弹进度条
		const float AmmoPerIcon = MyWeapon->GetAmmoPerClip() / AmmoIconsCount;
		for (int32 i = 0; i < AmmoIconsCount; i++)
		{
			if (((i+1) * AmmoPerIcon > MyWeapon->GetCurrentAmmoClip()))
			{
				const float UsedPerIcon = (i + 1) * AmmoPerIcon - MyWeapon->GetCurrentAmmoClip();
				float PercentLeftInIcon = 0;
				if (UsedPerIcon < AmmoPerIcon)
				{
					PercentLeftInIcon = (AmmoPerIcon - UsedPerIcon) / AmmoPerIcon;
				}
				const int32 Color = 128 + 128 * PercentLeftInIcon;
				Canvas->SetDrawColor(Color, Color, Color, Color);
			}
			const float ClipOffset = PrimaryClipIconOffset * i * ScaleUI;
			Canvas->DrawIcon(AmmoClipIcon_1, PriClipPosX + ClipOffset, PriClipPosY, ScaleUI);
		}
		Canvas->SetDrawColor(HUDDark);
	}
}

FString AShooterHUD::GetTimeString(float TimeSeconds)
{
	const int32 TotalSeconds = FMath::Max(0, FMath::TruncToInt(TimeSeconds) % 3600);	// 总秒数
	const int32 NumMinute = TotalSeconds / 60;		// 分
	const int32 NumSeconds = TotalSeconds % 60;		// 秒
	const FString TimeSec = FString::Printf(TEXT("%02d:%02d"), NumMinute, NumSeconds);

	return  TimeSec;
}
