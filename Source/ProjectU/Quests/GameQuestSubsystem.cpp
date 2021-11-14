// Fill out your copyright notice in the Description page of Project Settings.


#include "GameQuestSubsystem.h"

TArray<FQuest> UGameQuestSubsystem::GetActiveQuests() const
{
        // TODO: @Stefan Return the actual active quests
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all active quests"))
	return Quests;
}

TArray<FQuest> UGameQuestSubsystem::GetAllQuests() const
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all quests"))
	return Quests;
}

void UGameQuestSubsystem::LoadQuests(TArray<FQuest> InQuests)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Loaded quests"))
	Quests = InQuests;
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
