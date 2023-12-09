// Copyright GanBowen 2022-2023. All Rights Reserved.


#include "CqsjFlowMoveComponent.h"




#include "CqsjPSFuncLib.h"
#include "CqsjFlowMoveFuncLib.h"
#include "CqsjPSFuncLib.h"
#include "CqsjRMSBPFuncLib.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UCqsjFlowMoveComponent::UCqsjFlowMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCqsjFlowMoveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


// Called when the game starts
void UCqsjFlowMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//TaskState 是一个结构体
	TaskState.OwnerCharacter = Cast<ACharacter>(GetOwner());
	TaskState.Timer = 0.0f;
	TaskState.bIsActive =false ;
	TaskState.bIsStopping =false;
	TaskState.FlowMoveComponent =this ;

	if(TaskState.OwnerCharacter)
	{
		TArray<UActorComponent*> SKCArr; 
		TaskState.OwnerCharacter->GetComponents(USkeletalMeshComponent::StaticClass(),SKCArr);
		
		MovementCompMaxAccelerationCache = TaskState.OwnerCharacter->GetCharacterMovement()->GetMaxAcceleration();

		if(FlowMoveBrain)
		{
			SetFlowMoveViewMode(FlowMoveBrain->DefaultViewMode,true);
		}
		
		bIsConstrainToPlaneCache = TaskState.OwnerCharacter->GetCharacterMovement()->bConstrainToPlane;
		PlaneConstraintNormalCache = TaskState.OwnerCharacter->GetCharacterMovement()->GetPlaneConstraintNormal();
		PlaneConstraintOriginCache = TaskState.OwnerCharacter->GetCharacterMovement()->GetPlaneConstraintOrigin();
	
		bUseControllerRotationYawCache = TaskState.OwnerCharacter->bUseControllerRotationYaw;
	}
	
	if(IsInServer())
	{	
		TaskState.FlowMoveComponentGuid = FGuid::NewGuid();
		IsAIPlayer();

		PSStateComponent = UCqsjPSFuncLib :: GetPowerfulStateComponent(TaskState.OwnerCharacter);
		PSStateComponent->bIsDebug =bPS_IsDebug;
		PSStateComponent->StateOperationNum = PS_StateOperationNum;
		PSStateComponent->DebugTextColor = PS_DebugTextColor ;
		PSStateComponent->DebugTextColor2 = PS_DebugTextColor2;
		PSStateComponent->bPrintToScreen = bPS_PrintToScreen;
		PSStateComponent->bPrintToLog = bPS_PrintToLog ;

		if(FlowMoveBrain)
		{
			UCqsjAnimFuncLib::SetIncludeMainInstance(TaskState.OwnerCharacter,FlowMoveBrain->AnimStateGetType.IncludeMainAnimInstance);
			UCqsjAnimFuncLib::SetIncludeLinkedInstance(TaskState.OwnerCharacter,FlowMoveBrain->AnimStateGetType.IncludeLinkedAnimInstance);
			UCqsjAnimFuncLib::AddIncludeLinkedInstanceClass(TaskState.OwnerCharacter,FlowMoveBrain->AnimStateGetType.IncludeLinkedAnimInstanceClassSet);
		
		}
	}
	
}


// Called every frame
void UCqsjFlowMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float RealDeltaTime = DeltaTime;
	// ...

	if(FrameNumber == GFrameCounter)
		return ;

	FlowMoveTick(RealDeltaTime);

	TaskState.Timer = TaskState.Timer+RealDeltaTime;
	TaskState.FrameDeltaTime = RealDeltaTime;
	FrameNumber = GFrameCounter;
}

bool UCqsjFlowMoveComponent::GetRootMotionParamDirect(FTransform& Result, float SimulationTime, float MovementTickTime,
	const ACharacter& Character, const UCharacterMovementComponent& MovementComponent)
{
	return ICqsjRMSInterface::GetRootMotionParamDirect(Result, SimulationTime, MovementTickTime, Character,
	                                                   MovementComponent);
}


