// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

//Ability연동을 위한 헤더
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"

#include "Components/TimelineComponent.h"

#include "ItemBase.generated.h"



/**
 * 
 */

class UGameplayEffect;

class UEquipmentAbility;
class AActionPortfolioCharacter;
struct FTimeline;

class IItemUserInterface;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0 UMETA(Hidden),
	Equipment,
	Consumption,
	Material,
};

ENUM_RANGE_BY_FIRST_AND_LAST(EItemType, EItemType::Equipment, EItemType::Material);

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	None = 0 UMETA(Hidden),
	Common,
	Uncommon,
	Rare,
	Unique,
	Legendary,
	Epic
};

ENUM_RANGE_BY_FIRST_AND_LAST(EItemGrade, EItemGrade::Common, EItemGrade::Epic);


USTRUCT(BlueprintType)
struct ACTIONPORTFOLIO_API FItemData_Base : public FTableRowBase
{
	GENERATED_BODY()

	FItemData_Base() : ItemType(EItemType::None), Grade(EItemGrade::Common), Name(), Description(), Price(0), StackSize(1)
	{}

	FItemData_Base(EItemType NewType) : ItemType(NewType), Name(), Description(), Price(0), StackSize(1)
	{}

	FItemData_Base(EItemType NewType, int NewStackSize) : ItemType(NewType), Name(), Description(), Price(0), StackSize(NewStackSize)
	{}

	FItemData_Base(EItemType NewType, int NewStackSize, TSubclassOf<UItemBase> NewItemClass) : ItemType(NewType), Name(), Description(), Price(0), StackSize(NewStackSize)
	{}

	virtual ~FItemData_Base() {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	EItemGrade Grade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	int Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase", meta = (ClampMin = 1, ClampMax = 255, EditCondition = "bStackable"))
	int StackSize;

	virtual TSubclassOf<class UItemBase> GetItemClass() const;
};

UCLASS(Abstract, BlueprintType)
class ACTIONPORTFOLIO_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	friend class UItemManagerSubsystem;

	UItemBase() : Icon(nullptr), ItemMesh(nullptr)
	{}
	virtual ~UItemBase(){}

private:
	FName ItemCode;

	EItemType ItemType;

	EItemGrade Grade;

	FName Name;

	FString Description;

	TObjectPtr<UTexture2D> Icon;

	TObjectPtr<UStaticMesh> ItemMesh;

	int Price;

	int StackSize;

	int Count;

protected:
	virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data, TArray<FText>& OutErroMsgs);

	void SetItemIcon(UTexture2D* NewIcon) { Icon = NewIcon; }

	void SetItemMesh(UStaticMesh* NewMesh) { ItemMesh = NewMesh; }

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FName GetItemCode() const { return ItemCode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FName GetItemName() const { return Name; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	EItemType GetItemType() const { return ItemType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	EItemGrade GetItemGrade() const {return Grade;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	virtual FString GetItemDescription() const { return Description; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	UTexture2D* GetItemIcon() const {return Icon;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	UStaticMesh* GetItemMesh() const {return ItemMesh;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	int GetPrice() const {return Price; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	int GetStackSize() const {return StackSize;}

	bool IsStackableItem() const {return 1 < StackSize;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	int GetCount() const {return Count;}

	UFUNCTION(BlueprintCallable, Category = "Item")
	void AddCount(int InCount);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void RemoveCount(int InCount);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetCount(int InCount);

	virtual bool CanStackableWithOther(const UItemBase* Target) const;
	
	virtual bool IsSame(const UItemBase* Other) const;
};


class UStaticMesh;
class UBoxComponent;

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API ADroppedItem : public AActor
{
	GENERATED_BODY()

	friend class UItemWorldSubsystem;

public:
	ADroppedItem();

private:
	UPROPERTY(VisibleAnywhere, Transient, Category = "Item")
	UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(VisibleAnywhere, Transient, Category = "Item")
	UBoxComponent* BoxCollider;

	UPROPERTY(Transient)
	UItemBase* DroppedItem;

	TSharedPtr<struct FStreamableHandle> MeshStreamingHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnBeginOverlapColliderWithOther(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetDICollisionActivate(bool NewState);

private:
	void CancelMeshStreaming();

public:
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	void SetDroppedItem(UItemBase* NewData);

	const UItemBase* GetDroppedItem() const {return DroppedItem;}

	bool IsValidDropItem() const;

private:
	void OnReadyDropItem();

public:
	void ClearDroppedItemData();

private:
	TWeakObjectPtr<APawn> MagnetizedTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> BounceCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> MangetizedCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float BouncePower;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float BounceHeight;

	FTimeline BounceTimeline;
	FTimeline MagnetizedTimeline;

	FVector BounceStartLocation, BounceGoalLocation;

	void BounceItem();

	void SetMagnetizedTarget(APawn* NewTarget);

	UFUNCTION()
	void BounceTimelineFunction(float Output);

	UFUNCTION()
	void MagnetizedEvent();

	UFUNCTION()
	void MagnetizedTimelineFunction(float Output);

	void MagnetizedCancel();

	UFUNCTION()
	void MagnetizedComplete();

public:
	int GetItemCount() const {return DroppedItem->GetCount(); }

	void SetItemCount(int NewCount);
};
