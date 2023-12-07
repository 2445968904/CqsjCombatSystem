// Fill out your copyright notice in the Description page of Project Settings.


#include "CqsjPowerfulStateComponent.h"
#include "CqsjPSFuncLib.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


float FPS_Value::GetFValue() const
{
	if (ValueType == EPS_ValueType::Float)
	{
		return FValue;
	}
	return 0.0;
}

FVector FPS_Value::GetVValue() const
{
	if (ValueType == EPS_ValueType::Vector)
	{
		return VValue;
	}
	return FVector::ZeroVector;
}

FTransform FPS_Value::GetTValue() const
{
	if (ValueType == EPS_ValueType::Transform)
	{
		return TValue;
	}
	return FTransform();
}

AActor* FPS_Value::GetActorValue() const
{
	if (ValueType == EPS_ValueType::Actor)
	{
		return ActorValue;
	}
	return nullptr;
}

FString FPS_Value::GetTextValue() const
{
	if (ValueType == EPS_ValueType::Text)
	{
		return TextValue;
	}
	return "";
}

FGameplayTag FPS_Value::GetGTValue() const
{
	if (ValueType == EPS_ValueType::GameplayTag)
	{
		return GTValue;
	}
	return FGameplayTag::EmptyTag;
}

bool FPS_Value::GetBoolValue() const
{
	if (ValueType == EPS_ValueType::Bool)
	{
		return BoolValue;
	}
	return false;
}

bool FPS_Value::IsTypeMatch(const EPS_ValueType InValueType) const
{
	return ValueType == InValueType;
}

void FPS_Value::SetFValue(const float NewValue)
{FValue = NewValue;ValueType = EPS_ValueType::Float;}

void FPS_Value::SetVValue(const FVector& NewValue)
{VValue = NewValue;ValueType = EPS_ValueType::Vector;}

void FPS_Value::SetTValue(const FTransform& NewValue)
{TValue = NewValue;ValueType = EPS_ValueType::Transform;}

void FPS_Value::SetActorValue(AActor* NewValue)
{ActorValue = NewValue;ValueType = EPS_ValueType::Actor;}

void FPS_Value::SetTextValue(FString NewValue)
{TextValue = NewValue;ValueType = EPS_ValueType::Text;}

void FPS_Value::SetGTValue(const FGameplayTag NewValue)
{GTValue = NewValue;ValueType = EPS_ValueType::GameplayTag;}

void FPS_Value::SetBoolValue(const bool NewValue)
{BoolValue = NewValue;ValueType = EPS_ValueType::Bool;}

FPS_Value::FPS_Value(const float NewValue)
{
	ValueType = EPS_ValueType::Float;
	FValue = NewValue;
}

FPS_Value::FPS_Value(const FVector& NewValue)
{
	ValueType = EPS_ValueType::Vector;
	VValue = NewValue;
}

FPS_Value::FPS_Value(const FTransform& NewValue)
{
	ValueType = EPS_ValueType::Transform;
	TValue = NewValue;
}

FPS_Value::FPS_Value(AActor* NewValue)
{
	ValueType = EPS_ValueType::Actor;
	ActorValue = NewValue;
}

FPS_Value::FPS_Value(FString NewValue)
{
	ValueType = EPS_ValueType::Text;
	TextValue = NewValue;
}

FPS_Value::FPS_Value(const FGameplayTag NewValue)
{
	ValueType = EPS_ValueType::GameplayTag;
	GTValue = NewValue;
}

FPS_Value::FPS_Value(bool NewValue)
{
	ValueType = EPS_ValueType::Bool;
	BoolValue = NewValue;
}

