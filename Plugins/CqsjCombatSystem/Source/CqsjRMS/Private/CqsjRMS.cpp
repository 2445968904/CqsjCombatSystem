// Fill out your copyright notice in the Description page of Project Settings.


#include "CqsjRMS.h"

FCqsjRMS_Common::FCqsjRMS_Common()
{
}

void FCqsjRMS_Common::PrepareRootMotion(float SimulationTime, float MovementTickTime, const ACharacter& Character,
	const UCharacterMovementComponent& MoveComponent)
{
	FRootMotionSource::PrepareRootMotion(SimulationTime, MovementTickTime, Character, MoveComponent);
}

bool FCqsjRMS_Common::UpdateStateFrom(const FRootMotionSource* SourceToTakeStateFrom, bool bMarkForSimulatedCatchup)
{
	return FRootMotionSource::UpdateStateFrom(SourceToTakeStateFrom, bMarkForSimulatedCatchup);
}

bool FCqsjRMS_Common::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return FRootMotionSource::NetSerialize(Ar, Map, bOutSuccess);
}

FRootMotionSource* FCqsjRMS_Common::Clone() const
{
	return FRootMotionSource::Clone();
}

bool FCqsjRMS_Common::Matches(const FRootMotionSource* Other) const
{
	return FRootMotionSource::Matches(Other);
}

bool FCqsjRMS_Common::MatchesAndHasSameState(const FRootMotionSource* Other) const
{
	return FRootMotionSource::MatchesAndHasSameState(Other);
}

UScriptStruct* FCqsjRMS_Common::GetScriptStruct() const
{
	return FRootMotionSource::GetScriptStruct();
}

FString FCqsjRMS_Common::ToSimpleString() const
{
	return FRootMotionSource::ToSimpleString();
}

void FCqsjRMS_Common::AddReferencedObjects(FReferenceCollector& Collector)
{
	FRootMotionSource::AddReferencedObjects(Collector);
}

bool FCqsjRMS_Common::IsRmsInterfaceObjectValid() const
{
	return false;
}

FTransform FCqsjRMS_Common::GetRootMotionParam(float SimulationTime, float MovementTickTime,
	const ACharacter& Character, const UCharacterMovementComponent& MovementComponent) const
{
	return FTransform();
}
