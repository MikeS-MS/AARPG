// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStructures.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="ID of the quest."))
	int32 QuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="If the quest is a bottom-level a master ID is expected, otherwise set to -1."))
	int32 MasterQuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Title of the quest."))
	FText QuestName; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Normal description of the quest."))
	FText QuestDescription; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Short description of the quest meant to be used on screen/HUD"))
	FText QuestDescriptionHUD; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="All the IDs of the bottom-level quests that this top-level quest rules over."))
	TArray<int32> SubQuestsIDs; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Whether this is a top-level quest or a subquest of a top-level one."))
	bool bIsMasterQuest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Whether this is a side quest or a main story quest."))
	bool bIsSideQuest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Is the quest currently active or not."))
	bool bIsActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta=(ToolTip="Whether the quest is completed or not."))
	bool bIsCompleted;
};