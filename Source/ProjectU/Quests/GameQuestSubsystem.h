// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameQuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestDelegate, int32, QuestID);

DECLARE_LOG_CATEGORY_EXTERN(LogQuestSystem, Log, All)
/**
 * 
 */
UCLASS()
class PROJECTU_API UGameQuestSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestActivated;
};
