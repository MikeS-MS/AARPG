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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	int32 QuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FName QuestName; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FName QuestDescription; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FName QuestDescriptionHUD; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	TArray<int32> SubQuestsIDs; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	bool bIsMasterQuest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	bool bIsSideQuest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	bool bIsCompleted;
};