// Fill out your copyright notice in the Description page of Project Settings.


#include "CqsjRMSBPFuncLib.h"
#include "Kismet/GameplayStatics.h"
#include "CqsjRMS.h"

int32 UCqsjRMSBPFuncLib::CqsjRMS_Common(FName& InstanceName, TScriptInterface<ICqsjRMSInterface> RMSInterfaceObj,
                                        UCharacterMovementComponent* MovementComponent, bool ClearOtherRMS)
{
	InstanceName = CqsjRms_GetRmsCommonInstanceName();
	if(!MovementComponent)
	{
		return -1;
	}
	constexpr int32 NewPriority =0 ;
	if(ClearOtherRMS)
	{
		MovementComponent->CurrentRootMotion.Clear();
	}
	else if(auto RMS = MovementComponent->GetRootMotionSource(InstanceName))
	{
		MovementComponent->RemoveRootMotionSource(InstanceName);
	}

	TSharedPtr<FCqsjRMS_Common> RmsCommon = MakeShared<FCqsjRMS_Common>();
	RmsCommon->Duration =100000000.0f;
	RmsCommon->InstanceName = InstanceName;
	RmsCommon->AccumulateMode = ERootMotionAccumulateMode::Override;
	RmsCommon->Settings.SetFlag(ERootMotionSourceSettingsFlags::UseSensitiveLiftoffCheck);
	RmsCommon->Priority = NewPriority;

	RmsCommon->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
	RmsCommon->FinishVelocityParams.SetVelocity = FVector::Zero();
	RmsCommon->FinishVelocityParams.ClampVelocity = 650.0f;

	RmsCommon->SetTime(0);

	return MovementComponent->ApplyRootMotionSource(RmsCommon);
}

bool UCqsjRMSBPFuncLib::CqsjRMS_StopRMSCommon(UCharacterMovementComponent* MovementComponent)
{
	if(MovementComponent)
	{
		if(auto RMS = MovementComponent->GetRootMotionSource(CqsjRms_GetRmsCommonInstanceName()))
		{
			MovementComponent->RemoveRootMotionSource(CqsjRms_GetRmsCommonInstanceName());
		}
		return true ;
	}
	return false ; 
}

FName UCqsjRMSBPFuncLib::CqsjRms_GetRmsCommonInstanceName()
{
	return FName("CqsjRMSCommon");
}

bool UCqsjRMSBPFuncLib::CqsjRms_IsRmsCommonActive(UCharacterMovementComponent* MovementComponent)
{
	if(MovementComponent)
	{
		if(auto RMS = MovementComponent->GetRootMotionSource(CqsjRms_GetRmsCommonInstanceName()))
		{
			return true;
		}
	}
	return false;
}


bool UCqsjRMSBPFuncLib::CqsjRms_GetCharacterViewInfo(ACharacter* Character, FVector& ViewLocation,
	FRotator& ViewRotation, FVector& ForwardVector)
{
	if (Character)
	{
		if (UGameplayStatics::GetPlayerPawn(Character->GetWorld(),0) == Character)
		{
			const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(Character->GetWorld(),0);
			ViewLocation = CameraManager->GetCameraLocation();
			ViewRotation = CameraManager->GetCameraRotation();
			ForwardVector = CameraManager->GetActorForwardVector();
		}
		else
		{
			Character->GetActorEyesViewPoint(ViewLocation,ViewRotation);
			ForwardVector = Character->GetActorForwardVector();
		}
		
		return true;
	}

	return false;
}