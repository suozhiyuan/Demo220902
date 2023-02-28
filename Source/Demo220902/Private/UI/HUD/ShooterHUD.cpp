// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShooterHUD.h"
#include <ddraw.h>

#include "Online/ShooterGameState.h"
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

	AimArr = 5.0f;		// Ĭ����׼���ؾ���

	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02Ob(TEXT("/Game/UI/HUD/HUDAssets02"));
	HpUIBg		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 162, 372, 50);
	HpUI		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 67, 212, 372, 50);
	HpIcon		= UCanvas::MakeIcon(HUDAssets02Ob.Object, 76, 262, 28, 28);

	TimerBg = UCanvas::MakeIcon(HUDMainTextureOb.Object, 262, 16, 255, 62);
	TimerIcon = UCanvas::MakeIcon(HUDMainTextureOb.Object, 381, 93, 24, 24);

	Offset = 20.f;

	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	BigFont = BigFontOb.Object;
	ShadowedFont.bEnableShadow = true;				// �Ƿ�����Ӱ
	HUDDark = FColor(110, 124, 131, 255);
}

// ����HUD
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;		// ��������ű���

	DrawCrosshair();
	DrawHpUI();
	DrawMatchTimerAndPosition();
}

// ����ʮ��׼��
void AShooterHUD::DrawCrosshair()
{
	AShooterPlayerController* PCOwner = Cast<AShooterPlayerController>(PlayerOwner);
	if (PCOwner)
	{
		float CenterX = Canvas->ClipX / 2;			// X���ĵ�
		float CenterY = Canvas->ClipY / 2;			// Y���ĵ�

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

// ����Ѫ��
void AShooterHUD::DrawHpUI()
{
	Canvas->SetDrawColor(FColor::White);									// ������ɫ���

	// ���½Ǽ�������
	const float HpUIPosX = (Canvas->ClipX - HpUIBg.UL * ScaleUI) / 2;		// Ѫ���ĺ�������
	const float HpUIPosY = Canvas->ClipY - HpUIBg.VL * ScaleUI;				// Ѫ������������
	Canvas->DrawIcon(HpUIBg, HpUIPosX, HpUIPosY, ScaleUI);

	// Ѫ��
	AShooterCharacter* MyPawn = Cast<AShooterCharacter>(GetOwningPawn());
	const float HpAmount = FMath::Min(1.0f, (float)MyPawn->GetHp() / (float)MyPawn->GetMaxHp());			// FMath::Min  ������͵�ֵ���÷��ص�ֵ������1
	FCanvasTileItem TileItem(FVector2D(HpUIPosX, HpUIPosY), HpUI.Texture->Resource, FVector2D(HpUI.UL * HpAmount * ScaleUI, HpUI.VL * ScaleUI), FLinearColor::White);
	MakeUV(HpUI, TileItem.UV0, TileItem.UV1, HpUI.U, HpUI.V, HpUI.UL * HpAmount, HpUI.VL);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

	Canvas->DrawIcon(HpIcon, HpUIPosX + Offset * ScaleUI, HpUIPosY + (HpUI.VL - HpIcon.VL) * ScaleUI / 2, ScaleUI);
	//Canvas->DrawIcon(HpIcon, HpUIPosX, HpUIPosY, ScaleUI);
}

/**
 * @brief			
 * @param Icon		FCanvasIcon �� UI��Դ
 * @param UV0		UV ���� 0 (Left/Top)
 * @param UV1		UV ���� 0 (Right/Bottom)
 * @param U			UI����ʼλ�ú�����
 * @param V			UI����ʵλ�ø�����
 * @param UL		UI�Ŀ��
 * @param VL		UI�ĸ߶�
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

// ���ƶ�ʱ��
void AShooterHUD::DrawMatchTimerAndPosition()
{
	const AShooterGameState* const MyGameState =  GetWorld()->GetGameState<AShooterGameState>();

	if (MyGameState && MyGameState->RemainingTime)
	{
		const float TimerPosX = Canvas->ClipX - (TimerBg.UL + Offset) * ScaleUI;
		const float TimerPosY = Canvas->OrgY + Offset * ScaleUI;

		Canvas->DrawIcon(TimerBg, TimerPosX, TimerPosY, ScaleUI);
		Canvas->DrawIcon(TimerIcon, TimerPosX + Offset * ScaleUI, TimerPosY + (TimerBg.VL - TimerIcon.VL) * ScaleUI / 2, ScaleUI);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
		TextItem.EnableShadow(FLinearColor::Black);									// ������Ӱƫ��������ɫ��

		float SizeX, SizeY;
		const float TextScale = 0.57f;
		TextItem.FontRenderInfo = ShadowedFont;										// �Զ���������Ⱦ��Ϣ��������Ӱ
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);		// ���ֵĳ߶�
		FString Text = GetTimeString(MyGameState->RemainingTime);					// ��ȡʣ��ʱ���ַ���
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		TextItem.SetColor(HUDDark);
		TextItem.Text = FText::FromString(Text);
		TextItem.Position = FVector2D(TimerPosX + Offset * 1.5 + TimerIcon.UL, TimerPosY + (TimerBg.VL - SizeY * TextScale) * ScaleUI / 2);
		Canvas->DrawItem(TextItem);
	}
}

FString AShooterHUD::GetTimeString(float TimeSeconds)
{
	const int32 TotalSeconds = FMath::Max(0, FMath::TruncToInt(TimeSeconds) % 3600);	// ������
	const int32 NumMinute = TotalSeconds / 60;		// ��
	const int32 NumSeconds = TotalSeconds % 60;		// ��
	const FString TimeSec = FString::Printf(TEXT("%02d:%02d"), NumMinute, NumSeconds);

	return  TimeSec;
}
