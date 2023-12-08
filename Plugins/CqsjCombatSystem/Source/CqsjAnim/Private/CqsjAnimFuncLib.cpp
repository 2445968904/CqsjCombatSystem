// Fill out your copyright notice in the Description page of Project Settings.



// Copyright GanBowen 2022-2023. All Rights Reserved.

#include "CqsjAnimFuncLib.h"

#include "AnimationStateMachineLibrary.h"
#include "CqsjAnimMetaData.h"
#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNode_AssetPlayerBase.h"
#include "Animation/AnimNode_LinkedAnimGraph.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/Package.h"
#include "Animation/AnimClassInterface.h"
#include "Runtime/Launch/Resources/Version.h"

bool FCqsjAnimPlayStateSet::IsValid() const
{
	if (!Instance)
	{
		return false;
	}

	if (AnimPlayStateSet.IsEmpty())
	{
		return false;
	}

	return true;
}

bool FCqsjConditionMontage::CanPlay() const
{
	if (Condition
		&& MontageSetting.MontageToPlay)
	{
		return true;
	}

	return false;
}

UCqsjAnimComponent* UCqsjAnimFuncLib::GetCqsjAnimComponent(AActor* InActor)
{
	if (!InActor)
	{
		return nullptr;
	}
	UCqsjAnimComponent* AComp = nullptr;
	if (UActorComponent* Component = InActor->GetComponentByClass(UCqsjAnimComponent::StaticClass()))
	{
		AComp = Cast<UCqsjAnimComponent>(Component);
	}

	if (!AComp && InActor->HasAuthority())
	{
		AComp = Cast<UCqsjAnimComponent>(
			InActor->AddComponentByClass(UCqsjAnimComponent::StaticClass(),
				false,
				FTransform(),
				false));
		AComp->SetIsReplicated(true);
	}

	return AComp->IsReadyToUse()? AComp : nullptr;
}

void UCqsjAnimFuncLib::SetIncludeMainInstance(AActor* InActor, bool bInclude)
{
	if (UCqsjAnimComponent* AnimComp = GetCqsjAnimComponent(InActor))
	{
		AnimComp->SetIncludeMainInstance(bInclude);
	}
}

void UCqsjAnimFuncLib::SetIncludeLinkedInstance(AActor* InActor, bool bInclude)
{
	if (UCqsjAnimComponent* AnimComp = GetCqsjAnimComponent(InActor))
	{
		AnimComp->SetIncludeLinkedInstance(bInclude);
	}
}

void UCqsjAnimFuncLib::AddIncludeLinkedInstanceClass(AActor* InActor,
	TArray<TSubclassOf<UAnimInstance>> IncludeLinkedAnimInstanceClassSet)
{
	if (UCqsjAnimComponent* AnimComp = GetCqsjAnimComponent(InActor))
	{
		AnimComp->AddIncludeLinkedInstanceClass(IncludeLinkedAnimInstanceClassSet);
	}
}

void UCqsjAnimFuncLib::CqsjPlayMontage(USkeletalMeshComponent* SKMComponent, UAnimMontage* MontageToPlay, float PlayRate,
                                     float StartingPosition, FName StartingSection)
{
	if (SKMComponent && SKMComponent->GetOwner())
	{
		if (UCqsjAnimComponent* AComp = UCqsjAnimFuncLib::GetCqsjAnimComponent(SKMComponent->GetOwner()))
		{
			AComp->PlayMontage(SKMComponent,MontageToPlay,PlayRate,StartingPosition,StartingSection);
		}
	}
}

void UCqsjAnimFuncLib::CqsjStopMontage(USkeletalMeshComponent* SKMComponent)
{
	if (SKMComponent && SKMComponent->GetOwner())
	{
		if (UCqsjAnimComponent* AComp = UCqsjAnimFuncLib::GetCqsjAnimComponent(SKMComponent->GetOwner()))
		{
			AComp->StopMontage(SKMComponent);
		}
	}
}

void UCqsjAnimFuncLib::CqsjConditionPlayMontage(USkeletalMeshComponent* SKMComponent,
                                              TArray<FCqsjConditionMontage> MontagePlaySettings,
                                              bool& CanPlay,
                                              FCqsjConditionMontage& MontageToPlay)
{
	if (SKMComponent)
	{
		for (auto Element : MontagePlaySettings)
		{
			if (Element.CanPlay())
			{
				CanPlay = true;
				MontageToPlay = Element;
				CqsjPlayMontage(
					SKMComponent,
					MontageToPlay.MontageSetting.MontageToPlay,
					MontageToPlay.MontageSetting.PlayRate,
					MontageToPlay.MontageSetting.StartingPosition,
					MontageToPlay.MontageSetting.StartingSection);
			}
		}
	}

	CanPlay = false;
}

