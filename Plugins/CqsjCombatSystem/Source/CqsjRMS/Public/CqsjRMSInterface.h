// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CqsjRMSInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCqsjRMSInterface : public UInterface
{
	GENERATED_BODY()
};

class CQSJRMS_API ICqsjRMSInterface
{
		GENERATED_BODY()	
public:
	virtual  bool GetRootMotionParamDirect(
		FTransform& Result,
		float SimulationTime,
		float MovementTickTime,
		const ACharacter & Character,
		const UCharacterMovementComponent& MovementComponent)
	{return false;}

};