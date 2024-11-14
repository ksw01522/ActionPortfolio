// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/SGameSettingWindow_Graphic.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Settings/ActionPFGameUserSettings.h"

#define LOCTEXT_NAMESPACE "ActionPFGameSetting"

SLATE_IMPLEMENT_WIDGET(SGameSettingWindow_Graphic);
void SGameSettingWindow_Graphic::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameSettingWindow_Graphic::Construct(const FArguments& InArgs)
{
	TextStyle = InArgs._TextStyle;
	FocusedFrameBrush = InArgs._FocusedFrameBrush;
	NameBlockPadding = InArgs._NameBlockPadding;
	ArrowImageBrush = InArgs._ArrowImageBrush;
	CheckedImageBrush = InArgs._CheckedImageBrush;
	UncheckedImageBrush = InArgs._UncheckedImageBrush;
	OnBackKeyDown = InArgs._OnBackKeyDown;
	DescTextStyle = InArgs._DescTextStyle;
	DescKeyLabelSize = InArgs._DescLabelSize;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1)
		[
			SAssignNew(ScrollBox, SScrollBox)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.AutoHeight()
		[
			CreateDescriptionWidget()
		]
	];

	TWeakObjectPtr<UActionPFGameUserSettings> UserSetting = UActionPFGameUserSettings::Get();

	if (UserSetting.IsValid())
	{
		//화면 모드
		FText WindowModeName = LOCTEXT("WindowMode Name", "화면 모드");
		const TArray<FText>& WindowModeOptions = UserSetting->GetWindowModeOptions();
		int SelectedIdx = UserSetting->GetCurrentWindowMode();

		CreateMultipleNode(WindowModeName, WindowModeOptions, SelectedIdx, 
			SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting] (int Idx) {
									if(!UserSetting.IsValid()) return;

									UserSetting->SetWindowMode(Idx, true);
							}));

		//화면비율
		FText ScreenResolutionName = LOCTEXT("ScreenResolution Name", "해상도");
		const TArray<FIntPoint>& ScreenResolutionOptions = UserSetting->GetScreenResolutionOptions();
		TArray<FText> SROptionsNames;
		for(int i = 0; i < ScreenResolutionOptions.Num(); i++)
		{
			FText TempName = FText::FromString(FString::Printf(TEXT("%dX%d"), ScreenResolutionOptions[i].X, ScreenResolutionOptions[i].Y));
			SROptionsNames.Add(TempName);
		}

		FIntPoint CurrentSR = UserSetting->GetScreenResolution();
		int CurrentSRIdx = 0;
		for (int i = 0; i < ScreenResolutionOptions.Num(); i++)
		{
			if (CurrentSR == ScreenResolutionOptions[i]) {
				CurrentSRIdx = i;
				break;
			}
		}

		
		CreateMultipleNode(ScreenResolutionName, SROptionsNames, CurrentSRIdx, 
								SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx){
											if(!UserSetting.IsValid()) return;
											UserSetting->SetScreenResolutionByIndex(Idx, true);
										}));

		//프레임
		FText FrameOptionName = LOCTEXT("Frame Option Name", "프레임");
		const TArray<int>& FrameOptions = UserSetting->GetFrameRateLimitOptions();
		TArray<FText> FrameOptionTexts;
		int CurrentFrame = UserSetting->GetFrameRateLimit();
		int CurrentFrameIdx = 0;
		
		for (int i = 0; i < FrameOptions.Num(); i++)
		{
			if(FrameOptions[i] == CurrentFrame) CurrentFrameIdx = i;

			FText TempName;
			if(FrameOptions[i] != 0) {TempName = FText::FromString(FString::Printf(TEXT("%d"), FrameOptions[i]));}
			else {TempName = LOCTEXT("Frame No Limit", "제한없음"); }
			FrameOptionTexts.Add(TempName);
		}

		CreateMultipleNode(FrameOptionName, FrameOptionTexts, CurrentFrameIdx,
			SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx) {
				if (!UserSetting.IsValid()) return;
				UserSetting->SetFrameRateLimitByIdx(Idx, true);;
				}));



		//VSync
		FText VSyncName = LOCTEXT("VSync Name", "수직동기화");
		bool CurrentVSync = UserSetting->IsVSyncEnabled();

		CreateToggleNode(VSyncName, CurrentVSync, SGameSettingNode_Toggle::FOnChangeState::CreateLambda([UserSetting](bool NewState) {
												if(!UserSetting.IsValid()) return;

												UserSetting->SetVSyncEnabled(NewState);
									}));

		//가시거리
		FText VDLName = LOCTEXT("ViewDistanceLevel Name", "가시거리");
		const TArray<FText>& VDLOptions = UserSetting->GetViewDistanceLevelOptions();
		int CurrentVDLIdx = UserSetting->GetViewDistanceQuality();

		CreateMultipleNode(VDLName, VDLOptions, CurrentVDLIdx, SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx){
													if (!UserSetting.IsValid()) return;
													
													UserSetting->SetViewDistanceQuality(Idx);
								}));

		//그림자
		FText SQName = LOCTEXT("ShadowQuality Name", "그림자 품질");
		const TArray<FText>& SQOptions = UserSetting->GetShadowQualityOptions();
		int CurrentSQIdx = UserSetting->GetShadowQuality();

		CreateMultipleNode(SQName, SQOptions, CurrentSQIdx, SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx) {
													if (!UserSetting.IsValid()) return;

													UserSetting->SetShadowQuality(Idx);
													UserSetting->ApplyGraphicSettings();
								}));

		//텍스쳐
		FText TQName = LOCTEXT("TextureQuality Name", "텍스쳐 품질");
		const TArray<FText>& TQOptions = UserSetting->GetTextureQualityOptions();
		int CurrentTQIdx = UserSetting->GetShadowQuality();

		CreateMultipleNode(TQName, TQOptions, CurrentTQIdx, SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx) {
													if (!UserSetting.IsValid()) return;

													UserSetting->SetTextureQuality(Idx);
													UserSetting->ApplyGraphicSettings();
								}));

		//AA
		FText AAMName = LOCTEXT("AntiAliasing Name", "안티에일리어싱");
		const TArray<FText>& AAMOptions = UserSetting->GetAntiAliasingMethodOptions();
		int CurrentAAMIdx = UserSetting->GetAntiAliasingMethod();

		CreateMultipleNode(AAMName, AAMOptions, CurrentAAMIdx, SGameSettingNode_Multiple::FOnChangeOption::CreateLambda([UserSetting](int Idx) {
			if (!UserSetting.IsValid()) return;

			UserSetting->SetAntiAliasingMethod(Idx, true);
			}));
	}
	

	RebuildNavigation();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION




#undef LOCTEXT_NAMESPACE