UAnimMontage* UCqsjAnimFuncLib::GetNextMontageFromList(AActor* InActor, const TArray<UAnimMontage*>& MontageList)
{
	if (UCqsjAnimComponent* AnimComp = GetCqsjAnimComponent(InActor))
	{
		if (AnimComp)
		{
			return AnimComp->GetNextMontageFromList(MontageList);
		}
	}

	return nullptr;
}

UAnimSequence* UCqsjAnimFuncLib::GetNextAnimSequenceFromList(
	AActor* InActor,
	bool bHoldNowWhenAnimActive,
	bool bHoldNowWhenAnimIsPreAnim,
	const TArray<UAnimSequence*>& AnimSequenceList)
{
	if (UCqsjAnimComponent* AnimComp = GetCqsjAnimComponent(InActor))
	{
		if (AnimComp)
		{
			return AnimComp->GetNextAnimSequenceFromList(bHoldNowWhenAnimActive,bHoldNowWhenAnimIsPreAnim,AnimSequenceList);
		}
	}

	return nullptr;
}

UAnimSequence* UCqsjAnimFuncLib::Anim_Pure_GetNextAnimSequenceFromList(
		const UAnimInstance* AnimInstance,
		ECqsjAnimNodeBlendState StateBlendState,
		const TArray<UAnimSequence*>& AnimSequenceList)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		bool bHoldNowWhenAnimActive = true;
		bool bHoldNowWhenAnimIsPreAnim = false;
		switch (StateBlendState)
		{
		case StateBlendIn:
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		case StateBlendOut: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = true;
			break;
		case NoBlend: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		case StateReBlendIn:
			bHoldNowWhenAnimActive = false;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		default: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
		}
		return UCqsjAnimFuncLib::GetNextAnimSequenceFromList(
			AnimInstance->GetOwningActor(),
			bHoldNowWhenAnimActive,
			bHoldNowWhenAnimIsPreAnim,
			AnimSequenceList);
	}

	return nullptr;
}

void UCqsjAnimFuncLib::Anim_Pure_GetNextAnimSequenceFromList_Pro(
		const UAnimInstance* AnimInstance,
		ECqsjAnimNodeBlendState StateBlendState,
		const TMap<UAnimSequence*, FCqsjAnimSequencePlaySettings>& AnimSequenceSettingList,
		UAnimSequence*& AnimRes,
		FCqsjAnimSequencePlaySettings& PlaySettings)
{
	AnimRes = nullptr;
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		bool bHoldNowWhenAnimActive = true;
		bool bHoldNowWhenAnimIsPreAnim = false;
		switch (StateBlendState)
		{
		case StateBlendIn:
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		case StateBlendOut: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = true;
			break;
		case NoBlend: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		case StateReBlendIn:
			bHoldNowWhenAnimActive = false;
			bHoldNowWhenAnimIsPreAnim = false;
			break;
		default: 
			bHoldNowWhenAnimActive = true;
			bHoldNowWhenAnimIsPreAnim = false;
		}
		TArray<UAnimSequence*> AnimSequenceList ={};
		AnimSequenceSettingList.GetKeys(AnimSequenceList);
		AnimRes = UCqsjAnimFuncLib::GetNextAnimSequenceFromList(AnimInstance->GetOwningActor(),bHoldNowWhenAnimActive,bHoldNowWhenAnimIsPreAnim,AnimSequenceList);
		if (AnimRes)
		{
			PlaySettings = AnimSequenceSettingList.FindRef(AnimRes);
		}
	}
}