void UCqsjFlowMoveComponent::OnEvent(const FFlowMoveEvent& FlowMoveEvent)
{
	//根据不同的事件类型执行相应的逻辑 , 有点像 Run Event
	TaskState.AddFlowMoveEvent(FlowMoveEvent);
	if(FlowMoveBrain)
	{
		FlowMoveBrain->OnFMEvent(TaskState.OwnerCharacter,this,TaskState,FlowMoveEvent);

		if(FlowMoveEvent.EventType == EFlowMoveEventType::OnActionStart
			&&FlowMoveBrain->ActionEventKey_Start.IsValid()
			&&FlowMoveBrain->ActionEventValueKey.IsValid())
		{
			UCqsjPSFuncLib::SendPowerfulEvent(
				TaskState.OwnerCharacter,
				UCqsjPSFuncLib::AddGTToPE(
					UCqsjPSFuncLib::MakePE_Event(FlowMoveBrain->ActionEventKey_Start),
					FlowMoveBrain->ActionEventValueKey,
					FlowMoveEvent.ActionTag));
		}
		else if(FlowMoveEvent.EventType == EFlowMoveEventType::OnActionEnd
			&& FlowMoveBrain->ActionEventKey_End.IsValid()
			&& FlowMoveBrain->ActionEventValueKey.IsValid())
		{
			UCqsjPSFuncLib::SendPowerfulEvent(
				TaskState.OwnerCharacter,
				UCqsjPSFuncLib::AddGTToPE(
					UCqsjPSFuncLib::MakePE_Event(FlowMoveBrain->ActionEventKey_End),
					FlowMoveBrain->ActionEventValueKey,
					FlowMoveEvent.ActionTag));
		}
		else if(FlowMoveEvent.EventType == EFlowMoveEventType::OnActionUpdate
			&& FlowMoveBrain->ActionStateKey.IsValid() && IsInServer())
		{
			UCqsjPSFuncLib::SetPSGT(TaskState.OwnerCharacter,FlowMoveBrain->ActionStateKey,FlowMoveEvent.ActionTag);
		}
	}
}

void UCqsjFlowMoveComponent::SetInputValueByTargetPoint(FVector TargetPoint)
{
}

void UCqsjFlowMoveComponent::SetMoveVector(FVector NewMoveVector, float ZeroFaultToleranceDuration)
{
	if(IsLocalOwn())
	{
		if(!NewMoveVector.IsNearlyZero() ||
			(NewMoveVector.IsNearlyZero() && TaskState.OwnerCharacter->GetWorld()->GetRealTimeSeconds()- TaskState.LastMoveVectorTime >= ZeroFaultToleranceDuration))
		{
			TaskState.LastMoveVectorTime = TaskState.OwnerCharacter->GetWorld()->GetTimeSeconds();
			TaskState.LastMoveVector = NewMoveVector;

			if(NewMoveVector == TaskState.MoveVector)
			{
				return ;
			}

			TaskState.SetMoveVector( (NewMoveVector));
			SetMoveVector_Server(NewMoveVector);
		}
		
	}
}

FVector UCqsjFlowMoveComponent::GetMoveVector(bool Consumed)
{
	return FVector();
}

void UCqsjFlowMoveComponent::SetControlVector(FVector NewControlVector)
{
}

FVector UCqsjFlowMoveComponent::GetControlVector()
{
	return FVector();
}

void UCqsjFlowMoveComponent::SetPerceptionVector(FVector NewPerceptionVector)
{
	if(IsLocalOwn())
	{
		if(IsLocalOwn())
		{
			return ;
		}
		TaskState.PerceptionVector = NewPerceptionVector;
		SetPerceptionVector_Server(NewPerceptionVector);
	}
}

FVector UCqsjFlowMoveComponent::GetPerceptionVector()
{
	return FVector();
}

void UCqsjFlowMoveComponent::GetMoveParamForBlendSpace(UBlendSpace* ForBS, float& Direction, float& Speed,
	float& PlayRate) const
{
}

void UCqsjFlowMoveComponent::StopFlowMove(bool WaitForCurrentActionFinished)
{
}

void UCqsjFlowMoveComponent::Stop()
{
}

void UCqsjFlowMoveComponent::ActiveFlowMove()
{
	if(IsInServer() || IsLocalOwn())
	{
		Active_Server();
	}
}

