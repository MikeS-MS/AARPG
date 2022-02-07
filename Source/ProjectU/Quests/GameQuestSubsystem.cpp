// Fill out your copyright notice in the Description page of Project Settings.


#include "GameQuestSubsystem.h"

DEFINE_LOG_CATEGORY(LogQuestSystem)

void UGameQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UGameQuestSubsystem::IsQuestItemsCompleted(int32 QuestID)
{
	const auto Quest = m_Quests.Find(QuestID);
	
	bool bItemRequirementsMet = true;
	for(const auto& ItemRequirement : Quest->QuestItemRequirements)
	{
		int* Result = m_QuestItemRequirements.Find(ItemRequirement.Key);
		if(!Result && *Result != 0)
		{
			bItemRequirementsMet = false;
			UE_LOG(LogQuestSystem, Warning, TEXT("Item requirement not met: \nRequirements: Item:%s, Qty:%d\nCurrent qty:%d"), *ItemRequirement.Key, ItemRequirement.Value, Result);
		} else
		{
			UE_LOG(LogQuestSystem, Warning, TEXT("Item requirements  met: \nRequirements: Item:%s, Qty:%d"), *ItemRequirement.Key, ItemRequirement.Value);
		}
	}
	return bItemRequirementsMet;
}

TArray<int32> UGameQuestSubsystem::GetActiveQuestsIDs() const
{
	TArray<int32> ActiveQuestIDs;
	for(const auto& Quest : m_Quests)
	{
		if (!Quest.Value.bIsCompleted && Quest.Value.bIsActive)
			ActiveQuestIDs.Push(Quest.Key);
	}

	return ActiveQuestIDs;
}

TArray<FQuest> UGameQuestSubsystem::GetActiveQuests() const
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all active quests"))
	TArray<FQuest> ActiveQuests;
	for(const auto& Quest : m_Quests)
	{
		if (Quest.Value.bIsActive && !Quest.Value.bIsCompleted)
		{
			ActiveQuests.Push(Quest.Value);
		}
	}

	return ActiveQuests;
}

TArray<FQuest> UGameQuestSubsystem::GetAllQuests() const
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all quests"))
	TArray<FQuest> AllQuests;
	m_Quests.GenerateValueArray(AllQuests);
	return AllQuests;
}

FQuest UGameQuestSubsystem::GetQuest(const int32 QuestID) const
{
	if(m_Quests.Find(QuestID))
	{
		return m_Quests[QuestID];
	}
	return FQuest();
}

void UGameQuestSubsystem::LoadQuests(const TArray<FQuest> InQuests, bool ActivateQuests)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Loaded %d quests"), InQuests.Num())
	for(const auto& Quest: InQuests)
	{
		m_Quests.Add(Quest.QuestID, Quest);
		
		if (ActivateQuests)
			ActivateQuest(Quest.QuestID);
	}
	
	OnQuestsLoaded.Broadcast(0);
}

bool UGameQuestSubsystem::CompleteQuest(const int32 QuestID)
{
	auto& CurrentQuest = m_Quests[QuestID];
	const bool bIsValid = m_Quests.Contains(QuestID);

	if(!CurrentQuest.bIsActive)
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("Quest is not active to be completed: QuestID %d"), CurrentQuest.QuestID);
		return false;
	}
	
	if(!IsQuestItemsCompleted(QuestID))
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("The item requirements are not met for the quest to be completed: QuestID %d"), CurrentQuest.QuestID);
		return false;
	}
	
	if(bIsValid)
	{
		const int32 MasterQuestID = CurrentQuest.MasterQuestID;
		CurrentQuest.bIsCompleted = true;
		UE_LOG(LogQuestSystem, Warning, TEXT("Completed quest, ID:%d"), QuestID)
		// Check if this is the last sub-quest and mark the master quest as completed if true
		bool bAllQuestsAreCompleted = true;
		if(CurrentQuest.bIsSideQuest && m_Quests.Find(MasterQuestID))
		{
			for(const auto& Quest : GetActiveQuests())
			{
				if(Quest.MasterQuestID == MasterQuestID)
				{
					bAllQuestsAreCompleted &= Quest.bIsCompleted;
					UE_LOG(LogQuestSystem, Warning, TEXT("Found quest that is completed: %b with ID: %d"), bAllQuestsAreCompleted, QuestID)
				}
			}
			
			if(bAllQuestsAreCompleted)
			{
				m_Quests[MasterQuestID].bIsCompleted = true;
				UE_LOG(LogQuestSystem, Warning, TEXT("Completed master quest, ID:%d"), QuestID)
				OnMainQuestCompleted.Broadcast(MasterQuestID);
			} else
			{
				return false;
			}
		}
		OnQuestCompleted.Broadcast(MasterQuestID);
		OnQuestCompleted.Broadcast(QuestID);
		return true;
	}
	
	UE_LOG(LogQuestSystem, Error, TEXT("Invalid quest ID:%d, couldn't complete the quest."), QuestID)
	return false;
}

bool UGameQuestSubsystem::ActivateQuest(const int32 QuestID)
{
	const auto Quest = m_Quests.Find(QuestID);
	if(Quest)
	{
		Quest->bIsActive = true;
		
		const auto ItemRequirements = Quest->QuestItemRequirements;
		// Add the quest item requirements to be tracked
		for(const auto& Requirement : ItemRequirements)
		{
			auto& ReqEntry = m_QuestItemRequirements.FindOrAdd(Requirement.Key);
			ReqEntry += Requirement.Value;
			UE_LOG(LogQuestSystem, Warning, TEXT("Added requirements: %s:%d"), *Requirement.Key, Requirement.Value);
		}

		// Broadcast a delegate with the activated quest id
		OnQuestActivated.Broadcast(QuestID);
		UE_LOG(LogQuestSystem, Warning, TEXT("Activated quest, ID:%d"), QuestID)
		return true;
	}
	
	UE_LOG(LogQuestSystem, Error, TEXT("Invalid quest ID:%d, couldn't activate the quest."), QuestID)
	return false;
}

// Update the quest item tracker
bool UGameQuestSubsystem::UpdateQuestItemTracker(const FString ItemName, const int32 ItemQty)
{
	const auto itemReq = m_QuestItemRequirements.Find(ItemName);
	if(itemReq)
	{
		*itemReq = FMath::Max(*itemReq - ItemQty, 0);
		UE_LOG(LogQuestSystem, Warning, TEXT("Updated quest item: %s, new qty: %d"), *ItemName, *itemReq)
		OnItemUpdated.Broadcast(ItemName, *itemReq);
		return *itemReq == 0;
	}
	return false;
}
void UGameQuestSubsystem::UpdateQuestItemTrackerDelegate(const FString ItemName, const int32 ItemQty)
{
	UpdateQuestItemTracker(ItemName, ItemQty);
}

// For internal use
bool UGameQuestSubsystem::CompleteQuestItem(const FString ItemName)
{
	const auto itemReq = m_QuestItemRequirements.Find(ItemName);
	if(itemReq)
	{
		*itemReq = 0;
		OnItemUpdated.Broadcast(ItemName, 0);
		return true;
	}
	return false;
}
