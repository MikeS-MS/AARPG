// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStructures.h"
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

private:
    
    UPROPERTY()
    TMap<int32, FQuest> Quests;
public:
    
    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestActivated;
    
    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestsLoaded;
public:
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<int32> GetActiveQuestsIDs() const;

    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<FQuest> GetActiveQuests() const;

    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<FQuest> GetAllQuests() const;
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    FQuest GetQuest(int32 QuestID) const;
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void LoadQuests(TArray<FQuest> InQuests);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void CompleteQuest(int32 QuestID);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void ActivateQuest(int32 QuestID);
};