void UCqsjFlowMoveComponent::Active_Imp()
{
	if(IsFlowMoveActive())
	{
		if(IsFlowMoveStopping())
		{
			if(IsInServer())
			{
				TaskState.bIsStopping =false ;
			}
			return ;
		}
		return ;
	}
	if(!FlowMoveBrain)
	{
		return ;
	}

	if(IsInServer())
	{
		if(!TaskState.FlowMoveComponentGuid.IsValid())
		{
			TaskState.FlowMoveComponentGuid = FGuid::NewGuid();
		}
	}
	TaskState.OwnerCharacter = Cast<ACharacter>(GetOwner());
	TaskState.Timer = 0.0f;
	TaskState.FlowMoveComponent = this ;

	//初始化Character的各种设置
	CapsuleRadiusCache = TaskState.OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	MeshRelativeLocationCache = TaskState.OwnerCharacter->GetMesh()->GetRelativeLocation();
	CapsuleHalfHeightCache = TaskState.OwnerCharacter ->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	bUseControllerRotationYawCache = TaskState.OwnerCharacter ->bUseControllerRotationYaw;

	if(TaskState.OwnerCharacter && FlowMoveBrain)
	{
		TaskState.OwnerCharacter->bUseControllerRotationYaw = false ;

		EnsureRMSActivation();

		MovementCompMaxAccelerationCache= TaskState.OwnerCharacter->GetCharacterMovement()->GetMaxAcceleration();
		TaskState.OwnerCharacter ->GetCharacterMovement()->MaxAcceleration =0.0f;

		TaskState.bIsActive =true ;
		TaskState.bIsStopping =false ;

		if(IsInServer())
		{
			for(const auto Item: FlowMoveBrain->FlowMovePerceptronSet)
			{
				if(Item.Value && Item.Value->bIsDefaultActive)
				{
					ActivePerception(Item.Key);
				}
			}

			for(const auto Item : FlowMoveBrain->FlowMoveScriptSet)
			{
				if(Item.Value && Item.Value->bIsDefaultActive)
				{
					ActiveScript(Item.Key);
				}
			}
		}

		UpdateMoveControlParam(TaskState.FrameDeltaTime);

		OnActiveEvent();
	}
	
}

void UCqsjFlowMoveComponent::SetMovementConstrain(const FVector& PlaneNormal, const FVector& PlaneOrigin)
{
}

void UCqsjFlowMoveComponent::ResetMovementConstrain()
{
}

void UCqsjFlowMoveComponent::ResetUseControllerRotationYaw()
{
}

void UCqsjFlowMoveComponent::NetInit()
{
}

void UCqsjFlowMoveComponent::SetRMSParamNow(FTransform NewRMSParam)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveViewMode(EFlowMoveCharacterViewMode NewViewMode, bool bAsDefaultViewMode)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveViewMode_Imp(EFlowMoveCharacterViewMode NewViewMode)
{
}

void UCqsjFlowMoveComponent::SetFocusActor(USceneComponent* InComponent, bool bSetViewModeAsActorLock)
{
}

void UCqsjFlowMoveComponent::SetFocusActor_Imp(USceneComponent* InComponent)
{
}

void UCqsjFlowMoveComponent::TryLockActor(FGameplayTag GetTargetFunctionTag)
{
}

bool UCqsjFlowMoveComponent::CheckSceneChange(const FCqsjFlowMoveScene& NewScene, FGameplayTag TargetScene,
	FCqsjFlowMoveSceneChangeInfo& ChangeInfo)
{
	return false;
}

