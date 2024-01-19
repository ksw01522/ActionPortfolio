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


ENUM_RANGE_BY_FIRST_AND_LAST(EItemType, EItemType::Equipment, EItemType::Material);

USTRUCT(BlueprintType)
struct ACTIONPORTFOLIO_API FItemData_Base : public FTableRowBase
{
	GENERATED_BODY()

	FItemData_Base() : ItemType(EItemType::None), Grade(EItemGrade::Common), ItemClass(), Name(), Description(), Icon(nullptr), Price(0), StackSize(1)
	{}

	FItemData_Base(EItemType NewType) : ItemType(NewType), Name(), Description(), Icon(nullptr), Price(0), StackSize(1)
	{}

	FItemData_Base(EItemType NewType, int NewStackSize) : ItemType(NewType), Name(), Description(), Icon(nullptr), Price(0), StackSize(NewStackSize)
	{}

	FItemData_Base(EItemType NewType, int NewStackSize, TSubclassOf<UItemBase> NewItemClass) : ItemType(NewType), ItemClass(NewItemClass), Name(), Description(), Icon(nullptr), Price(0), StackSize(NewStackSize)
	{}

	virtual ~FItemData_Base() {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	EItemGrade Grade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	TSoftObjectPtr<UMaterialInterface> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	int Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase")
	bool bStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemBase", meta = (ClampMin = 0, ClampMax = 255, EditCondition = "bStackable"))
	int StackSize;
};

UCLASS(Abstract, BlueprintType)
class ACTIONPORTFOLIO_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase() : bInitialized(false)
	{}
	virtual ~UItemBase(){}

	

private:
	FName ItemCode;

	EItemType ItemType;

	EItemGrade Grade;

	FName Name;

	FString Description;

	TSoftObjectPtr<UMaterialInterface> Icon;

	TSoftObjectPtr<UStaticMesh> ItemMesh;

	int Price;

	bool bStackable;

	int StackSize;

protected:
	bool bInitialized;

public:
	virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data);

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
	TSoftObjectPtr<UMaterialInterface> GetIconMaterial() const {return Icon;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	TSoftObjectPtr<UStaticMesh> GetItemMesh() const {return ItemMesh;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	int GetPrice() const {return Price; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	int GetStackSize() const {return StackSize;}

	bool IsStackableItem() const {return bStackable;}
	virtual bool CanStackableWithOther(const UItemBase* Target) const;
	
	virtual bool IsSame(const UItemBase* Other) const;
};

UENUM(BlueprintType)
enum class EEquipmentPart : uint8
{
	None = 0 UMETA(Hidden),
	Body,
	Pants,
	Head,
	Arm,
	Foot
};

UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Equipment : public UItemBase
{
	GENERATED_BODY()

public:
	UItemBase_Equipment()
	{};

private:
	TArray<TSubclassOf<UEquipmentAbility>> EquipmentAbilities;

	EEquipmentPart EquipmentPart;

	float MaxHP;

	float MaxStamina;

	float AttackP;

	float DefenseP;

protected:
	const class IItemUserInterface* OnEquippedUser;
	TArray<FGameplayAbilitySpecHandle> EquipAbilitySpecHandle;
	FActiveGameplayEffectHandle EquipEffectHandle;

protected:
	virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data) override;

public:
	virtual bool CanEquipItem(const IItemUserInterface* ItemUser) const;
	bool TryEquipItem(const IItemUserInterface* ItemUser);

protected:
	virtual UGameplayEffect* MakeAddStatusEffect() const;
	virtual void OnEquipItem(const IItemUserInterface* ItemUser);

public:
	virtual bool CanUnequipItem(const IItemUserInterface* ItemUser) const;
	bool TryUnequipItem(const IItemUserInterface* ItemUser);
protected:
	virtual void OnUnequipItem();

public:
	EEquipmentPart GetEquipmentPart() const {return EquipmentPart;}

	virtual bool IsSame(const UItemBase* Other) const override;

};

USTRUCT(BlueprintType)
struct FItemData_Equipment : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Equipment() : FItemData_Base(EItemType::Equipment, 1, UItemBase_Equipment::StaticClass()), MaxHP(0), MaxStamina(0), AttackP(0), DefenseP(0)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<UEquipmentAbility>> EquipmentAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EEquipmentPart EquipmentPart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float AttackP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float DefenseP;
};



UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Consumption : public UItemBase
{
	GENERATED_BODY()


protected:
	//virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data) override;

};

USTRUCT(BlueprintType)
struct FItemData_Consumption : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Consumption() : FItemData_Base(EItemType::Consumption, 99, UItemBase_Consumption::StaticClass())
	{}

};



UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Material : public UItemBase
{
	GENERATED_BODY()

protected:
	//virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data) override;

};

USTRUCT(BlueprintType)
struct FItemData_Material : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Material() : FItemData_Base(EItemType::Material, 99, UItemBase_Material::StaticClass())
	{}
};

class UStaticMesh;
class UBoxComponent;

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API ADroppedItem : public AActor
{
	GENERATED_BODY()

	friend class UDropItemPoolWorldSubsystem;

public:
	ADroppedItem();

private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	UBoxComponent* BoxCollider;

	UPROPERTY(Transient)
	UItemBase* DroppedItem;

	int ItemCount;

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

	void SetDroppedItem(UItemBase* NewData, int NewCount);
	void SetItemCount(int Count);
	int GetItemCount() const {return ItemCount;}

	const UItemBase* GetDroppedItem() const {return DroppedItem;}

	bool IsValidDropItem() const;

private:
	void OnReadyDropItem();

	void ClearDroppedItemData();

	void OnPickUpItem();

private:
	TWeakObjectPtr<APawn> MagnetizedTarget;

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
	

};