bool UCqsjAnimFuncLib::GetAnimPlayState(
		UAnimInstance* Instance,
		FCqsjAnimStateGetType GetType,
		float DeltaTime,
		FCqsjAnimPlayStateSet OldAnimPlayStateSet,
		FCqsjAnimPlayStateSet& AnimPlayStateSet)
{
	if (!Instance)
	{
		return false;
	}

	TArray<FCqsjAnimPlayState> Res_AnimPlayState;
	FCqsjAnimPlayState MainAnimPlayState = FCqsjAnimPlayState();
	TArray<UAnimNotifyState*> InNotifyStateSet;
	TMap<FName,FCqsjAnimCurveValue> InCurveValueState;

	const IAnimClassInterface* Interface = IAnimClassInterface::GetFromClass(Instance->GetClass());
	
	const TArray<FStructProperty*>& LinkedAnimGraphNodeProperties = Interface->GetLinkedAnimGraphNodeProperties();
	TArray<UAnimInstance*> LinkedAnimInstances = {};
	TArray<IAnimClassInterface*> LinkedAnimInstanceInterfaces = {};

	if (GetType.IncludeLinkedAnimInstance)
	{
		for(auto& LinkedAnimGraphNode : LinkedAnimGraphNodeProperties)
		{
			if (LinkedAnimGraphNode->Struct->IsChildOf(FAnimNode_LinkedAnimGraph::StaticStruct()))
			{
				if (FAnimNode_LinkedAnimGraph* NodePtr = LinkedAnimGraphNode->ContainerPtrToValuePtr<FAnimNode_LinkedAnimGraph>(Instance))
				{
					bool bShouldInclude = false;
					for (auto Class : GetType.IncludeLinkedAnimInstanceClassSet)
					{
						if (NodePtr->InstanceClass->IsChildOf(Class))
						{
							bShouldInclude = true;
							break;
						}
					}
					if (bShouldInclude)
					{
						LinkedAnimInstances.Add(NodePtr->GetTargetInstance<UAnimInstance>());
						LinkedAnimInstanceInterfaces.Add(IAnimClassInterface::GetFromClass(NodePtr->InstanceClass));
					}
				}
			}
		}
	}

	TMap<FStructProperty*, UAnimInstance*> AnimNodePropertiesMap;
	
	TArray<FStructProperty*> AnimNodePropertyArr = Interface->GetAnimNodeProperties();
	if (GetType.IncludeMainAnimInstance)
	{
		for(auto AnimNodeProperty : AnimNodePropertyArr)
		{
			AnimNodePropertiesMap.Add(AnimNodeProperty,Instance);
		}
	}

	for (int i = 0; i < LinkedAnimInstances.Num(); i++)
	{
		AnimNodePropertyArr = LinkedAnimInstanceInterfaces[i]->GetAnimNodeProperties();
		for(auto AnimNodeProperty : AnimNodePropertyArr)
		{
			AnimNodePropertiesMap.Add(AnimNodeProperty,LinkedAnimInstances[i]);
		}
	}
	
	for(auto AnimNodePropertyItem : AnimNodePropertiesMap)
	{
		if (AnimNodePropertyItem.Key->Struct->IsChildOf(FAnimNode_AssetPlayerBase::StaticStruct()))
		{
			FAnimNode_AssetPlayerBase* NodePtr = AnimNodePropertyItem.Key->ContainerPtrToValuePtr<FAnimNode_AssetPlayerBase>(AnimNodePropertyItem.Value);
			if (NodePtr->GetAnimAsset())
			{
				ECqsjAnimAssetType AssetType = ECqsjAnimAssetType::None;
				if (NodePtr->GetAnimAsset()->GetClass()->IsChildOf(UAnimSequenceBase::StaticClass()))
				{
					AssetType = ECqsjAnimAssetType::AnimSequence;
				}
				else if (NodePtr->GetAnimAsset()->GetClass()->IsChildOf(UBlendSpace::StaticClass()))
				{
					AssetType = ECqsjAnimAssetType::AnimBlendSpace;
				}
				
				Res_AnimPlayState.Add(FCqsjAnimPlayState(
					AnimNodePropertyItem.Value,
					AssetType,
					NodePtr->GetAnimAsset(),
					NodePtr->GetNodeIndex(),
					0.0f,
					0.0f,
					NodePtr->GetAnimAsset()->GetPlayLength(),
					NodePtr->GetAccumulatedTime(),
					NodePtr->GetAnimAsset()->GetPlayLength(),
					NodePtr->GetAccumulatedTime(),
					NodePtr->GetCachedBlendWeight()
				));
			}
		}
	}

	if (const FAnimMontageInstance* MontageIns = Instance->GetActiveMontageInstance())
	{
		if (MontageIns->Montage)
		{
			float StartTime = 0.0f;
			float EndTime = 0.0f;
			MontageIns->Montage->GetSectionStartAndEndTime(MontageIns->Montage->GetSectionIndex(MontageIns->GetCurrentSection()),StartTime,EndTime);
			Res_AnimPlayState.Add(FCqsjAnimPlayState(
					Instance,
					ECqsjAnimAssetType::AnimMontage,
					MontageIns->Montage,
					-1,
					MontageIns->Montage->BlendIn.GetBlendTime(),
					MontageIns->Montage->BlendOut.GetBlendTime(),
					GetMontageSectionLength(MontageIns->Montage,MontageIns->GetCurrentSection()),
					MontageIns->GetPosition() - StartTime,
					MontageIns->Montage->GetPlayLength(),
					MontageIns->GetPosition(),
					MontageIns->GetWeight()
				));
		}
	}

	//Check Asset PlayState
	for (int i = 0; i < Res_AnimPlayState.Num(); i++)
	{
		FCqsjAnimPlayState OldAnimPlayState;
		if ( UCqsjAnimFuncLib::CqsjGetAnimPlayStateFromArrayByAsset(
			OldAnimPlayStateSet.AnimPlayStateSet,
			Res_AnimPlayState[i].AnimAsset,
			Res_AnimPlayState[i].AnimNodeIndex,
			OldAnimPlayState))
		{
			if (OldAnimPlayState.AnimPlayPosition != Res_AnimPlayState[i].AnimPlayPosition
				|| OldAnimPlayState.AnimBlendWeight != Res_AnimPlayState[i].AnimBlendWeight)
			{
				if (OldAnimPlayState.AnimBlendWeight > Res_AnimPlayState[i].AnimBlendWeight)
				{
					Res_AnimPlayState[i].PlayState = ECqsjAnimPlayStateType::BlendOut;
				}
				else if(OldAnimPlayState.AnimBlendWeight < Res_AnimPlayState[i].AnimBlendWeight)
				{
					Res_AnimPlayState[i].PlayState = ECqsjAnimPlayStateType::BlendIn;
				}
				else
				{
					Res_AnimPlayState[i].PlayState = ECqsjAnimPlayStateType::Active;
				}
				
				if (OldAnimPlayState.AnimPlayPosition > Res_AnimPlayState[i].AnimPlayPosition
					&& (OldAnimPlayState.PlayState == ECqsjAnimPlayStateType::Active
						|| OldAnimPlayState.PlayState == ECqsjAnimPlayStateType::BlendIn))
				{
					Res_AnimPlayState[i].bIsReplay = true;
				}
				else
				{
					Res_AnimPlayState[i].bIsReplay = false;
				}
			}
			else
			{
				Res_AnimPlayState[i].PlayState = ECqsjAnimPlayStateType::Deactivated;
			}
		}
		else
		{
			Res_AnimPlayState[i].PlayState = ECqsjAnimPlayStateType::Deactivated;
		}
	}

	//Get MainAnimAsset
	float MaxBlendWeight = 0.0f;
	for (int i=0; i<Res_AnimPlayState.Num(); i++)
	{
		if (Res_AnimPlayState[i].AnimAsset
			&& (Res_AnimPlayState[i].PlayState == ECqsjAnimPlayStateType::Active || Res_AnimPlayState[i].PlayState == ECqsjAnimPlayStateType::BlendIn)
			&& Res_AnimPlayState[i].AnimBlendWeight >= MaxBlendWeight)
		{
			MaxBlendWeight = Res_AnimPlayState[i].AnimBlendWeight;
			MainAnimPlayState = Res_AnimPlayState[i];
		}
	}

	int MainAssetPriorityLevel = -1;
	for (auto AnimPlayState : Res_AnimPlayState)
	{
		TArray<UAnimMetaData*> AnimMetaData = {};
		if (AnimPlayState.AnimAsset
			&& (AnimPlayState.PlayState == ECqsjAnimPlayStateType::Active || AnimPlayState.PlayState == ECqsjAnimPlayStateType::BlendIn))
		{
			AnimMetaData = AnimPlayState.AnimAsset->GetMetaData();

			for (auto MetaData : AnimMetaData)
			{
				if (UCqsjAnimMetaData_MainAssetSetting* MainAssetSetting = Cast<UCqsjAnimMetaData_MainAssetSetting>(MetaData))
				{
					if (MainAssetSetting->MainAssetPriorityLevel >= MainAssetPriorityLevel)
					{
						MainAssetPriorityLevel = MainAssetSetting->MainAssetPriorityLevel;
						MainAnimPlayState = AnimPlayState;
						break;
					}
				}
			}
		}
	}

	if (!MainAnimPlayState.AnimAsset)
	{
		MainAnimPlayState = OldAnimPlayStateSet.MainAnimPlayState;
		/*MainAnimPlayState.AnimPlayDuration += DeltaTime;
		MainAnimPlayState.AnimPlayPosition += DeltaTime;*/
	}
	
	//Get Anim StateMachineState //Check BlendFinished
	TArray<FCqsjAnimStateMachineNodeInfo> StateMachineState = {};
	bool bIsBlendFinished = true;
	
	TMap<FStructProperty*, UAnimInstance*> StateMachineNodePropertiesMap;
	TArray<FStructProperty*> StateMachineNodePropertyArr = Interface->GetStateMachineNodeProperties();
	for(auto StateMachineNodeProperty : StateMachineNodePropertyArr)
	{
		StateMachineNodePropertiesMap.Add(StateMachineNodeProperty,Instance);
	}
	for (int i = 0; i < LinkedAnimInstances.Num(); i++)
	{
		StateMachineNodePropertyArr = LinkedAnimInstanceInterfaces[i]->GetStateMachineNodeProperties();
		for(auto StateMachineNodeProperty : StateMachineNodePropertyArr)
		{
			StateMachineNodePropertiesMap.Add(StateMachineNodeProperty,LinkedAnimInstances[i]);
		}
	}
	
	for (auto StateMachineNodePropertyItem : StateMachineNodePropertiesMap)
	{
		if (StateMachineNodePropertyItem.Key->Struct->IsChildOf(FAnimNode_StateMachine::StaticStruct()))
		{
			if (FAnimNode_StateMachine* NodePtr = StateMachineNodePropertyItem.Key->ContainerPtrToValuePtr<FAnimNode_StateMachine>(StateMachineNodePropertyItem.Value))
			{
				bool bIsActive = false;
				FCqsjAnimStateMachineNodeInfo OldStateMachineNodeInfo = FCqsjAnimStateMachineNodeInfo();
				if (UCqsjAnimFuncLib::CqsjGetAnimStateMachineStateFromArrayByNodeIndex(
						OldAnimPlayStateSet.StateMachineState,
						NodePtr->GetNodeIndex(),
						OldStateMachineNodeInfo))
				{
					if (NodePtr && NodePtr->GetCurrentStateElapsedTime() != OldStateMachineNodeInfo.ElapsedTime)
					{
						bIsActive = true;
					}
					if (NodePtr && NodePtr->GetNodeIndex() == OldStateMachineNodeInfo.NodeIndex && NodePtr->GetStateWeight(NodePtr->GetCurrentState()) != OldStateMachineNodeInfo.CurrentStateWeight)
					{
						bIsBlendFinished = false;
					}
				}
				
				StateMachineState.Add(FCqsjAnimStateMachineNodeInfo(
					StateMachineNodePropertyItem.Value,
					NodePtr->GetNodeIndex(),
					NodePtr->GetCurrentStateName(),
					OldStateMachineNodeInfo.CurrentStateName != NodePtr->GetCurrentStateName() && OldStateMachineNodeInfo.ElapsedTime>=0.2f? OldStateMachineNodeInfo.CurrentStateName : OldStateMachineNodeInfo.LastStateName,
					NodePtr->GetStateWeight(NodePtr->GetCurrentState()),
					bIsActive,
					NodePtr->GetCurrentStateElapsedTime(),
					OldStateMachineNodeInfo.CurrentStateName != NodePtr->GetCurrentStateName() && OldStateMachineNodeInfo.ElapsedTime>=0.2f? OldStateMachineNodeInfo.ElapsedTime : OldStateMachineNodeInfo.LastStateElapsedTime
				));
			}
		}
	}

	//Make AnimPlayStateSet
	AnimPlayStateSet = FCqsjAnimPlayStateSet(Instance, LinkedAnimInstances, MainAnimPlayState, Res_AnimPlayState, StateMachineState, bIsBlendFinished);
	
	if (AnimPlayStateSet.IsValid())
	{
		//Get Anim NotifyState
		if (AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimMontage
			|| AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimSequence)
		{
			UAnimSequenceBase* A = Cast<UAnimSequenceBase>(AnimPlayStateSet.MainAnimPlayState.AnimAsset);
			if (A)
			{
				for (auto Notify : A->Notifies)
				{
					if (Notify.NotifyStateClass
						&& Notify.GetTriggerTime() <= AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition
						&& Notify.GetEndTriggerTime() >= AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition)
					{
						AnimPlayStateSet.NotifyStateSet.Add(Notify.NotifyStateClass);
					}
				}
			}
		}
		else if (AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimBlendSpace)
		{
			UBlendSpace* B = Cast<UBlendSpace>(AnimPlayStateSet.MainAnimPlayState.AnimAsset);
			for (auto BSSample : B->GetBlendSamples())
			{
				for (auto Notify : BSSample.Animation->Notifies)
				{
					if (Notify.NotifyStateClass
						&& Notify.GetTriggerTime() <= AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition
						&& Notify.GetEndTriggerTime() >= AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition)
					{
						AnimPlayStateSet.NotifyStateSet.Add(Notify.NotifyStateClass);
					}
				}
			}
			/*for (const auto Notify : Instance->ActiveAnimNotifyState)
			{
				if (Notify.NotifyStateClass)
				{
					AnimPlayStateSet.NotifyStateSet.Add(Notify.NotifyStateClass);
				}
			}*/
		}

		//Get Anim Curve State
		TArray<FName> ActiveCurveNames = {};
		Instance->GetActiveCurveNames(EAnimCurveType::AttributeCurve,ActiveCurveNames);
		
		if (AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimMontage
			|| AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimSequence)
		{
			UAnimSequenceBase* A = Cast<UAnimSequenceBase>(AnimPlayStateSet.MainAnimPlayState.AnimAsset);
			if (A)
			{
				for (const auto ActiveCurveName : ActiveCurveNames)
				{
					float CValue = 0.0f;
					float CValueDelta = 0.0f;
					if (UCqsjAnimFuncLib::GetAnimCurveValue(
						A,
						ActiveCurveName,
						AnimPlayStateSet.MainAnimPlayState.BlendInTime,
						AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition,
						DeltaTime,
						CValue,
						CValueDelta))
					{
						AnimPlayStateSet.CurveValueState.Add(
							FCqsjAnimCurveValue(
								ActiveCurveName,
								CValue,
								CValueDelta
							));
					}
				}
			}
		}
		else if (AnimPlayStateSet.MainAnimPlayState.AssetType == ECqsjAnimAssetType::AnimBlendSpace)
		{
			UBlendSpace* B = Cast<UBlendSpace>(AnimPlayStateSet.MainAnimPlayState.AnimAsset);
			UAnimSequenceBase* A = Cast<UAnimSequenceBase>(B->GetBlendSample(0).Animation);
			if (A)
			{
				const float AnimScale = A->GetPlayLength() / B->GetPlayLength();
				for (const auto ActiveCurveName : ActiveCurveNames)
				{
					float CValue = 0.0f;
					float CValueDelta = 0.0f;
					if (UCqsjAnimFuncLib::GetAnimCurveValue(
						A,
						ActiveCurveName,
						AnimPlayStateSet.MainAnimPlayState.BlendInTime,
						AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition * AnimScale,
						DeltaTime * AnimScale,
						CValue,
						CValueDelta))
					{
						AnimPlayStateSet.CurveValueState.Add(
							FCqsjAnimCurveValue(
								ActiveCurveName,
								CValue,
								CValueDelta
							));
					}
				}
			}
		}
	}
	
	return AnimPlayStateSet.IsValid();
}

