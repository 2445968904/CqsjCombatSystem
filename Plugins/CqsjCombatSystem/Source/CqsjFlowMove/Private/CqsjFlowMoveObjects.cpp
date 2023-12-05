// Copyright GanBowen 2022-2023. All Rights Reserved.

#include "CqsjFlowMoveObjects.h"
#include "CqsjFlowMoveFuncLib.h"
#include "CqsjFlowMoveComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Animation/Skeleton.h"
#include "Engine/SkeletalMesh.h"


bool UCqsjFlowMoveObject_Base::IsNameStableForNetworking() const
{
	return true;
}

bool UCqsjFlowMoveObject_Base::IsSupportedForNetworking() const
{
	return true;
}

UCqsjFlowMoveObject_Base::UCqsjFlowMoveObject_Base()
{Guid = FGuid::NewGuid();}

void UCqsjFlowMoveObject_Base::SetWorldContext(UObject* NewWorldContext)
{
	WorldContext = new FWorldContext();
	WorldContext->SetCurrentWorld(NewWorldContext->GetWorld());
}

UObject* UCqsjFlowMoveObject_Base::GetWorldContextObject()
{
	if (WorldContext && WorldContext->World())
	{
		return WorldContext->World();
	}
	
	WorldContext = new FWorldContext();
	WorldContext->SetCurrentWorld(GWorld->GetWorld());
	return GWorld->GetWorld();
}

UWorld* UCqsjFlowMoveObject_Base::GetWorld() const
{
	if (UWorld* TheWorld = GWorld->GetWorld())
	{
		if (TheWorld->WorldType == EWorldType::Type::Game || TheWorld->WorldType == EWorldType::Type::PIE)
		{
			return TheWorld;
		}
	}
	return nullptr;
}

bool FFlowMoveMoveVectorCondition::IsMet(const FCqsjFlowMoveState& FlowMoveState)
{
	return true;
}

void UCqsjFlowMovePerception_Base::SetCache(FCqsjFlowMoveScene TheFlowMoveSceneCache)
{
	
}

bool UCqsjFlowMovePerception_Base::GetCache(FCqsjFlowMoveScene& Result, const bool bConsume)
{
	return true;
}



bool UCqsjFlowMovePerception_Base::GetPerceptionResult_Implementation(FCqsjFlowMoveScene& FlowMoveScene,
	ECqsjFlowMoveEventType EventType, ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent)
{
	return  true;
}

bool UCqsjFlowMovePerception_Base::GetThePerceptionResult(FCqsjFlowMoveScene& FlowMoveScene,
	const ECqsjFlowMoveEventType EventType, ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent)
{
	return true;
}

void UCqsjFlowMovePerception_Base::OnUpdate_Implementation(FGameplayTag PerceptionKey, ECqsjFlowMoveEventType EventType,
                                                           UCqsjFlowMoveComponent* FlowMoveComponent)
{
}

void UCqsjFlowMovePerception_Base::CheckCopyCache()
{
}

UCqsjFlowMovePerception_Base* UCqsjFlowMovePerception_Base::GetCopy(UObject* Outer)
{
	return  nullptr;
}