void UCqsjFlowMoveComponent::SetFlowMoveScene(const FCqsjFlowMoveScene& NewScene, FGameplayTag TargetScene,
	bool bIsRemoveSupplementaryScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_Imp(const FCqsjFlowMoveScene& NewScene, FGameplayTag TargetScene,
	bool bIsRemoveSupplementaryScene)
{
}

void UCqsjFlowMoveComponent::ApplyActionLockStateSetting(bool bHasActionLock,
	const FCqsjFlowMoveActionLockStateSettings ActionLockStateSetting)
{
}

void UCqsjFlowMoveComponent::ApplyActionLockStateSetting_Imp(bool bHasActionLock,
	const FCqsjFlowMoveActionLockStateSettings& ActionLockStateSetting)
{
}

void UCqsjFlowMoveComponent::SetActionLockState(bool bHasActionLock, FGameplayTagQuery ActionToLock)
{
}

void UCqsjFlowMoveComponent::SetActionLockState_Imp(bool bHasActionLock, const FGameplayTagQuery& ActionToLock)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_SceneType_Imp(const FName SceneType, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_TargetActor_Imp(AActor* TargetActor, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_TargetLocation_Imp(const FCqsjFlowMovePointScene& TargetLocation,
	FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_ActorLocation_Imp(const FCqsjFlowMovePointScene& ActorLocation,
	FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_LeftLocation_Imp(const FCqsjFlowMovePointScene& Location,
	FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_RightLocation_Imp(const FCqsjFlowMovePointScene& Location,
	FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_Slope_Imp(const float Slope, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_PerceptionResult_Imp(const FCqsjPerceptionResult PerceptionResult,
	FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveLastAction(FGameplayTag NewLastActionTag)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveLastAction_Imp(FGameplayTag NewLastActionTag)
{
}

void UCqsjFlowMoveComponent::ActivePerception(const FGameplayTag PerceptronTag)
{
	if(IsInServer()||IsLocalOwn())
	{
		ActivePerception_Server(PerceptronTag);
	}
}

void UCqsjFlowMoveComponent::ActivePerception_Imp(const FGameplayTag PerceptionTag)
{
	TaskState.AddPerceptionTag(PerceptionTag);
}

void UCqsjFlowMoveComponent::DeactivePerception(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::DeactivePerceptron_Imp(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::ActiveScript(const FGameplayTag ScriptTag)
{
	if(IsInServer() || IsLocalOwn())
	{
		ActiveScript_Server(ScriptTag);
	}
}

void UCqsjFlowMoveComponent::ActiveScript_Imp(const FGameplayTag ScriptTag)
{
	TaskState.AddScriptTag(ScriptTag);
}

void UCqsjFlowMoveComponent::DeactiveScript(const FGameplayTag ScriptTag)
{
}

void UCqsjFlowMoveComponent::DeactiveScript_Imp(const FGameplayTag ScriptTag)
{
}

void UCqsjFlowMoveComponent::SetMoveControl(const FCqsjFlowMoveControlSettings MoveControlSettings)
{
}

void UCqsjFlowMoveComponent::SetMoveControl_Imp(const FCqsjFlowMoveControlSettings& MoveControlSettings)
{
}

void UCqsjFlowMoveComponent::DeactiveMoveControl()
{
}

void UCqsjFlowMoveComponent::DeactiveMoveControl_Imp()
{
}

void UCqsjFlowMoveComponent::ResetMoveControl()
{
}

void UCqsjFlowMoveComponent::ResetMoveControl_Imp()
{
}

FCqsjFlowMoveControlParam UCqsjFlowMoveComponent::GetMoveControlParam() const
{
	return FCqsjFlowMoveControlParam();
}

FVector UCqsjFlowMoveComponent::GetMoveToVector(float DeltaTime, bool bUseDefaultMoveVectorWhenZero,
	const FVector& DefaultMoveVector)
{
	return FVector();
}

bool UCqsjFlowMoveComponent::IsActionNow() const
{
	return false;
}

UCqsjFlowMoveBrain_Base* UCqsjFlowMoveComponent::GetFlowMoveBrain()
{
	return FlowMoveBrain;
}

FVector UCqsjFlowMoveComponent::GetDirectionVector(EFlowMoveDirectionType Type)
{
	return FVector();
}

bool UCqsjFlowMoveComponent::GetScene(FCqsjFlowMoveScene& Scene, FGameplayTag SceneSlot, bool bIsConsume)
{
	return false;
}

bool UCqsjFlowMoveComponent::IsActionLock(FGameplayTagContainer ActionToLock) const
{
	return false;
}

bool UCqsjFlowMoveComponent::IsAnyActionLock() const
{
	return false;
}

void UCqsjFlowMoveComponent::SetCurrentTargetScene(FGameplayTag ActionTargetScene)
{
}

void UCqsjFlowMoveComponent::SetCurrentActionTag(FGameplayTag ActionTag)
{
}

FVector UCqsjFlowMoveComponent::GetActionTargetPoint(ECqsjFlowMoveScenePointType ActionTargetPoint)
{
	return FVector();
}

bool UCqsjFlowMoveComponent::IsLocalPlayer(bool bIsReset)
{
	return false;
}

bool UCqsjFlowMoveComponent::IsAIPlayer(bool bIsReset)
{
	if(!TaskState.OwnerCharacter)
	{
		return false ; 
	}

	if(bIsReset)
	{
		if(IsInServer())
		{
			bIsAIPlayer = UCqsjFlowMoveFuncLib::IsAIPlayer(TaskState.OwnerCharacter);
			bIsAIPlayerInit =true;
		}
	}

	return bIsAIPlayer;
}

bool UCqsjFlowMoveComponent::IsLocalOwn(bool bIsReset)
{
	if(!TaskState.OwnerCharacter)
	{
		return false ;
	}

	if(bIsLocalOwnInit || bIsReset)
	{
		//这里转到了FuncLib去初始化
		bIsLocalOwn = UCqsjFlowMoveFuncLib::IsLocalOwn(TaskState.OwnerCharacter);
		bIsLocalOwnInit =true ;
	}
	return bIsLocalOwn;
}

bool UCqsjFlowMoveComponent::IsInServer(bool bIsReset)
{
	if(!bIsInServerInit || bIsReset)
	{
		bIsInServer = IsValid(TaskState.OwnerCharacter) && TaskState.OwnerCharacter->HasAuthority();
		bIsInServerInit =true ;
	}
	return bIsInServer;
}

bool UCqsjFlowMoveComponent::IsFlowMoveActive() const
{
	return TaskState.OwnerCharacter && TaskState.bIsActive;
}

bool UCqsjFlowMoveComponent::IsFlowMoveStopping() const
{
	return TaskState.bIsStopping;
}

AActor* UCqsjFlowMoveComponent::GetNetWorkActor(FGuid ActorGuid)
{
	return nullptr;
}

void UCqsjFlowMoveComponent::SpawnNetWorkActor(FGuid ActorGuid, TSubclassOf<AActor> ActorClass, FTransform Transform)
{
}

void UCqsjFlowMoveComponent::SpawnNetWorkActor_Imp(FGuid ActorGuid, TSubclassOf<AActor> ActorClass,
	FTransform Transform)
{
}

void UCqsjFlowMoveComponent::DestroyNetWorkActor(FGuid ActorGuid)
{
}

void UCqsjFlowMoveComponent::DestroyNetWorkActor_Imp(FGuid ActorGuid)
{
}

void UCqsjFlowMoveComponent::SetNetWorkActorLocation(FGuid ActorGuid, FVector NewLocation)
{
}

void UCqsjFlowMoveComponent::SetNetWorkActorLocation_Imp(FGuid ActorGuid, FVector NewLocation)
{
}

void UCqsjFlowMoveComponent::ResetCapsuleAndMesh()
{
}

void UCqsjFlowMoveComponent::ResetCapsuleAndMesh_Imp() const
{
}

void UCqsjFlowMoveComponent::SetCapsuleAndKeepMesh(float NewHeight)
{
}

void UCqsjFlowMoveComponent::SetCapsuleAndKeepMesh_Imp(float TargetRadius, float TargetHalfHeight,
	float TargetMeshRlz) const
{
}

void UCqsjFlowMoveComponent::AdjustCapsuleAndKeepMeshByTargetLocation(FCqsjFlowMoveScene FlowMoveScene)
{
}

void UCqsjFlowMoveComponent::AdjustCapsuleAndKeepMeshByCurrentLocation(FCqsjFlowMoveScene FlowMoveScene)
{
}

float UCqsjFlowMoveComponent::GetActorScale() const
{
	return 0.0f;
}

void UCqsjFlowMoveComponent::PostureAdjust(float DeltaTime) const
{
}

void UCqsjFlowMoveComponent::RecoveryPosture() const
{
}

void UCqsjFlowMoveComponent::MovingAdjust()
{
}

void UCqsjFlowMoveComponent::FlowMoveTick(float DeltaTime)
{
	TaskState.ClearFlowMoveEvent();

	if(!CheckUpdateValid())
	{
		return ;
	}
	//施工标识符
	CheckActive();

	CheckInput();

	CheckViewMode();

	UpdateCharacterState(DeltaTime);

	UpdateScript(DeltaTime);

	UpdateScene(DeltaTime);

	CheckStop();

	//OnUpdate.Broadcast(TaskState);
	//OnEvent(FFlowMoveEvent(EFlowMoveEventType::OnUpdate));

	UpdateCurrentAction(DeltaTime);

	UpdateMoveControlParam(DeltaTime);

	UpdateRMS(DeltaTime);
}

void UCqsjFlowMoveComponent::CheckActive()
{
	if(IsInServer())
	{
		bool IsActive =false;
		bool IsWaitForCurrentActionFinished =true ;
		if(FlowMoveBrain)
		{
			FlowMoveBrain ->GetFMIsActive(
				TaskState.OwnerCharacter,
				this,
				TaskState.FrameDeltaTime,
				TaskState,
				IsActive,
				IsWaitForCurrentActionFinished
			);
		}

		if(IsActive && (!IsFlowMoveActive()||IsFlowMoveStopping()))
		{
			ActiveFlowMove();
		}
		else if(!IsActive && IsFlowMoveActive())
		{
			StopFlowMove(IsWaitForCurrentActionFinished);
		}
	}
}

void UCqsjFlowMoveComponent::CheckStop()
{
}

void UCqsjFlowMoveComponent::CheckInput(bool bReset)
{
	if(IsLocalPlayer() && FlowMoveBrain)
	{
		FVector MoveVector ;
		FVector ControlVector;
		bool IsMoveInput ;
		bool IsControlInput;

		FlowMoveBrain-> GetFmMoveVector(
			TaskState.OwnerCharacter,
			this,
			TaskState.FrameDeltaTime,
			TaskState,
			IsMoveInput,
			MoveVector);

		FlowMoveBrain->GetFMControlVector(
			TaskState.OwnerCharacter,
			this,
			TaskState.FrameDeltaTime,
			TaskState,
			IsControlInput,
			ControlVector);

		  if(IsMoveInput)
		  {
			  SetMoveVector(
			  	MoveVector,
			  	FlowMoveBrain->MoveVectorZeroFaultToleranceDuration);//移动矢量零容错持续时间
		  }
		if(IsControlInput)
		{
			SetControlVector(ControlVector);
		}

		const FTransform TraceT = TaskState.OwnerCharacter->GetTransform();
		FVector Direction = UKismetMathLibrary::ProjectVectorOnToPlane(TaskState.ControlVector,TaskState.OwnerCharacter->GetActorForwardVector());
		FVector DirectionResult =UKismetMathLibrary::InverseTransformDirection(TraceT,Direction);
		//这个Direction看的是上下左右的转角（去除了前后的关系）
		if(const float Pitch = Direction.Z * 90.0f; -FlowMoveBrain->PerceptionInputSettings.DownAngleThreshold<Pitch && Pitch< FlowMoveBrain->PerceptionInputSettings.UpAngleThreshold)
		{
			DirectionResult.Z =0 ;
		}
		if(const float Yaw = DirectionResult.Y * 90.0f ; -FlowMoveBrain->PerceptionInputSettings.LeftAndRightAngleThreshold<Yaw && Yaw < FlowMoveBrain->PerceptionInputSettings.LeftAndRightAngleThreshold)
		{
			DirectionResult.Y = 0 ;
		}
		DirectionResult.X =0;
		Direction .Normalize();
		const FVector PerceptionVector = DirectionResult;
		SetPerceptionVector(PerceptionVector);
	}

	
}

void UCqsjFlowMoveComponent::CheckViewMode()
{
}

bool UCqsjFlowMoveComponent::CheckUpdateValid()
{
	if(!TaskState.OwnerCharacter)
	{
		TaskState.OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
	if(!TaskState.OwnerCharacter)
	{
		return false ;
	}
	if(!IsValid(FlowMoveBrain))
	{
		return false ;
	}
	if(!bIsNetInit)
	{
		NetInit();
	}
	return bIsNetInit ;
}

void UCqsjFlowMoveComponent::EnsureRMSActivation()
{
	if(!UCqsjRMSBPFuncLib::CqsjRms_IsRmsCommonActive(TaskState.OwnerCharacter->GetCharacterMovement()))
	{
		FName RMSInsName;
		UCqsjRMSBPFuncLib::CqsjRMS_Common(
						RMSInsName,
						this,
						TaskState.OwnerCharacter->GetCharacterMovement(),
						true);
	}
	FRootMotionSourceGroup RmsArr = TaskState.OwnerCharacter->GetCharacterMovement()->CurrentRootMotion;
	for(const auto Rms:RmsArr.RootMotionSources)
	{
		if(Rms->InstanceName != UCqsjRMSBPFuncLib::CqsjRms_GetRmsCommonInstanceName())
		{
			TaskState.OwnerCharacter->GetCharacterMovement()->RemoveRootMotionSource(Rms->InstanceName);
		}
	}
	
}

void UCqsjFlowMoveComponent::UpdateCharacterState(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::UpdateScript(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::UpdateScene(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::UpdateMoveControlParam(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::UpdateRMS(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::UpdateCurrentAction(float DeltaTime)
{
}

void UCqsjFlowMoveComponent::OnActiveEvent()
{
	OnActive.Broadcast(TaskState);
	OnEvent(FFlowMoveEvent(EFlowMoveEventType::OnActive));
}

void UCqsjFlowMoveComponent::OnStopEvent()
{
}

void UCqsjFlowMoveComponent::OnActionEndEvent(FGameplayTag ActionTag)
{
}

bool UCqsjFlowMoveComponent::IsLocked(FGameplayTag ForActionTag) const
{
	return false ;
}

void UCqsjFlowMoveComponent::Recovery()
{
}

void UCqsjFlowMoveComponent::UpdateScene_Multicast_Implementation(float DeltaTime, FGameplayTag PerceptionKey,
                                                                  ECqsjFlowMoveEventType EventType)
{
}

void UCqsjFlowMoveComponent::UpdateScene_Server_Implementation(float DeltaTime, FGameplayTag PerceptionKey,
                                                               ECqsjFlowMoveEventType EventType)
{
}

void UCqsjFlowMoveComponent::UpdateScript_Multicast_Implementation(float DeltaTime, FGameplayTag ScriptKey,
                                                                   ECqsjFlowMoveEventType EventType)
{
}

void UCqsjFlowMoveComponent::UpdateScript_Server_Implementation(float DeltaTime, FGameplayTag ScriptKey,
                                                                ECqsjFlowMoveEventType EventType)
{
}

void UCqsjFlowMoveComponent::SetCapsuleAndKeepMesh_Multicast_Implementation(float TargetRadius, float TargetHalfHeight,
                                                                            float TargetMeshRlz)
{
}

void UCqsjFlowMoveComponent::SetCapsuleAndKeepMesh_Server_Implementation(float TargetRadius, float TargetHalfHeight,
                                                                         float TargetMeshRlz)
{
}

void UCqsjFlowMoveComponent::ResetCapsuleAndMesh_Multicast_Implementation()
{
}

void UCqsjFlowMoveComponent::ResetCapsuleAndMesh_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::SetNetWorkActorLocation_Server_Implementation(FGuid ActorGuid, FVector NewLocation)
{
}

void UCqsjFlowMoveComponent::DestroyNetWorkActor_Server_Implementation(FGuid ActorGuid)
{
}

void UCqsjFlowMoveComponent::SpawnNetWorkActor_Server_Implementation(FGuid ActorGuid, TSubclassOf<AActor> ActorClass,
                                                                     FTransform Transform)
{
}

void UCqsjFlowMoveComponent::SetCurrentActionTag_Server_Implementation(FGameplayTag ActionTag)
{
}

void UCqsjFlowMoveComponent::SetCurrentTargetScene_Server_Implementation(FGameplayTag ActionTargetScene)
{
}

void UCqsjFlowMoveComponent::ResetMoveControl_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::DeactiveMoveControl_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::SetMoveControl_Server_Implementation(
	const FCqsjFlowMoveControlSettings MoveControlSettings)
{
}

void UCqsjFlowMoveComponent::DeactiveScript_Server_Implementation(const FGameplayTag ScriptTag)
{
}

void UCqsjFlowMoveComponent::ActiveScript_Server_Implementation(const FGameplayTag ScriptTag)
{
	ActiveScript_Imp(ScriptTag);
}

void UCqsjFlowMoveComponent::DeactivePerception_Server_Implementation(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::ActivePerception_Server_Implementation(const FGameplayTag PerceptionTag)
{
	ActivePerception_Imp(PerceptionTag);
}

void UCqsjFlowMoveComponent::SetFlowMoveLastAction_Server_Implementation(FGameplayTag NewLastActionTag)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_PerceptionResult_Server_Implementation(
	FCqsjPerceptionResult PerceptionResult, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_Slope_Server_Implementation(float Slope, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_RightLocation_Server_Implementation(FCqsjFlowMovePointScene Location,
                                                                                  FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_LeftLocation_Server_Implementation(FCqsjFlowMovePointScene Location,
                                                                                 FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_ActorLocation_Server_Implementation(FCqsjFlowMovePointScene ActorLocation,
                                                                                  FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_TargetLocation_Server_Implementation(
	FCqsjFlowMovePointScene TargetLocation, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_TargetActor_Server_Implementation(AActor* TargetActor,
                                                                                FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_SceneType_Server_Implementation(FName SceneType, FGameplayTag TargetScene)
{
}

void UCqsjFlowMoveComponent::SetActionLockState_Server_Implementation(bool bHasActionLock,
                                                                      FGameplayTagQuery ActionToLock)
{
}

void UCqsjFlowMoveComponent::ApplyActionLockStateSetting_Server_Implementation(bool bHasActionLock,
                                                                               const FCqsjFlowMoveActionLockStateSettings ActionLockStateSetting)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveScene_Server_Implementation(FCqsjFlowMoveScene NewScene,
                                                                    FGameplayTag TargetScene, bool bIsRemoveSupplementaryScene)
{
}

void UCqsjFlowMoveComponent::SetFocusActor_Multicast_Implementation(USceneComponent* InComponent)
{
}

void UCqsjFlowMoveComponent::SetFocusActor_Server_Implementation(USceneComponent* InComponent)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveViewMode_Multicast_Implementation(EFlowMoveCharacterViewMode NewViewMode)
{
}

void UCqsjFlowMoveComponent::SetFlowMoveViewMode_Server_Implementation(EFlowMoveCharacterViewMode NewViewMode,
                                                                       bool bAsDefaultViewMode)
{
}

void UCqsjFlowMoveComponent::SetRMSParamNow_Multicast_Implementation(FTransform NewRMSParam)
{
}

void UCqsjFlowMoveComponent::SetRMSParamNow_Server_Implementation(FTransform NewRMSParam)
{
}

void UCqsjFlowMoveComponent::ResetUseControllerRotationYaw_Multicast_Implementation()
{
}

void UCqsjFlowMoveComponent::ResetUseControllerRotationYaw_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::ResetMovementConstrain_Multicast_Implementation()
{
}

void UCqsjFlowMoveComponent::ResetMovementConstrain_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::SetMovementConstrain_Multicast_Implementation(FVector PlaneNormal, FVector PlaneOrigin)
{
}

void UCqsjFlowMoveComponent::SetMovementConstrain_Server_Implementation(FVector PlaneNormal, FVector PlaneOrigin)
{
}

void UCqsjFlowMoveComponent::SetActive_Multicast_Implementation(bool NewActive)
{
}

void UCqsjFlowMoveComponent::SetActive_Server_Implementation(bool NewActive)
{
}

void UCqsjFlowMoveComponent::Active_Multicast_Implementation()
{
	if(TaskState.OwnerCharacter)
	{
		Active_Imp();
	}
}

void UCqsjFlowMoveComponent::Active_Server_Implementation()
{
	
	Active_Multicast();
}

void UCqsjFlowMoveComponent::Stop_Multicast_Implementation()
{
}

void UCqsjFlowMoveComponent::Stop_Server_Implementation()
{
}

void UCqsjFlowMoveComponent::StopFlowMove_Multicast_Implementation(bool WaitForCurrentActionFinished)
{
}

void UCqsjFlowMoveComponent::StopFlowMove_Server_Implementation(bool WaitForCurrentActionFinished)
{
}

void UCqsjFlowMoveComponent::SetPerceptionVector_Server_Implementation(FVector NewPerceptionVector)
{
	TaskState.PerceptionVector = NewPerceptionVector ;
}

void UCqsjFlowMoveComponent::SetControlVector_Server_Implementation(FVector ControlVector)
{
}

void UCqsjFlowMoveComponent::SetMoveVector_Server_Implementation(FVector NewMoveVector)
{
}

void UCqsjFlowMoveComponent::SetInputValueByTargetPoint_Server_Implementation(FVector TargetPoint)
{
}