bool UCqsjAnimFuncLib::CqsjGetAnimPlayState(AActor* InActor, FCqsjAnimPlayStateSet& AnimPlayStateSet)
{
	if (UCqsjAnimComponent* AComp = UCqsjAnimFuncLib::GetCqsjAnimComponent(InActor))
	{
		return AComp->GetAnimPlayState(AnimPlayStateSet);
	}

	return false;
}

bool UCqsjAnimFuncLib::CqsjGetAnimPlayState_Local(AActor* InActor, FCqsjAnimPlayStateSet& AnimPlayStateSet)
{
	if (UCqsjAnimComponent* AComp = UCqsjAnimFuncLib::GetCqsjAnimComponent(InActor))
	{
		return AComp->GetAnimPlayState_Local(AnimPlayStateSet);
	}

	return false;
}

bool UCqsjAnimFuncLib::CqsjGetAnimPlayStateFromArrayByAsset(
		TArray<FCqsjAnimPlayState> AnimPlayStateArr,
		UAnimationAsset* AnimAsset,
		int32 AnimNodeIndex,
		FCqsjAnimPlayState& AnimPlayState)
{
	for (const auto Element : AnimPlayStateArr)
	{
		if (!Element.AnimAsset)
		{
			continue;
		}
		if (AnimNodeIndex<0 || (AnimNodeIndex >=0 && AnimNodeIndex == Element.AnimNodeIndex))
		{
			AnimPlayState = Element;
			return true;
		}
	}

	return false;
}

