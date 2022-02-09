// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStructures.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameQuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestDelegate, int32, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestCompletedXP, int32, QuestID, int32, QuestXP);
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

    UPROPERTY()
    TArray<int32> m_ActiveQuestIDs;
public:

    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestDelegate  OnQuestCompleted;
    
    UPROPERTY(BlueprintAssignable, Category="Dispatchers")
    FQuestCompletedXP  OnQuestCompletedXP;
    
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
    bool QuestRequirementsMet(int32 QuestID);
    
    UFUNCTION()
    void RegenerateActiveQuests();
    
    UFUNCTION()
    void UpdateQuestItemTrackerDelegate(const FString ItemName, const int32 ItemQty);
    
public:
    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<int32> GetActiveQuestIDs() const;

    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<FQuest> GetActiveQuests();

    UFUNCTION(BlueprintCallable, Category="Quests")
    TArray<FQuest> GetAllQuests() const;
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    FQuest GetQuest(const int32 QuestID) const;
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    void LoadQuests(const TArray<FQuest> InQuestIDs, bool ActivateQuests);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool CompleteQuest(const int32 QuestID);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool ForceCompleteQuest(const int32 QuestID);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool ActivateQuest(const int32 QuestID, const bool bBroadcastIt = true);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool UpdateQuestItemTracker(const FString ItemName, const int32 ItemQty);
    
    UFUNCTION(BlueprintCallable, Category="Quests")
    bool CompleteQuestItem(const FString ItemName);
};
