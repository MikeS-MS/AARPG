// Fill out your copyright notice in the Description page of Project Settings.


#include "GameQuestSubsystem.h"

DEFINE_LOG_CATEGORY(LogQuestSystem)

TArray<int32> UGameQuestSubsystem::GetActiveQuestsIDs() const
{
	TArray<int32> ActiveQuestIDs;
	for(const auto& Quest : Quests)
	{
		if (!Quest.Value.bIsCompleted)
			ActiveQuestIDs.Push(Quest.Key);
	}

	return ActiveQuestIDs;
}

TArray<FQuest> UGameQuestSubsystem::GetActiveQuests() const
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all active quests"))
	TArray<FQuest> ActiveQuests;
	for(const auto& Quest : Quests)
	{
		if (!Quest.Value.bIsCompleted)
			ActiveQuests.Push(Quest.Value);
	}

	return ActiveQuests;
}

TArray<FQuest> UGameQuestSubsystem::GetAllQuests() const
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all quests"))
	TArray<FQuest> AllQuests;
	for(const auto& Quest : Quests)
	{
		AllQuests.Push(Quest.Value);
	}

	return AllQuests;
}

FQuest UGameQuestSubsystem::GetQuest(int32 QuestID) const
{
	return Quests[QuestID];
}

void UGameQuestSubsystem::LoadQuests(TArray<FQuest> InQuests)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Loaded quests"))
	for(const auto& Quest: InQuests)
		Quests[Quest.QuestID] = Quest;
	
	OnQuestsLoaded.Broadcast(0);
}

void UGameQuestSubsystem::CompleteQuest(int32 QuestID)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Completed quest, ID:%d"), QuestID)
	OnQuestCompleted.Broadcast(QuestID);
}

void UGameQuestSubsystem::ActivateQuest(int32 QuestID)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Activated quest, ID:%d"), QuestID)
	OnQuestActivated.Broadcast(QuestID);
}