bool UCqsjAnimFuncLib::CqsjGetAnimStateMachineStateFromArrayByNodeIndex(
	TArray<FCqsjAnimStateMachineNodeInfo> StateMachineNodeInfoArr, int32 NodeIndex,
	FCqsjAnimStateMachineNodeInfo& StateMachineNodeInfo)
{
	bool bIsGet = false;
	for (const auto Element : StateMachineNodeInfoArr)
	{
		if (Element.NodeIndex == NodeIndex)
		{
			StateMachineNodeInfo = Element;
			bIsGet = true;
			break;
		}
	}

	return bIsGet;
}

FString UCqsjAnimFuncLib::GetDebugStringFromAnimPlayState(FCqsjAnimPlayStateSet AnimPlayStateSet)
{
	FString Res = "IsBlendFinished:" + (AnimPlayStateSet.bIsBlendFinished? FString("True") : FString("False"));
	Res = Res + "\nMainAnimPlayState:" + FString::Printf(
										  TEXT("\n    (%s)(%s)(%d)%s %s (%f / %f)  (%f / %f)  %f")
										  ,*(StaticEnum<ECqsjAnimPlayStateType>()->GetNameStringByIndex( ( int32 ) AnimPlayStateSet.MainAnimPlayState.PlayState))
										  ,*(AnimPlayStateSet.MainAnimPlayState.bIsReplay? FString("r") : FString("."))
										  ,AnimPlayStateSet.MainAnimPlayState.AnimNodeIndex
										  ,*(StaticEnum<ECqsjAnimAssetType>()->GetNameStringByIndex( ( int32 ) AnimPlayStateSet.MainAnimPlayState.AssetType))
										  ,*(AnimPlayStateSet.MainAnimPlayState.AnimAsset? AnimPlayStateSet.MainAnimPlayState.AnimAsset->GetName() : "")
										  ,AnimPlayStateSet.MainAnimPlayState.AnimPlayDuration
										  ,AnimPlayStateSet.MainAnimPlayState.AnimPlayLength
										  ,AnimPlayStateSet.MainAnimPlayState.AnimPlayPosition
										  ,AnimPlayStateSet.MainAnimPlayState.AnimAllPlayLength
										  ,AnimPlayStateSet.MainAnimPlayState.AnimBlendWeight
									  );
	Res = Res + "\nAnimPlayedCache:" + AnimPlayStateSet.AnimPlayedCache.GetAnimSequencePlayedCacheDebugString(5);
	
	Res = Res + "\nAnimStateMachineStates:";
	for (int i=0; i<AnimPlayStateSet.StateMachineState.Num(); i++)
	{
		Res = Res + FString::Printf(
			  TEXT("\n    (%s)(%d) %s (%f) (%f) - %s (%f)")
			  ,*(AnimPlayStateSet.StateMachineState[i].bIsActive? FString("a") : FString("."))
			  ,AnimPlayStateSet.StateMachineState[i].NodeIndex
			  ,*AnimPlayStateSet.StateMachineState[i].CurrentStateName.ToString()
			  ,AnimPlayStateSet.StateMachineState[i].CurrentStateWeight
			  ,AnimPlayStateSet.StateMachineState[i].ElapsedTime
			  ,*AnimPlayStateSet.StateMachineState[i].LastStateName.ToString()
			  ,AnimPlayStateSet.StateMachineState[i].LastStateElapsedTime
		  );
	}
	
	Res = Res + "\nAllAnimPlayState";
	for (int i=0; i<AnimPlayStateSet.AnimPlayStateSet.Num(); i++)
	{
		Res = Res + FString::Printf(
			  TEXT("\n    (%s)(%s)(%d)%s %s (%f / %f)  (%f / %f)  %f")
			  ,*(StaticEnum<ECqsjAnimPlayStateType>()->GetNameStringByIndex( ( int32 ) AnimPlayStateSet.AnimPlayStateSet[i].PlayState))
			  ,*(AnimPlayStateSet.AnimPlayStateSet[i].bIsReplay? FString("r") : FString("."))
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimNodeIndex
			  ,*(StaticEnum<ECqsjAnimAssetType>()->GetNameStringByIndex( ( int32 ) AnimPlayStateSet.AnimPlayStateSet[i].AssetType))
			  ,*(AnimPlayStateSet.AnimPlayStateSet[i].AnimAsset? AnimPlayStateSet.AnimPlayStateSet[i].AnimAsset->GetName() : "")
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimPlayDuration
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimPlayLength
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimPlayPosition
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimAllPlayLength
			  ,AnimPlayStateSet.AnimPlayStateSet[i].AnimBlendWeight
		  );
	}

	Res = Res + "\nAnimNotifyState:";
	for (int i=0; i<AnimPlayStateSet.NotifyStateSet.Num(); i++)
	{
		Res = Res + FString::Printf(
			  TEXT("\n    %s")
			  ,*AnimPlayStateSet.NotifyStateSet[i]->GetName()
		);
	}

	Res = Res + "\nCurveValueState:";
	for (auto Curve : AnimPlayStateSet.CurveValueState)
	{
		Res = Res + FString::Printf(
			  TEXT("\n    %s (%f  %f)")
			  ,*Curve.CurveName.ToString()
			  ,Curve.Value
			  ,Curve.DeltaValue
		  );
	}
	
	return Res;
}

