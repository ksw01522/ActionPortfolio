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
	TObjectPtr<UAnimMontage> MontageToPlay;

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
	TObjectPtr<UDialogueSession> DialogueSession;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> ParentNodes;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> ChildrenNodes;

	UPROPERTY()
	TMap<TObjectPtr<UDialogueNode>, TObjectPtr<UDialogueEdge>> Edges;

	UPROPERTY()
	EDialogueNodeType DialogueNodeType;

private:
	UPROPERTY(EditAnywhere, Category = "DialogueEvents", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UDialogueEvent>> EnterEvents;


public:
	virtual const UDialogueEdge* GetEdge(const UDialogueNode* ChildNode) const;

	bool IsLeafNode() const;

	UDialogueSession* GetDialogueSession() const;

	EDialogueNodeType GetDialogueNodeType() const { return DialogueNodeType; }

	virtual TArray<const UDialogueNode*> GetNextDialogueNodes() const;

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

	virtual bool CanCreateConnection(UDialogueNode* Other, FText& ErrorMessage) const;

	virtual bool CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) const;
	virtual bool CanCreateConnectionFrom(UDialogueNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage) const;

	virtual void RebuildNode() {};

public:
	virtual void OnChanged_PreviewLanguage() {};
#endif
};