bool FPS_Value::EqualTo(const FPS_Value& Other) const
{
	if (ValueType == Other.ValueType)
	{
		switch (ValueType) { case EPS_ValueType::Float: return FValue==Other.FValue;
		case EPS_ValueType::Vector: return VValue==Other.VValue;
		case EPS_ValueType::Transform: return TValue.Equals(Other.TValue);
		case EPS_ValueType::Actor: return ActorValue==Other.ActorValue;
		case EPS_ValueType::Text: return TextValue==Other.TextValue;
		case EPS_ValueType::GameplayTag: return GTValue==Other.GTValue;
		case EPS_ValueType::Bool: return BoolValue==Other.BoolValue;
		default: ;
		}
	}
	return false;
}

FString FPS_Value::GetString() const
{
	switch (ValueType)
	{
	case EPS_ValueType::Float: return "<Float> "+FString::Printf(TEXT("%f"), FValue);
	case EPS_ValueType::Vector: return "<Vector> "+VValue.ToString();
	case EPS_ValueType::Transform: return "<Transform> "+TValue.ToString();
	case EPS_ValueType::Actor: return "<Actor> " + (ActorValue? ActorValue->GetName() : "None");
	case EPS_ValueType::Text: return "<Text> " + TextValue;
	case EPS_ValueType::GameplayTag: return "<GT> " + GTValue.ToString();
	case EPS_ValueType::Bool: return "<Bool> " + (BoolValue? FString("True") : FString("False"));
	default: return "";
	}
}

FPS_Element::FPS_Element(const FGameplayTag InKey, const FPS_Value& InValue, float InTime)
{
	Key = InKey;
	Value = InValue;
	Time = InTime;
}

FString FPS_Element::GetString(UWorld* InWorld, bool bIsGetOperationTime) const
{
	return FString::Printf(TEXT("%s : %s (%.2fs)"),
		*Key.ToString(),
		*Value.GetString(),
		bIsGetOperationTime? Time : (InWorld? InWorld->GetTimeSeconds() - Time : -1.0f)
		);
}

FPS_StateOperation::FPS_StateOperation(EPS_StateEventType InOperationType, FPS_Element InStateElement)
{
	OperationType = InOperationType;
	StateElement = InStateElement;
}

FString FPS_StateOperation::GetString(UWorld* InWorld) const
{
	return FString::Printf(TEXT("<%s> %s"),
		*(StaticEnum<EPS_StateEventType>()->GetNameStringByIndex( ( int32 ) OperationType)),
		*StateElement.GetString(InWorld, true)
		);
}

void FPS_State::SetState(const FGameplayTag Key, const FPS_Value& Value, float Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key)
		{
			if (State[i].Value.EqualTo(Value))
			{
				return;
			}
			State[i].Value = Value;
			State[i].Time = Time;
			AddStateOperation(FPS_StateOperation(EPS_StateEventType::Change,State[i]));
			return;
		}
	}
	
	State.Add(FPS_Element(Key, Value, Time));
	AddStateOperation(FPS_StateOperation(EPS_StateEventType::Add,FPS_Element(Key, Value, Time)));
}

bool FPS_State::RemoveState(const FGameplayTag Key, FPS_Value& OldValue, float Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key)
		{
			OldValue = State[i].Value;
			State[i].Time = Time;
			AddStateOperation(FPS_StateOperation(EPS_StateEventType::Remove,State[i]));
			State.RemoveAt(i);
			return true;
		}
	}
	return false;
}

bool FPS_State::GetFValue(const FGameplayTag Key, float& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Float))
		{
			Result = UCqsjPSFuncLib::GetFValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetVValue(const FGameplayTag Key, FVector& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Vector))
		{
			Result = UCqsjPSFuncLib::GetVValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetTValue(const FGameplayTag Key, FTransform& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Transform))
		{
			Result = UCqsjPSFuncLib::GetTValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetActorValue(const FGameplayTag Key, AActor*& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Actor))
		{
			Result = UCqsjPSFuncLib::GetActorValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetTextValue(const FGameplayTag Key, FString& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Text))
		{
			Result = UCqsjPSFuncLib::GetTextValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetGTValue(const FGameplayTag Key, FGameplayTag& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::GameplayTag))
		{
			Result = UCqsjPSFuncLib::GetGTValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetBoolValue(FGameplayTag Key, bool& Result, float& Time)
{
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key && State[i].Value.IsTypeMatch(EPS_ValueType::Bool))
		{
			Result = UCqsjPSFuncLib::GetBoolValue(State[i].Value);
			Time = State[i].Time;
			return true;
		}
	}
	return false;
}