UAnimNotifyState* UCqsjAnimFuncLib::GetNotifyStateByClass(FCqsjAnimPlayStateSet AnimPlayStateSet,
	TSubclassOf<UAnimNotifyState> NotifyStateClass)
{
	for (const auto Element : AnimPlayStateSet.NotifyStateSet)
	{
		if (Element && Element->GetClass()->IsChildOf(NotifyStateClass))
		{
			return Element;
		}
	}
	return nullptr;
}

FCqsjAnimCurveValue UCqsjAnimFuncLib::GetCurveValueByName(FCqsjAnimPlayStateSet AnimPlayStateSet, FName CurveName)
{
	for (auto Element : AnimPlayStateSet.CurveValueState)
	{
		if (Element.CurveName == CurveName)
		{
			return Element;
		}
	}

	return FCqsjAnimCurveValue();
}

float UCqsjAnimFuncLib::GetMontageSectionLength(UAnimMontage* Montage, FName SectionName)
{
	if (!Montage)
	{
		return -1;
	}

	return Montage->GetSectionLength(Montage->GetSectionIndex(SectionName));
}

bool UCqsjAnimFuncLib::GetAnimCurveValue(
	UAnimSequenceBase* Anim,
	FName CurveName,
	float TimeOffset,
	float Position,
	float DeltaTime,
	float& Result,
	float& ResultDelta)
{
	if (Anim)
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
		if (Anim->HasCurveData(CurveName))
		{
			const float AnimLength = Anim->GetPlayLength();
			Result = Anim->EvaluateCurveData(CurveName, UKismetMathLibrary::FClamp(Position - TimeOffset,0.0f,AnimLength));
			ResultDelta = Anim->EvaluateCurveData(CurveName, UKismetMathLibrary::FClamp(Position + DeltaTime - TimeOffset,0.0f,AnimLength)) - Result;
			return true;
		}
		return false;
#else
		const USkeleton* Skeleton = Anim->GetSkeleton();
		if (!Skeleton)
		{
			return false;
		}
		const FSmartNameMapping* NameMapping = Skeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);
		if (!NameMapping)
		{
			return false;
		}
		const USkeleton::AnimCurveUID Uid = NameMapping->FindUID(CurveName);
		if (Uid == INDEX_NONE)
		{
			return false;
		}
		const float AnimLength = Anim->GetPlayLength();
		Result = Anim->EvaluateCurveData(Uid, UKismetMathLibrary::FClamp(Position - TimeOffset,0.0f,AnimLength));
		ResultDelta = Anim->EvaluateCurveData(Uid, UKismetMathLibrary::FClamp(Position + DeltaTime - TimeOffset,0.0f,AnimLength)) - Result;
		return true;
