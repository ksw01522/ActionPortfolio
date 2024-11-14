// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ActionPFAbilitySystemComponent.generated.h"

/**
 * 
 */
class UActionPFGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FGenericEventInputDelegate, int32);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnNewOrRemovedStateIcon, const FGameplayTag&, TSubclassOf<class UStateIconWidget>, const FSlateBrush&, bool);

USTRUCT()
struct FStateIconStruct
{
	GENERATED_BODY()
	
	FStateIconStruct()
	{
	
	}

	FStateIconStruct(const FGameplayTag& NewTag, TSubclassOf<class UStateIconWidget> NewClass, const FSlateBrush& NewBrush)
		: StateTag(NewTag), WidgetClass(NewClass), ImageBrush(NewBrush)
	{}

	FGameplayTag StateTag;

	TSubclassOf<class UStateIconWidget> WidgetClass;

	FSlateBrush ImageBrush;
};

UCLASS()
class ACTIONPORTFOLIO_API UActionPFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UActionPFAbilitySystemComponent();

private:
	TMap<int32, FGenericEventInputDelegate> GenericEventInputDelegate;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsGenericEventInputBound(int32 InputID) const;
	
	virtual void OnTagUpdated(const FGameplayTag& Tag, bool TagExists) override;
	
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

public:
	void ClearAbilityWithClass(TSubclassOf<class UGameplayAbility> InAbilityClass);

	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities);
	bool IsActingAbilityByClass(TSubclassOf<UGameplayAbility> CheckAbility);

	static UActionPFAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	bool GetCooldownRemainingAndDurationByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) const;

	bool CanActivateAbility(TSubclassOf<UActionPFGameplayAbility> AbilityClass);


	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;

	void AbilityLocalInputPressedByClass(const TSubclassOf<UActionPFGameplayAbility>& InClass);
	void AbilityLocalInputReleassedByClass(const TSubclassOf<UActionPFGameplayAbility>& InClass);

	FGenericEventInputDelegate& GetGenericEventInputDelegate(int32 InputID);

private:
	UPROPERTY()
	TArray<FStateIconStruct> StateIconArray;

	FOnNewOrRemovedStateIcon OnNewOrRemovedStateIcon;


public:
	void AddStateIcon(const FGameplayTag& InTag, TSubclassOf<UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush);
	void RemoveStateIcon(const FGameplayTag& InTag);

	FOnNewOrRemovedStateIcon& GetOnNewOrRemovedStateIconDelegate() {return OnNewOrRemovedStateIcon; }

	TArray<FStateIconStruct> GetStateIconArray() const { return StateIconArray; }
};
