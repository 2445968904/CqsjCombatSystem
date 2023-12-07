// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "UObject/Object.h"
#include "CqsjAnimMetaData.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "GBW_MainAssetSetting"))
class GBWANIM_API UCqsjAnimMetaData_MainAssetSetting : public UAnimMetaData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Settings")
	int MainAssetPriorityLevel = -1;
};