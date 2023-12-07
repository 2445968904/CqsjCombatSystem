// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CqsjAnimComponent.h"
#include "CqsjAnimFuncLib.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCqsjAnimSequencePlaySettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float PlayRateBasis = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float PlayRate = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float StartPosition = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool LoopAnimation = true;
	
	FCqsjAnimSequencePlaySettings(){}
};


UCLASS()
class CQSJANIM_API UCqsjAnimFuncLib : public UObject
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static UCqsjAnimComponent* GetCqsjAnimComponent(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static void SetIncludeMainInstance(AActor* InActor, bool bInclude = false);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static void SetIncludeLinkedInstance(AActor* InActor, bool bInclude = true);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static void AddIncludeLinkedInstanceClass(AActor* InActor, TArray<TSubclassOf<UAnimInstance>> IncludeLinkedAnimInstanceClassSet);
	
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static bool GetAnimPlayState(
		UAnimInstance* Instance,
		FCqsjAnimStateGetType GetType,
		float DeltaTime,
		FCqsjAnimPlayStateSet OldAnimPlayStateSet,
		FCqsjAnimPlayStateSet& AnimPlayStateSet);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static bool CqsjGetAnimPlayState(AActor* InActor, FCqsjAnimPlayStateSet& AnimPlayStateSet);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static bool CqsjGetAnimPlayState_Local(AActor* InActor, FCqsjAnimPlayStateSet& AnimPlayStateSet);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static bool CqsjGetAnimPlayStateFromArrayByAsset(
		TArray<FCqsjAnimPlayState> AnimPlayStateArr,
		UAnimationAsset* AnimAsset,
		int32 AnimNodeIndex,
		FCqsjAnimPlayState& AnimPlayState);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static bool CqsjGetAnimStateMachineStateFromArrayByNodeIndex(
		TArray<FCqsjAnimStateMachineNodeInfo> StateMachineNodeInfoArr,
		int32 NodeIndex,
		FCqsjAnimStateMachineNodeInfo& StateMachineNodeInfo);
	
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static void CqsjPlayMontage(USkeletalMeshComponent* SKMComponent,
		UAnimMontage* MontageToPlay,
		float PlayRate=1.0f,
		float StartingPosition=0.0f,
		FName StartingSection=NAME_None);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static void CqsjStopMontage(USkeletalMeshComponent* SKMComponent);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static void CqsjConditionPlayMontage(USkeletalMeshComponent* SKMComponent,
		TArray<FCqsjConditionMontage> MontagePlaySettings,
		bool& CanPlay,
		FCqsjConditionMontage& MontageToPlay);

	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static UAnimMontage* GetNextMontageFromList(AActor* InActor,
	                                            const TArray<UAnimMontage*>& MontageList);

	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim", meta=(DefaultToSelf="InActor"))
	static UAnimSequence* GetNextAnimSequenceFromList(
		AActor* InActor,
		bool bHoldNowWhenAnimActive,
		bool bHoldNowWhenAnimIsPreAnim,
		const TArray<UAnimSequence*>& AnimSequenceList);
	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim|Pure", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	static UAnimSequence* Anim_Pure_GetNextAnimSequenceFromList(
		const UAnimInstance* AnimInstance,
		ECqsjAnimNodeBlendState StateBlendState,
		const TArray<UAnimSequence*>& AnimSequenceList);
	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim|Pure", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	static void Anim_Pure_GetNextAnimSequenceFromList_Pro(
		const UAnimInstance* AnimInstance,
		ECqsjAnimNodeBlendState StateBlendState,
		const TMap<UAnimSequence*, FCqsjAnimSequencePlaySettings>& AnimSequenceSettingList,
		UAnimSequence*& AnimRes,
		FCqsjAnimSequencePlaySettings& PlaySettings);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static FString GetDebugStringFromAnimPlayState(
		FCqsjAnimPlayStateSet AnimPlayStateSet);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static UAnimNotifyState* GetNotifyStateByClass(FCqsjAnimPlayStateSet AnimPlayStateSet, TSubclassOf<UAnimNotifyState> NotifyStateClass);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static FCqsjAnimCurveValue GetCurveValueByName(FCqsjAnimPlayStateSet AnimPlayStateSet, FName CurveName);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static float GetMontageSectionLength(UAnimMontage* Montage, FName SectionName);

	UFUNCTION(BlueprintCallable, Category="Cqsj|Anim")
	static bool GetAnimCurveValue(
		UAnimSequenceBase* Anim,
		FName CurveName,
		float TimeOffset,
		float Position,
		float DeltaTime,
		float& Result,
		float& ResultDelta);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	static bool IsPlayingSlotAnim(
		UAnimInstance* Instance,
		FName SlotName,
		UAnimSequenceBase*& AnimSequence,
		UAnimMontage*& OutMontage);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim|Condition", meta=(DefaultToSelf="InActor"))
	static bool Anim_IsBlendFinished(AActor* InActor);
	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim|Pure|Condition", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	static bool Anim_Pure_IsBlendFinished(const UAnimInstance* AnimInstance);

	//On the server, OutCondition is the same as InCondition;
	//On the client side, OutCondition is false.
	UFUNCTION(BlueprintPure, Category = "Cqsj|Anim|Pure|Condition", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	static void Anim_Pure_OnlyServer(
		bool InCondition,
		const UAnimInstance* AnimInstance,
		bool& OutCondition);

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	static void Cqsj_AnimSM_FollowServer(const UAnimInstance* AnimInstance, FAnimUpdateContext Context, FAnimNodeReference Node);

};
