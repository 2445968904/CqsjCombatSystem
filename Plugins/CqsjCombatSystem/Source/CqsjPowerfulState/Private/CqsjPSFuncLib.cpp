// Copyright GanBowen 2022-2023. All Rights Reserved.


#include "CqsjPSFuncLib.h"
#include "CqsjPowerfulStateComponent.h"
#include "Animation/AnimInstance.h"

FPS_Value UCqsjPSFuncLib::MakePSFloat(float Value)
{
	FPS_Value Res;
	Res.SetFValue(Value);
	return Res;
}

FPS_Value UCqsjPSFuncLib::MakePSVector(FVector Value)
{
	FPS_Value Res;
	Res.SetVValue(Value);
	return Res;	
}

FPS_Value UCqsjPSFuncLib::MakePSTransform(FTransform Value)
{
	FPS_Value Res;
	Res.SetTValue(Value);
	return Res;
}

FPS_Value UCqsjPSFuncLib::MakePSActor(AActor* Value)
{
	FPS_Value Res;
	Res.SetActorValue(Value);
	return Res;
}

FPS_Value UCqsjPSFuncLib::MakePSText(FString Value)
{
	FPS_Value Res;
	Res.SetTextValue(Value);
	return Res;
}

FPS_Value UCqsjPSFuncLib::MakePSGT(FGameplayTag Value)
{
	FPS_Value Res;
	Res.SetGTValue(Value);
	return Res;
}

FPS_Value UCqsjPSFuncLib::MakePSBool(bool Value)
{
	FPS_Value Res;
	Res.SetBoolValue(Value);
	return Res;
}

float UCqsjPSFuncLib::GetFValue(const FPS_Value PSValue)
{return PSValue.GetFValue();}

FVector UCqsjPSFuncLib::GetVValue(const FPS_Value PSValue)
{return PSValue.GetVValue();}

FTransform UCqsjPSFuncLib::GetTValue(const FPS_Value PSValue)
{return PSValue.GetTValue();}

AActor* UCqsjPSFuncLib::GetActorValue(const FPS_Value PSValue)
{return PSValue.GetActorValue();}

FString UCqsjPSFuncLib::GetTextValue(const FPS_Value PSValue)
{return PSValue.GetTextValue();}

FGameplayTag UCqsjPSFuncLib::GetGTValue(const FPS_Value PSValue)
{return PSValue.GetGTValue();}

bool UCqsjPSFuncLib::GetBoolValue(const FPS_Value PSValue)
{return PSValue.GetBoolValue();}

FString UCqsjPSFuncLib::GetValueDebugString(FPS_Value PSValue)
{return PSValue.GetString();}

FPS_State UCqsjPSFuncLib::MakePowerfulState()
{
	FPS_State Result;
	return Result;
}

