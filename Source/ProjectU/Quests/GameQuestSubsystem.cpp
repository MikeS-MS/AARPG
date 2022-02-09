// Fill out your copyright notice in the Description page of Project Settings.


#include "GameQuestSubsystem.h"

DEFINE_LOG_CATEGORY(LogQuestSystem)

void UGameQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UGameQuestSubsystem::QuestRequirementsMet(int32 QuestID)
{
	const auto currentQuest = m_Quests.Find(QuestID);
	bool itemsCompleted = true;
	
	if(currentQuest->QuestItemRequirements.Num() > 0)
	{
		for(const auto& Requirement : currentQuest->QuestItemRequirements)
		{
			const auto itemQty = m_QuestItemRequirements.Find(Requirement.Key);
			if(*itemQty != 0)
			{
				UE_LOG(LogQuestSystem, Error, TEXT("Not completed requirement: ITEM:%s, QTY:%d"), *Requirement.Key, *itemQty)
				itemsCompleted = false;
			}
		}
	}
	
	return itemsCompleted;
}

void UGameQuestSubsystem::RegenerateActiveQuests()
{
	m_ActiveQuestIDs.Empty();
	UE_LOG(LogQuestSystem, Warning, TEXT("Regenerating active IDs"));
	for(const auto& questItr : m_Quests)
	{
		if(questItr.Value.bIsActive && questItr.Value.bIsCompleted != false)
		{
			UE_LOG(LogQuestSystem, Warning, TEXT("Added key to active quests: %d"), questItr.Key);
			m_ActiveQuestIDs.Push(questItr.Key);
		}
	}
}

TArray<int32> UGameQuestSubsystem::GetActiveQuestIDs() const
{
	return m_ActiveQuestIDs;
}

TArray<FQuest> UGameQuestSubsystem::GetActiveQuests()
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Returned all active quests"))
	TArray<FQuest> ActiveQuests;
	for(const auto& questID : GetActiveQuestIDs())
	{
		ActiveQuests.Push(m_Quests[questID]);
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

void UGameQuestSubsystem::LoadQuests(const TArray<FQuest> InQuestIDs, bool ActivateQuests)
{
	UE_LOG(LogQuestSystem, Warning, TEXT("Loaded %d quests"), InQuestIDs.Num())
	for(const auto& Quest: InQuestIDs)
	{
		m_Quests.Add(Quest.QuestID, Quest);
		
		if (ActivateQuests)
			ActivateQuest(Quest.QuestID);
	}

	OnQuestsLoaded.Broadcast(0);
}

bool UGameQuestSubsystem::CompleteQuest(const int32 QuestID)
{
	auto& currentQuest = m_Quests[QuestID];
	const bool validQuest = m_Quests.Contains(QuestID);

	if(currentQuest.bIsCompleted)
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("Quest has been completed already: QuestID %d"), currentQuest.QuestID);
		return true;
	}
	
	if(!currentQuest.bIsActive)
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("Quest is not active to be completed: QuestID %d"), currentQuest.QuestID);
		return false;
	}
	
	// Check if quest has any requirements to be completed (collect items, etc...)
	if(!QuestRequirementsMet(QuestID))
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("The item requirements are not met for the quest to be completed: QuestID %d"), currentQuest.QuestID);
		return false;
	}

	// Check if it's a master quest and the sub-quests are completed else automatically brand it as completed
	if(validQuest && currentQuest.bIsMasterQuest)
	{
		bool allQuestsCompleted = true;
		for(const auto& quest : m_Quests)
		{
			if(quest.Value.MasterQuestID == QuestID)
				allQuestsCompleted &= quest.Value.bIsCompleted;
		}

		if(allQuestsCompleted)
		{
			currentQuest.bIsActive = false;
			currentQuest.bIsCompleted = true;
		} else
		{
			return false;
		}
	} else
	{
		currentQuest.bIsActive = false;
		currentQuest.bIsCompleted = true;

		CompleteQuest(currentQuest.MasterQuestID);
	}

	if(!currentQuest.bIsMasterQuest)
	{
		m_ActiveQuestIDs.Remove(QuestID);
	} else
	{
		RegenerateActiveQuests();
	}
	
	OnQuestCompleted.Broadcast(QuestID);
	OnQuestCompletedXP.Broadcast(QuestID, currentQuest.ExperiencePoints);
	return true;
}

bool UGameQuestSubsystem::ForceCompleteQuest(const int32 QuestID)
{
	auto& currentQuest = m_Quests[QuestID];
	const bool validQuest = m_Quests.Contains(QuestID);

	if(validQuest)
	{
		if(currentQuest.bIsMasterQuest)
		{
			for(const auto& subQuestID : currentQuest.SubQuestsIDs)
			{
				ForceCompleteQuest(subQuestID);
			}
		}
		
		currentQuest.bIsActive = false;
		currentQuest.bIsCompleted = true;
	}
	
	if(currentQuest.bIsMasterQuest)
		OnMainQuestCompleted.Broadcast(QuestID);
	
	OnQuestCompleted.Broadcast(QuestID);

	RegenerateActiveQuests();
	return validQuest;
}

bool UGameQuestSubsystem::ActivateQuest(const int32 QuestID, const bool bBroadcastIt)
{
	const auto currentQuest = m_Quests.Find(QuestID);
	if(currentQuest)
	{
		currentQuest->bIsActive = true;
		
		const auto ItemRequirements = currentQuest->QuestItemRequirements;
		// Add the quest item requirements to be tracked
		for(const auto& Requirement : ItemRequirements)
		{
			auto& ReqEntry = m_QuestItemRequirements.FindOrAdd(Requirement.Key);
			ReqEntry += Requirement.Value;
			UE_LOG(LogQuestSystem, Warning, TEXT("Added requirements: %s:%d"), *Requirement.Key, Requirement.Value);
		}

		if(currentQuest->bIsMasterQuest)
		{
			for(const auto& subQuestID : currentQuest->SubQuestsIDs)
			{
				ActivateQuest(subQuestID, false);
			}
		}

		if(!m_ActiveQuestIDs.Contains(QuestID))
		{
			m_ActiveQuestIDs.Push(QuestID);
		}
		
		// Broadcast a delegate with the activated quest id
		if(bBroadcastIt)
		{
			OnQuestActivated.Broadcast(QuestID);
		}
		UE_LOG(LogQuestSystem, Warning, TEXT("Activated quest: ID:%d"), QuestID)
		
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
