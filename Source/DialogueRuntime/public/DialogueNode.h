// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Engine/CompositeDataTable.h"
#include "DialogueNode.generated.h"

/**
 * 
 */



class UDialogueEvent;
class APlayerController;
class UDialogueSession;
class UDialogueEdge;
class UDataTable;
class USRichTextBlockDecorator;
class URichTextBlockDecorator;

struct FDialogueHandle;
struct FDialogueElement;

enum class EDialogueLanguage : uint8;

UENUM(BlueprintType)
enum class ECustomNodeLimit : uint8
{
	Unlimited,
	Limited
};

UENUM(BlueprintType)
enum class EDialogueNodeType : uint8
{
	None = 0,
	Start			UMETA(DisplayName = "Start"),
	Basic			UMETA(DisplayName = "Basic"),
	Question		UMETA(DisplayName = "Question"),
	Answer			UMETA(DisplayName = "Answer"),
	End				UMETA(DislplayName = "End")
};

struct FDialogueElement
{
	FDialogueElement() : Name(""), DialogueString(""), DialogueStyleSet(nullptr)
	{
		DialogueSlateDecorators.Empty();
		DialogueUMGDecorators.Empty();
	}

	FDialogueElement(FString Name, FString DialogueString, UDataTable* StyleSet, TArray<TSubclassOf<USRichTextBlockDecorator>> SlateDeco, TArray<TSubclassOf<URichTextBlockDecorator>> UMGDeco)
						: Name(Name), DialogueString(DialogueString), DialogueSlateDecorators(SlateDeco), DialogueUMGDecorators(UMGDeco)
	{
		DialogueSlateDecorators.Empty();
		DialogueUMGDecorators.Empty();
	}


public:
	FString Name;

	FString DialogueString;

	UDataTable* DialogueStyleSet;

	TArray<TSubclassOf<USRichTextBlockDecorator>> DialogueSlateDecorators;

	TArray<TSubclassOf<URichTextBlockDecorator>> DialogueUMGDecorators;

};

struct FDialogueElementContainer
{
	FDialogueElementContainer(){}
	~FDialogueElementContainer(){}
public:
	EDialogueNodeType ContainerType;

	TArray<FDialogueElement> Elements;

	void Clear();
};

UENUM(BlueprintType)
enum class ERichTextBlockType : uint8
{
	UMG,
	SLATE
};

USTRUCT(BlueprintType)
struct FAnimInDialogueStruct
{
	GENERATED_BODY()

	FAnimInDialogueStruct() : DialoguerID(""), MontageToPlay(nullptr), AnimPlayLate(1)
	{

	}
public:
	UPROPERTY(EditAnywhere)
	FString DialoguerID;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere)
	float AnimPlayLate;
};

USTRUCT(Atomic)
struct FDialogueLocalization : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDialogueLocalization() : Original(), Korean(), English()
	{}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Original;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Korean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString English;

public:
	void DialogueClear() {
		Original = "";
		Korean = "";
		English = "";
	}

	FString GetStringByLanguage(EDialogueLanguage Language) const;
};

struct FNextDialogueNodeOptionalStruct
{
public:
	FNextDialogueNodeOptionalStruct(){}
	virtual ~FNextDialogueNodeOptionalStruct(){}
};


UCLASS(Abstract, BlueprintType)
class DIALOGUERUNTIME_API UDialogueNode : public UObject
{
	GENERATED_BODY()

public:
	UDialogueNode();
	virtual ~UDialogueNode() {}

public:
	UPROPERTY()
	UDialogueSession* DialogueSession;

	UPROPERTY()
	TArray<UDialogueNode*> ParentNodes;

	UPROPERTY()
	TArray<UDialogueNode*> ChildrenNodes;

	UPROPERTY()
	TMap<UDialogueNode*, UDialogueEdge*> Edges;

	UPROPERTY()
	EDialogueNodeType DialogueNodeType;

private:
	UPROPERTY(EditAnywhere, Category = "DialogueEvents", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<UDialogueEvent*> EnterEvents;


public:
	virtual const UDialogueEdge* GetEdge(const UDialogueNode* ChildNode) const;

	bool IsLeafNode() const;

	UDialogueSession* GetDialogueSession() const;

	EDialogueNodeType GetDialogueNodeType() const { return DialogueNodeType; }

	virtual void GetDialogueElementContainer(FDialogueElementContainer& OutElement) const;

	virtual const UDialogueNode* GetNextDialogueNode(FNextDialogueNodeOptionalStruct* OptionalStruct = nullptr) const;

	TArray<UDialogueEvent*> GetDialogueEvents() const {return EnterEvents;}

#if WITH_EDITOR
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DialogueNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;


	virtual void OnChangedDialogueTextStyle() {}

#endif

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY()
	FText ContextMenuName;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor")
	ECustomNodeLimit ParentLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor", meta = (ClampMin = "0", EditCondition = "ParentLimitType == ECustomNodeLimit::Limited", EditConditionHides))
	int32 ParentLimit;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor")
	ECustomNodeLimit ChildrenLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor", meta = (ClampMin = "0", EditCondition = "ChildrenLimitType == ECustomNodeLimit::Limited", EditConditionHides))
	int32 ChildrenLimit;


#endif

#if WITH_EDITOR

	virtual bool IsNameEditable() const;

	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle();

	virtual void SetNodeTitle(const FText& NewTitle) {};

	virtual bool CanCreateConnection(UDialogueNode* Other, FText& ErrorMessage);

	virtual bool CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
	virtual bool CanCreateConnectionFrom(UDialogueNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);

	virtual void RebuildNode() {};

public:
	virtual void OnChanged_PreviewLanguage() {};
#endif
};
