// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CqsjPowerfulAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ECqsjAnimNodeBlendState
{
	NoBlend,
	StateBlendIn,
	StateBlendOut,
	StateReBlendIn
};

UCLASS()
class CQSJANIM_API UCqsjPowerfulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="States", meta = (DisplayName="State Blend State", BlueprintInternalUseOnly = "true", AnimGetter="true", BlueprintThreadSafe))
	ECqsjAnimNodeBlendState GetStateBlendState(int32 MachineIndex, int32 StateIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim", meta=(DefaultToSelf="AnimInstance", BlueprintThreadSafe))
	void AnimSM_FollowServer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

};
