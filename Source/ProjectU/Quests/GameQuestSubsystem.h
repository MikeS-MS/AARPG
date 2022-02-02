// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStructures.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameQuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestDelegate, int32, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestItemDelegate, FString, ItemName, int32, ItemQty);

DECLARE_LOG_CATEGORY_EXTERN(LogQuestSystem, Log, All)

UCLASS()
class PROJECTU_API UGameQuestSubsystem final : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:

    UPROPERTY()
    TMap<int32, FQuest> m_Quests;

    UPROPERTY()
    TMap<FString, int32> m_QuestItemRequirements;
public:

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestCompleted;
    
    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnMainQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestActivated;

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestsLoaded;

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestItemDelegate  OnItemUpdated;

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    UFUNCTION()
    bool IsQuestItemsCompleted(int32 QuestID);
    
    UFUNCTION()
    void UpdateQuestItemTrackerDelegate(const FString ItemName, const int32 ItemQty);
    
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
    void LoadQuests(const TArray<FQuest> InQuests);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void CompleteQuest(const int32 QuestID);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void ActivateQuest(const int32 QuestID);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool UpdateQuestItemTracker(const FString ItemName, const int32 ItemQty);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool CompleteQuestItem(const FString ItemName);
};