FPS_State UCqsjPSFuncLib::AddFloatToPS(FPS_State InState, FGameplayTag Key, float Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddVectorToPS(FPS_State InState, FGameplayTag Key, FVector Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddTransformToPS(FPS_State InState, FGameplayTag Key, FTransform Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddActorToPS(FPS_State InState, FGameplayTag Key, AActor* Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddTextToPS(FPS_State InState, FGameplayTag Key, FString Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddGTToPS(FPS_State InState, FGameplayTag Key, FGameplayTag Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

FPS_State UCqsjPSFuncLib::AddBoolToPS(FPS_State InState, FGameplayTag Key, bool Value)
{
	FPS_State Res = InState;
	Res.SetState(Key, FPS_Value(Value));
	return Res;
}

bool UCqsjPSFuncLib::GetStateFValue(FPS_State InState, FGameplayTag Key, float& Result)
{
	float Time = -1.0f;
	return InState.GetFValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateVValue(FPS_State InState, FGameplayTag Key, FVector& Result)
{
	float Time = -1.0f;
	return InState.GetVValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateTValue(FPS_State InState, FGameplayTag Key, FTransform& Result)
{
	float Time = -1.0f;
	return InState.GetTValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateActorValue(FPS_State InState, FGameplayTag Key, AActor*& Result)
{
	float Time = -1.0f;
	return InState.GetActorValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateTextValue(FPS_State InState, FGameplayTag Key, FString& Result)
{
	float Time = -1.0f;
	return InState.GetTextValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateGTValue(FPS_State InState, FGameplayTag Key, FGameplayTag& Result)
{
	float Time = -1.0f;
	return InState.GetGTValue(Key, Result, Time);
}

bool UCqsjPSFuncLib::GetStateBoolValue(FPS_State InState, FGameplayTag Key, bool& Result)
{
	float Time = -1.0f;
	return InState.GetBoolValue(Key, Result, Time);
}

UCqsjPowerfulStateComponent* UCqsjPSFuncLib::GetPowerfulStateComponent(AActor* InActor)
{
	if (!InActor)
	{
		return nullptr;
	}
	UCqsjPowerfulStateComponent* PSComp = nullptr;
	if (UActorComponent* Component = InActor->GetComponentByClass(UCqsjPowerfulStateComponent::StaticClass()))
	{
		PSComp = Cast<UCqsjPowerfulStateComponent>(Component);
	}

	if (!PSComp && InActor->HasAuthority())
	{
		PSComp = Cast<UCqsjPowerfulStateComponent>(
			InActor->AddComponentByClass(UCqsjPowerfulStateComponent::StaticClass(),
				false,
				FTransform(),
				false));
		PSComp->SetIsReplicated(true);
	}

	return PSComp? PSComp : nullptr;
}

void UCqsjPSFuncLib::SetPowerfulState(AActor* InActor, bool bOnlyTriggerInServer, FGameplayTag Key, FPS_Value Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		if (bOnlyTriggerInServer && !InActor->HasAuthority())
		{
			return;
		}
		PSC->SetState(Key,Value);
	}
}

void UCqsjPSFuncLib::PS_SetPowerfulState(
	bool InCondition, bool ExecuteWhenNotMeetCondition, bool& OutCondition,
	const UAnimInstance* AnimInstance, FGameplayTag Key, FPS_Value Value)
{
	OutCondition = InCondition;
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		if ((!ExecuteWhenNotMeetCondition && InCondition)
			|| (ExecuteWhenNotMeetCondition && !InCondition))
		{
			UCqsjPSFuncLib::SetPowerfulState(AnimInstance->GetOwningActor(), true, Key, Value);
		}
	}
}

void UCqsjPSFuncLib::RemovePowerfulState(AActor* InActor, bool bOnlyTriggerInServer, FGameplayTag Key)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		if (bOnlyTriggerInServer && !InActor->HasAuthority())
		{
			return;
		}
		PSC->RemoveState(Key);
	}
}

void UCqsjPSFuncLib::PS_RemovePowerfulState(bool InCondition, bool ExecuteWhenNotMeetCondition, bool& OutCondition,
	const UAnimInstance* AnimInstance, FGameplayTag Key)
{
	OutCondition = InCondition;
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		if ((!ExecuteWhenNotMeetCondition && InCondition)
			|| (ExecuteWhenNotMeetCondition && !InCondition))
		{
			UCqsjPSFuncLib::RemovePowerfulState(AnimInstance->GetOwningActor(), true, Key);
		}
	}
}

void UCqsjPSFuncLib::SetPSFloat(AActor* InActor, FGameplayTag Key, float Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateFloat(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSVector(AActor* InActor, FGameplayTag Key, FVector Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateVector(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSTransform(AActor* InActor, FGameplayTag Key, FTransform Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateTransform(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSActor(AActor* InActor, FGameplayTag Key, AActor* Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateActor(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSText(AActor* InActor, FGameplayTag Key, const FString Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateText(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSGT(AActor* InActor, FGameplayTag Key, FGameplayTag Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateGT(Key,Value);
	}
}

void UCqsjPSFuncLib::SetPSBool(AActor* InActor, FGameplayTag Key, bool Value)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SetStateBool(Key,Value);
	}
}

void UCqsjPSFuncLib::GetPSFValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, float& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetFValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSFValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	float& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSFValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSVValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, FVector& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetVValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSVValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	FVector& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSVValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSTValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, FTransform& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetTValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSTValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	FTransform& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSTValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSActorValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, AActor*& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetActorValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSActorValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	AActor*& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSActorValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSTextValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, FString& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetTextValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSTextValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	FString& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSTextValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSGTValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, FGameplayTag& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetGTValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSGTValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	FGameplayTag& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSGTValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSBoolValue(AActor* InActor, FGameplayTag Key, bool& bIsGet, bool& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetBoolValue(Key,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSBoolValue(const UAnimInstance* AnimInstance, FGameplayTag Key, bool& bIsGet,
	bool& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSBoolValue(
			AnimInstance->GetOwningActor(),Key,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSFValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                        float& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetFValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSFValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, float& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSFValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSVValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                        FVector& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetVValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSVValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, FVector& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSVValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSTValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                        FTransform& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetTValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSTValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, FTransform& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSTValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSActorValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                            AActor*& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetActorValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSActorValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key, int DistanceFromNow, bool& bIsGet, AActor*& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSActorValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSTextValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                           FString& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetTextValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSTextValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, FString& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSTextValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSGTValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                         FGameplayTag& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetGTValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSGTValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, FGameplayTag& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSGTValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::GetPSBoolValue_History(AActor* InActor, FGameplayTag Key, int DistanceFromNow, bool& bIsGet,
                                           bool& Result, float& Duration)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		bIsGet = PSC->GetBoolValue_History(Key,DistanceFromNow,Result,Duration);
		return;
	}

	bIsGet = false;
}

void UCqsjPSFuncLib::PS_GetPSBoolValue_History(const UAnimInstance* AnimInstance, FGameplayTag Key,
	int DistanceFromNow, bool& bIsGet, bool& Result, float& Duration)
{
	if (AnimInstance && AnimInstance->GetOwningActor())
	{
		UCqsjPSFuncLib::GetPSBoolValue_History(
			AnimInstance->GetOwningActor(),Key,DistanceFromNow,bIsGet,Result,Duration);
	}
}

void UCqsjPSFuncLib::ActivePSDebug(AActor* InActor, FColor InDebugTextColor, bool InPrintToScreen, bool InPrintToLog)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		return PSC->ActivePSDebug(InDebugTextColor,InPrintToScreen,InPrintToLog);
	}
}

void UCqsjPSFuncLib::DeactivePSDebug(AActor* InActor)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		return PSC->DeactivePSDebug();
	}
}

bool UCqsjPSFuncLib::GetEventFValue(FPE_Event InEvent, FGameplayTag Key, float& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetFValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventVValue(FPE_Event InEvent, FGameplayTag Key, FVector& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetVValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventTValue(FPE_Event InEvent, FGameplayTag Key, FTransform& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetTValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventActorValue(FPE_Event InEvent, FGameplayTag Key, AActor*& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetActorValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventTextValue(FPE_Event InEvent, FGameplayTag Key, FString& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetTextValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventGTValue(FPE_Event InEvent, FGameplayTag Key, FGameplayTag& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetGTValue(Key,Result, Time);
}

bool UCqsjPSFuncLib::GetEventBoolValue(FPE_Event InEvent, FGameplayTag Key, bool& Result)
{
	float Time = -1.0f;
	return InEvent.EventState.GetBoolValue(Key,Result, Time);
}

FPE_Event UCqsjPSFuncLib::MakePE_Event(FGameplayTag EventKey)
{
	FPE_Event Result;
	Result.Key = EventKey;
	return Result;
}

FPE_Event UCqsjPSFuncLib::AddStateToPE(FPE_Event InEvent, FPS_State EventState)
{
	FPE_Event Res = InEvent;
	for(auto S:EventState.State)
	{
		Res.EventState.SetState(S.Key, S.Value);
	}
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddFloatToPE(FPE_Event InEvent, FGameplayTag Key, float Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddVectorToPE(FPE_Event InEvent, FGameplayTag Key, FVector Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddTransformToPE(FPE_Event InEvent, FGameplayTag Key, FTransform Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddActorToPE(FPE_Event InEvent, FGameplayTag Key, AActor* Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddTextToPE(FPE_Event InEvent, FGameplayTag Key, FString Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddGTToPE(FPE_Event InEvent, FGameplayTag Key, FGameplayTag Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

FPE_Event UCqsjPSFuncLib::AddBoolToPE(FPE_Event InEvent, FGameplayTag Key, bool Value)
{
	FPE_Event Res = InEvent;
	Res.EventState.SetState(Key, FPS_Value(Value));
	return Res;
}

void UCqsjPSFuncLib::SendPowerfulEvent(AActor* InActor, FPE_Event PE_Event)
{
	if (UCqsjPowerfulStateComponent* PSC = GetPowerfulStateComponent(InActor))
	{
		PSC->SendEvent(PE_Event);
	}
}

FString UCqsjPSFuncLib::GetPSEventDebugString(FPS_Event InEvent)
{
	return InEvent.GetString();
}

FString UCqsjPSFuncLib::GetPEEventDebugString(FPE_Event InEvent)
{
	return InEvent.GetString();
}
