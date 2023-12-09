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


bool FCqsjFlowMoveTraceSetting::IsMetScreeningConditions(const AActor* InActor)
{
	//判断Class和Tag是否满足条件
	if(!InActor)
	{
		return false;
	}
	if(!UseScreeningConditions
		||(ActorWithClass.IsEmpty()&& ActorWithTag.IsEmpty()))
	{
		return true;
	}

	bool bIsMetActorClass =false;
	bool bIsMetActorTag =false;
	for(auto Class : ActorWithClass)
	{
		if(IsValid(Class) && InActor->GetClass()->IsChildOf(Class))
		{
			bIsMetActorClass = true;
			break;
		}
	}
	for(const auto Tag:ActorWithTag)
	{
		if(InActor->ActorHasTag(Tag))
		{
			bIsMetActorTag = true;
			break;
		}
	}

	return bIsMetActorClass || bIsMetActorTag ;
}

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

void FCqsjFlowMoveState::ClearFlowMoveEvent()
{
	TArray<FFlowMoveEvent> FlowMoveEventNow_Temp = FlowMoveEventNow;
	int RemoveCount =0;
	for(int i=0; i< FlowMoveEventNow_Temp.Num();i++)
	{
		//延迟两帧去清除时间
		if(FlowMoveEventNow_Temp[i].InFrameNum<= GFrameCounter-2)
		{
			FlowMoveEventNow.RemoveAt((i-RemoveCount));
			RemoveCount++;//删除后数组会变小
		}
	}
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

void UCqsjFlowMoveScript_Base::Update(ECqsjFlowMoveEventType EventType, EFlowMoveNetworkLocationType NetworkLocation,
	bool bIsLocalOwn, UCqsjFlowMoveComponent* FlowMoveComponent)
{
}

void UCqsjFlowMoveScript_Base::CheckCopyCache()
{
}

UCqsjFlowMoveScript_Base* UCqsjFlowMoveScript_Base::GetCopy(UObject* Outer)
{
	return nullptr;
}

USceneComponent* UCqsjFlowMoveGetFocusActor_Base::OnGetFocusActor_Implementation(
	UCqsjFlowMoveComponent* FlowMoveComponent)
{
	return nullptr;
}

USceneComponent* UCqsjFlowMoveGetFocusActor_Base::GetFocusActor(UCqsjFlowMoveComponent* FlowMoveComponent)
{
	return nullptr;
}

bool UCqsjFlowMoveGetFocusActor_Base::TestVisibility(AActor* StartActor, AActor* EndActor,
	FCqsjFlowMoveTraceSetting TraceSetting, FVector& TouchPoint)
{
	return true;
}

void UCqsjFlowMoveBrain_Base::OnFlowMoveEvent_Implementation(ACharacter* OwnerCharacter,
                                                             UCqsjFlowMoveComponent* FlowMoveComponent, FCqsjFlowMoveState FlowMoveState, const FFlowMoveEvent& FlowMoveEvent)
{
}

void UCqsjFlowMoveBrain_Base::OnFMEvent(ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent,
	FCqsjFlowMoveState FlowMoveState, const FFlowMoveEvent& FlowMoveEvent)
{
}
//意义不明
void UCqsjFlowMoveBrain_Base::GetFlowMoveIsActive_Implementation(ACharacter* OwnerCharacter,
	UCqsjFlowMoveComponent* FlowMoveComponent, float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsActive,
	bool& WaitForCurrentActionFinished)
{
	bIsActive =false ;
	WaitForCurrentActionFinished =true ;
}

void UCqsjFlowMoveBrain_Base::GetFMIsActive(ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent,
	float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsActive, bool& WaitForCurrentActionFinished)
{
	GetFlowMoveIsActive(
		OwnerCharacter,
		FlowMoveComponent,
		DeltaTime,
		FlowMoveState,
		bIsActive,
		WaitForCurrentActionFinished);
}

void UCqsjFlowMoveBrain_Base::GetMoveVector_Implementation(ACharacter* OwnerCharacter,
	UCqsjFlowMoveComponent* FlowMoveComponent, float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsGet,
	FVector& MoveVector)
{
}

void UCqsjFlowMoveBrain_Base::GetFmMoveVector(ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent,
	float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsGet, FVector& MoveVector)
{
}

void UCqsjFlowMoveBrain_Base::GetControlVector_Implementation(ACharacter* OwnerCharacter,
                                                              UCqsjFlowMoveComponent* FlowMoveComponent, float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsGet,
                                                              FVector& ControlVector)
{
}

void UCqsjFlowMoveBrain_Base::GetFMControlVector(ACharacter* OwnerCharacter, UCqsjFlowMoveComponent* FlowMoveComponent,
	float DeltaTime, FCqsjFlowMoveState FlowMoveState, bool& bIsGet, FVector& ControlVector)
{
}
