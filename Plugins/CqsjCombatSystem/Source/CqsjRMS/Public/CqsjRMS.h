// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CqsjRMSInterface.h"
#include "UObject/Object.h"
#include "GameFramework/RootMotionSource.h"
#include "CqsjRMS.generated.h"

/**
 * 
 */
USTRUCT()
struct CQSJRMS_API FCqsjRMS_Common : public FRootMotionSource
{
	GENERATED_BODY()
	FCqsjRMS_Common();

	virtual ~FCqsjRMS_Common(){}

	virtual void PrepareRootMotion(float SimulationTime, float MovementTickTime, const ACharacter& Character, const UCharacterMovementComponent& MoveComponent) override;
	
	virtual  bool UpdateStateFrom(const FRootMotionSource* SourceToTakeStateFrom, bool bMarkForSimulatedCatchup) override;


	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	virtual FRootMotionSource* Clone() const override;

	virtual bool Matches(const FRootMotionSource* Other) const override;

	virtual bool MatchesAndHasSameState(const FRootMotionSource* Other) const override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FString ToSimpleString() const override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	UPROPERTY()
	TScriptInterface<ICqsjRMSInterface> RMSInterfaceObj = nullptr;
	
	bool IsRmsInterfaceObjectValid () const ;
	FTransform GetRootMotionParam(
		float SimulationTime,
		float MovementTickTime,
		const ACharacter & Character,
		const UCharacterMovementComponent & MovementComponent) const ;
};

template<>
struct TStructOpsTypeTraits< FCqsjRMS_Common > : public TStructOpsTypeTraitsBase2< FCqsjRMS_Common >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