bool FPS_State::GetFValue_History(FGameplayTag Key, int DistanceFromNow, float& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetFValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Float))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetFValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetVValue_History(FGameplayTag Key, int DistanceFromNow, FVector& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetVValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Vector))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetVValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetTValue_History(FGameplayTag Key, int DistanceFromNow, FTransform& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetTValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Transform))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetTValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetActorValue_History(FGameplayTag Key, int DistanceFromNow, AActor*& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetActorValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Actor))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetActorValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetTextValue_History(FGameplayTag Key, int DistanceFromNow, FString& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetTextValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Text))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetTextValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetGTValue_History(FGameplayTag Key, int DistanceFromNow, FGameplayTag& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetGTValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - 1 - i >= 0)
		{
			Index = StateOperationIndexNow - i - 1;
		}
		else
		{
			Index = StateOperationIndexNow - i - 1 + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::GameplayTag))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetGTValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

bool FPS_State::GetBoolValue_History(FGameplayTag Key, int DistanceFromNow, bool& Result, float& Time)
{
	if (DistanceFromNow <= 0)
	{
		return GetBoolValue(Key,Result,Time);
	}

	FPS_StateOperation LastOperation;
	FPS_StateOperation TargetOperation;
	int DistanceCache = 0;
	bool bIsGet = false;
	for (int i=0; i<StateOperations.Num(); i++)
	{
		int Index = -1;
		if (StateOperationIndexNow - i >= 0)
		{
			Index = StateOperationIndexNow - i;
		}
		else
		{
			Index = StateOperationIndexNow - i + StateOperations.Num();
		}
		if (StateOperations.Num() <= Index || Index < 0)
		{
			break;
		}

		if (StateOperations[Index].StateElement.Key == Key
			&& StateOperations[Index].StateElement.Value.IsTypeMatch(EPS_ValueType::Bool))
		{
			DistanceCache++;
			if (DistanceCache == DistanceFromNow)
			{
				LastOperation = StateOperations[Index];
			}
			else if (DistanceCache == DistanceFromNow+1)
			{
				TargetOperation = StateOperations[Index];
				bIsGet = true;
				break;
			}
		}
	}

	if (bIsGet)
	{
		if (TargetOperation.OperationType == EPS_StateEventType::Add || TargetOperation.OperationType == EPS_StateEventType::Change)
		{
			Result = TargetOperation.StateElement.Value.GetBoolValue();
			Time = LastOperation.StateElement.Time - TargetOperation.StateElement.Time;
		}
		else
		{
			return false;
		}
	}

	return bIsGet;
}

void FPS_State::AddStateOperation(const FPS_StateOperation& NewStateOperation)
{
	if (StateOperations.Num() <= MaxStateOperationNum)
	{
		StateOperationIndexNow = StateOperations.Num();
		StateOperations.Add(NewStateOperation);
	}
	else
	{
		if (StateOperationIndexNow >= MaxStateOperationNum)
		{
			StateOperationIndexNow = 0;
		}
		StateOperations[StateOperationIndexNow] = NewStateOperation;
		StateOperationIndexNow++;
	}
}

