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