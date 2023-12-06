// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CqsjRMSInterface.h"
#include "UObject/Object.h"
#include "CqsjRMSBPFuncLib.generated.h"

/**
 * 
 */
UCLASS()
class CQSJRMS_API UCqsjRMSBPFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Cqsj|RMS")
	static int32 CqsjRMS_Common(
		FName& InstanceName,
		TScriptInterface<ICqsjRMSInterface> RMSInterfaceObj,
		UCharacterMovementComponent* MovementComponent,
		bool ClearOtherRMS = true
		
		);
};