bool FPS_State::CheckStateChangeType(
	FGameplayTag Key,
	const FPS_Value& Value,
	EPS_StateEventType& ChangeType,
	FPS_Value& OldValue,
	FPS_Value& NewValue)
{
	NewValue = Value;
	for (int i=0; i<State.Num(); i++)
	{
		if (State[i].Key == Key)
		{
			if (!State[i].Value.EqualTo(Value))
			{
				ChangeType = EPS_StateEventType::Change;
				OldValue = State[i].Value;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	ChangeType = EPS_StateEventType::Add;
	OldValue = Value;
	return true;
}

FString FPS_State::GetString(UWorld* InWorld, int StateOperationNum)
{
	FString Res = "";
	for (int i=State.Num()-1; i>=0; i--)
	{
		Res = Res + "\n" + State[i].GetString(InWorld);
	}

	if (StateOperationNum > 0)
	{
		Res =  Res + "\n\n ---------StateOperation---------";
		for (int i=0; i<StateOperationNum; i++)
		{
			int Index = -1;
			if (StateOperationIndexNow - i >= 0)
			{
				Index = StateOperationIndexNow - i;
			}
			else if (StateOperations.Num()>=StateOperationNum)
			{
				Index = StateOperationIndexNow - i + StateOperations.Num();
			}
			if (StateOperations.Num() <= Index || Index < 0)
			{
				break;
			}
			Res = Res + FString::Printf(TEXT("\n [%d] %s"), Index ,*StateOperations[Index].GetString(InWorld));
		}
		Res =  Res + "\n ---------StateOperation---------";
	}
	return Res;
}

FString FPE_Event::GetString(UWorld* InWorld)
{
	return ">>>>>>>>>PowerfulEvent Start ("+ Key.ToString() +")<<<<<<<<< : "
	+ EventState.GetString(InWorld,0)
	+ "\n>>>>>>>>>PowerfulEvent End ("+ Key.ToString() +")<<<<<<<<< : ";
}

FPS_Event::FPS_Event(EPS_StateEventType InStateEventType, FGameplayTag InStateKey, FPS_Value InOldValue,
	FPS_Value InNewValue)
{
	StateEventType = InStateEventType;
	StateKey = InStateKey;
	OldValue = InOldValue;
	NewValue = InNewValue;
}

FString FPS_Event::GetString()
{
	return ">>>>>>>>PS_Event Start<<<<<<<<<\n"
		+ (StaticEnum<EPS_StateEventType>()->GetNameStringByIndex( static_cast<int32>(StateEventType))) + "\n"
		+ "StateKey : " + StateKey.ToString() + "\n"
		+ "OldValue : " + OldValue.GetString() + "\n"
		+ "NewValue : " + NewValue.GetString() + "\n"
		+ ">>>>>>>>PS_Event End<<<<<<<<<";
}



// Sets default values for this component's properties
UCqsjPowerfulStateComponent::UCqsjPowerfulStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCqsjPowerfulStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


// Called when the game starts
void UCqsjPowerfulStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCqsjPowerfulStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCqsjPowerfulStateComponent::HasNetAuth() const
{
	return false;
}

void UCqsjPowerfulStateComponent::SetState_Imp(FGameplayTag Key, const FPS_Value& Value)
{
}

void UCqsjPowerfulStateComponent::SetMaxStateOperationNum(int InMaxStateOperationNum)
{
}

void UCqsjPowerfulStateComponent::SetState(FGameplayTag Key, FPS_Value Value)
{
}

void UCqsjPowerfulStateComponent::RemoveState(FGameplayTag Key)
{
}

void UCqsjPowerfulStateComponent::SetStateFloat(FGameplayTag Key, float Value)
{
}

void UCqsjPowerfulStateComponent::SetStateVector(FGameplayTag Key, FVector Value)
{
}

void UCqsjPowerfulStateComponent::SetStateTransform(FGameplayTag Key, FTransform Value)
{
}

void UCqsjPowerfulStateComponent::SetStateActor(FGameplayTag Key, AActor* Value)
{
}

void UCqsjPowerfulStateComponent::SetStateText(FGameplayTag Key, const FString Value)
{
}

void UCqsjPowerfulStateComponent::SetStateGT(FGameplayTag Key, FGameplayTag Value)
{
}

void UCqsjPowerfulStateComponent::SetStateBool(FGameplayTag Key, bool Value)
{
}

bool UCqsjPowerfulStateComponent::GetFValue(FGameplayTag Key, float& Result, float& Duration)
{
	return false;
}

bool UCqsjPowerfulStateComponent::GetVValue(FGameplayTag Key, FVector& Result, float& Duration)
{
	return false;
}

bool UCqsjPowerfulStateComponent::GetTValue(FGameplayTag Key, FTransform& Result, float& Duration)
{
	return false;
}

bool UCqsjPowerfulStateComponent::GetActorValue(FGameplayTag Key, AActor*& Result, float& Duration)
{
	return false;
}

bool UCqsjPowerfulStateComponent::GetTextValue(FGameplayTag Key, FString& Result, float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetGTValue(FGameplayTag Key, FGameplayTag& Result, float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetBoolValue(FGameplayTag Key, bool& Result, float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetFValue_History(FGameplayTag Key, int DistanceFromNow, float& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetVValue_History(FGameplayTag Key, int DistanceFromNow, FVector& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetTValue_History(FGameplayTag Key, int DistanceFromNow, FTransform& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetActorValue_History(FGameplayTag Key, int DistanceFromNow, AActor*& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetTextValue_History(FGameplayTag Key, int DistanceFromNow, FString& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetGTValue_History(FGameplayTag Key, int DistanceFromNow, FGameplayTag& Result,
	float& Duration)
{
	return false ;
}

bool UCqsjPowerfulStateComponent::GetBoolValue_History(FGameplayTag Key, int DistanceFromNow, bool& Result,
	float& Duration)
{
	return false ;
}

void UCqsjPowerfulStateComponent::SendEvent(FPE_Event PE_Event)
{
	
}

void UCqsjPowerfulStateComponent::SendEvent_Imp(const FPE_Event& PE_Event) const
{
}

void UCqsjPowerfulStateComponent::ActivePSDebug(FColor InDebugTextColor, bool InPrintToScreen, bool InPrintToLog)
{
}

void UCqsjPowerfulStateComponent::DeactivePSDebug()
{
}

void UCqsjPowerfulStateComponent::PSDebug()
{
}

void UCqsjPowerfulStateComponent::PEDebug(const FPE_Event& PE_Event) const
{
}

void UCqsjPowerfulStateComponent::DeactivePSDebug_Server_Implementation()
{
}

void UCqsjPowerfulStateComponent::ActivePSDebug_Server_Implementation(FColor InDebugTextColor, bool InPrintToScreen,
                                                                      bool InPrintToLog)
{
}

void UCqsjPowerfulStateComponent::SendEvent_Multicast_Implementation(FPE_Event PE_Event)
{
}

void UCqsjPowerfulStateComponent::SendEvent_Server_Implementation(FPE_Event PE_Event)
{
}

void UCqsjPowerfulStateComponent::SetStateBool_Server_Implementation(FGameplayTag Key, bool Value)
{
}

void UCqsjPowerfulStateComponent::SetStateGT_Server_Implementation(FGameplayTag Key, FGameplayTag Value)
{
}

void UCqsjPowerfulStateComponent::SetStateText_Server_Implementation(FGameplayTag Key, const FString& Value)
{
}

void UCqsjPowerfulStateComponent::SetStateActor_Server_Implementation(FGameplayTag Key, AActor* Value)
{
}

void UCqsjPowerfulStateComponent::SetStateTransform_Server_Implementation(FGameplayTag Key, FTransform Value)
{
}

void UCqsjPowerfulStateComponent::SetStateVector_Server_Implementation(FGameplayTag Key, FVector Value)
{
}

void UCqsjPowerfulStateComponent::SetStateFloat_Server_Implementation(FGameplayTag Key, float Value)
{
}

void UCqsjPowerfulStateComponent::RemoveState_Server_Implementation(FGameplayTag Key)
{
}

void UCqsjPowerfulStateComponent::SetState_Server_Implementation(FGameplayTag Key, FPS_Value Value)
{
}

void UCqsjPowerfulStateComponent::SetMaxStateOperationNum_Server_Implementation(int InMaxStateOperationNum)
{
}

