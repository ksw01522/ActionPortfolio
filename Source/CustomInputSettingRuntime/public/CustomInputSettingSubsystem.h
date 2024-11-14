// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Framework/Application/NavigationConfig.h"
#include "Engine/DataTable.h"
#include "CustomInputDeveloperSettings.h"
#include "Events/ChangeInputDevice.h"
#include "CustomInputSettingSubsystem.generated.h"

/**
 * 
 */
class UPlayerMappableInputConfig;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangedMappingDelegate, const FName&, const FKey&);
 
enum class EInputDeviceType : uint8;

USTRUCT(BlueprintType)
struct FKeySettingMetaData : public FTableRowBase
{
	GENERATED_BODY();

	FKeySettingMetaData() {};
	virtual ~FKeySettingMetaData() {};

	UPROPERTY(EditAnywhere)
	FKey Key;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;
};

class FCustomNavigationConfig : public FNavigationConfig
{
public:
	FCustomNavigationConfig();
	virtual ~FCustomNavigationConfig() {}

public:
	TMap<FKey, EUINavigationAction> KeyActionEventRules;

protected:
	virtual EUINavigation GetNavigationDirectionFromKey(const FKeyEvent& InKeyEvent) const override;
	virtual EUINavigationAction GetNavigationActionFromKey(const FKeyEvent& InKeyEvent) const override;
	//virtual EUINavigation GetNavigationDirectionFromAnalog(const FAnalogInputEvent& InAnalogEvent) override;
};

UCLASS(Config = Game)
class CUSTOMINPUTSETTINGRUNTIME_API UCustomInputSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UCustomInputSettingSubsystem();

private:
	static UCustomInputSettingSubsystem* SubsystemInstance;
public:
	static UCustomInputSettingSubsystem* GetInstance() { return SubsystemInstance; };

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	TWeakObjectPtr< APlayerController> Controller;

////////////////////// 키 설정 ///////////////////
private:
	TSharedPtr<class UChangeMappingInputProcessor> ChangeMappingInputProcessor;

	UPROPERTY(GlobalConfig)
	TMap<FName, FKey> CustomMappings;

	TSet<FKey> IgnoreMappingKeys;

private:
	//커스텀 매핑 시도 : 실질적인 커스텀 매핑 시도, 키를 입력받아 바꿈
	bool TryCustomMapping(const FName& Code, const FKey& NewKey, bool bIsGamepad, FText& ErrorMsg);

public:
	//커스텀 매핑 요청 : 커스텀 매핑을 위한 Slate 보여주기
	void RequestMappingKey(const FName& Code, bool bGamepad, const FText& DisplayName);

	//커스텀 매핑된 키들 얻기
	TMap<FName, FKey> GetCustomMappingKeys() const;

	//커스텀 매핑에 영향을 받을 컨트롤러 등록
	void RegisterAffectedController(APlayerController* NewController);


//For Changed Mapping Event
private:

	FOnChangedMappingDelegate OnChangedMappingDel;

private:
	void OnChangedMappingEvent(const FName& Code, const FKey& NewKey);

public:
	FDelegateHandle AddOnChangedMappingEvent(const TDelegate<void (const FName&, const FKey&)>& InDel);
	void RemoveOnChangedMappingEvent(FDelegateHandle& Handle);

//Reset
private:
	TSharedPtr<class UWaitInputForResetMappingInputProcessor> ResetProcessor;

	void ResetCustomMapping(UPlayerMappableInputConfig* InInputConfig);

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnResetMappingDelegate, class UPlayerMappableInputConfig*);
private:
	FOnResetMappingDelegate OnResetDelegate;

public:
	FDelegateHandle AddResetMappingEvent(TDelegate<void (UPlayerMappableInputConfig*)> InDel);
	void RemoveResetMappingEvent(FDelegateHandle& InHandle);

	void RequestResetMapping(UPlayerMappableInputConfig* InInputConfig);

//Keys MetaData 관련
private:
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UTexture2D>> KeyIcons;

public:
	UTexture2D* GetKeyIcon(const FKey& TargetKey) const;


////////////////// 네비게이션
private:
	TSharedPtr<FCustomNavigationConfig> CustomNavConfig;

	void RebuildNavigationRules();

	TMap<FName, FKeySettingContainer> OptionalNavKeyActions;

public:
	void GetNavigationDirectionKey(EUINavigation InDirection, FKey& OutKeyboard, FKey& OutGamepad) const;

	void GetNavigationActionKey(EUINavigationAction InAction, FKey& OutKeyboard, FKey& OutGamepad) const;

	void GetNavigationOptionalActionKey(const FName& InActionName, FKey& OutKeyboard, FKey& OutGamepad) const;

	

//Check Current Input Device;
private:
	TSharedPtr<class UCheckInputDeviceProcessor> CheckInputDeviceProcessor;

public:
	EInputDeviceType GetCurrentInputDevice() const;

	//입력장치 변화에 대한 이벤트 추가
	FDelegateHandle AddInputDeviceChangeEvent(const TDelegate<void(EInputDeviceType)>& InDelegate);
	
	//입력장치 변화에 대한 이벤트 제거
	void RemoveInputDeviceChangeEvent(FDelegateHandle& InHandle);
};