#endif
	}

	return false;
}

bool UCqsjAnimFuncLib::IsPlayingSlotAnim(
	UAnimInstance* Instance,
	FName SlotName,
	UAnimSequenceBase*& AnimSequence,
	UAnimMontage*& OutMontage)
{
	for (int32 InstanceIndex = 0; InstanceIndex < Instance->MontageInstances.Num(); InstanceIndex++)
	{
		// check if this is playing
		const FAnimMontageInstance* MontageInstance = Instance->MontageInstances[InstanceIndex];
		// make sure what is active right now is transient that we created by request
		if (MontageInstance && MontageInstance->IsActive() && MontageInstance->IsPlaying())
		{
			UAnimMontage* CurMontage = MontageInstance->Montage;
			if (CurMontage && CurMontage->GetOuter() == GetTransientPackage())
			{
				const FAnimTrack* AnimTrack = CurMontage->GetAnimationData(SlotName);
				if (AnimTrack && AnimTrack->AnimSegments.Num() == 1)
				{
					OutMontage = CurMontage;
					AnimSequence = AnimTrack->AnimSegments[0].GetAnimReference();
					if (AnimSequence)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool UCqsjAnimFuncLib::Anim_IsBlendFinished(AActor* InActor)
{
	FCqsjAnimPlayStateSet AnimPlayStateSet = FCqsjAnimPlayStateSet();
	if (CqsjGetAnimPlayState(InActor,AnimPlayStateSet)
		&& AnimPlayStateSet.bIsBlendFinished)
	{
		return true;
	}

	return false;
}

bool UCqsjAnimFuncLib::Anim_Pure_IsBlendFinished(const UAnimInstance* AnimInstance)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		return UCqsjAnimFuncLib::Anim_IsBlendFinished(AnimInstance->GetOwningActor());
	}

	return false;
}

void UCqsjAnimFuncLib::Anim_Pure_OnlyServer(bool InCondition, const UAnimInstance* AnimInstance, bool& OutCondition)
{
	if (AnimInstance && AnimInstance->GetOwningActor() && AnimInstance->GetOwningActor()->HasAuthority())
	{
		OutCondition = InCondition;
		return;
	}

	OutCondition = false;
}

void UCqsjAnimFuncLib::Cqsj_AnimSM_FollowServer(const UAnimInstance* AnimInstance, FAnimUpdateContext Context, FAnimNodeReference Node)
{
	if (AnimInstance && AnimInstance->GetOwningActor() && !AnimInstance->GetOwningActor()->HasAuthority())
	{
		if (const UCqsjAnimComponent* AComp = UCqsjAnimFuncLib::GetCqsjAnimComponent(AnimInstance->GetOwningActor()))
		{
			FAnimationStateMachineReference AnimationStateMachine = FAnimationStateMachineReference();
			bool bIsSMNode = false;
			UAnimationStateMachineLibrary::ConvertToAnimationStateMachinePure(Node, AnimationStateMachine, bIsSMNode);
			if (bIsSMNode)
			{
				if (const FAnimNode_StateMachine* SMNode = Node.GetAnimNodePtr<FAnimNode_StateMachine>())
				{
					FCqsjAnimStateMachineNodeInfo SMInfo = FCqsjAnimStateMachineNodeInfo();
					if (AComp->GetStateMachineInfoByNodeIndex(SMNode->GetNodeIndex(),SMInfo) && SMInfo.bIsActive)
					{
						if (SMNode->GetCurrentStateName() != SMInfo.CurrentStateName)
						{
							UAnimationStateMachineLibrary::SetState(
                            	Context,
                            	AnimationStateMachine,
                            	SMInfo.CurrentStateName,
                            	0.2f,
                            	ETransitionLogicType::Type::TLT_StandardBlend,
                            	nullptr,
                            	EAlphaBlendOption::Linear,
                            	nullptr);
						}
					}
				}
			}
		}
	}
}
