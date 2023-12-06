// Copyright GanBowen 2022-2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CqsjFlowMoveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCqsjFlowMoveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CQSJFLOWMOVE_API ICqsjFlowMoveInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cqsj|FlowMove")
	void CanInteractive(
		ACharacter* FromCharacter,
		UCqsjFlowMoveComponent* FromFlowMoveComponent,
		bool& CanInteractive);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cqsj|FlowMove")
	void GetTargetPoint(
		ACharacter* FromCharacter,
		UCqsjFlowMoveComponent* FromFlowMoveComponent,
		bool& bIsGet,
		FVector& TargetPoint,
		FName& SceneTag);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
