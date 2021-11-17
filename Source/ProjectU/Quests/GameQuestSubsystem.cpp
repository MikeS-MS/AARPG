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
	//UE_LOG(LogQuestSystem, Warning, TEXT("Returned all quests"))
	//TArray<FQuest> AllQuests;
	//for(const auto& Quest : Quests)
	//{
	//	AllQuests.Push(Quest.Value);
	//}

	//return AllQuests;
        return Quests.Values();
}

FQuest UGameQuestSubsystem::GetQuest(const int32 QuestID) const
{
	if(Quests.Find(QuestID))
	{
		return Quests[QuestID];
	}
	else return FQuest();
}

void UGameQuestSubsystem::LoadQuests(const TArray<FQuest> InQuests)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Loaded %d quests"), InQuests.Num())
	for(const auto& Quest: InQuests)
		Quests[Quest.QuestID] = Quest;
	
	OnQuestsLoaded.Broadcast(0);
}

void UGameQuestSubsystem::CompleteQuest(const int32 QuestID)
{
	if(Quests.Find(QuestID))
	{
		Quests[QuestID].bIsCompleted = true;
		OnQuestCompleted.Broadcast(QuestID);
		UE_LOG(LogQuestSystem, Warning, TEXT("Completed quest, ID:%d"), QuestID)
	}
	UE_LOG(LogQuestSystem, Error, TEXT("Invalid quest ID:%d, couldn't complete the quest."), QuestID)
}

void UGameQuestSubsystem::ActivateQuest(const int32 QuestID)
{
	if(Quests.Find(QuestID))
	{
		Quests[QuestID].bIsActive = true;
		OnQuestActivated.Broadcast(QuestID);
		UE_LOG(LogQuestSystem, Warning, TEXT("Activated quest, ID:%d"), QuestID)
	}
	UE_LOG(LogQuestSystem, Error, TEXT("Invalid quest ID:%d, couldn't activate the quest."), QuestID)
}
