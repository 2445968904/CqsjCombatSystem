// Copyright GanBowen 2022-2023. All Rights Reserved.


#include "CqsjFlowMoveComponent.h"


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
	
}


// Called every frame
void UCqsjFlowMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCqsjFlowMoveComponent::GetRootMotionParamDirect(FTransform& Result, float SimulationTime, float MovementTickTime,
	const ACharacter& Character, const UCharacterMovementComponent& MovementComponent)
{
	return ICqsjRMSInterface::GetRootMotionParamDirect(Result, SimulationTime, MovementTickTime, Character,
	                                                   MovementComponent);
}


void UCqsjFlowMoveComponent::OnEvent(const FFlowMoveEvent& FlowMoveEvent)
{
}

void UCqsjFlowMoveComponent::SetInputValueByTargetPoint(FVector TargetPoint)
{
}

void UCqsjFlowMoveComponent::SetMoveVector(FVector NewMoveVector, float ZeroFaultToleranceDuration)
{
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
}

void UCqsjFlowMoveComponent::Active_Imp()
{
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

void UCqsjFlowMoveComponent::ActivePerceptron(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::ActivePerceptron_Imp(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::DeactivePerceptron(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::DeactivePerceptron_Imp(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::ActiveScript(const FGameplayTag ScriptTag)
{
}

void UCqsjFlowMoveComponent::ActiveScript_Imp(const FGameplayTag ScriptTag)
{
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
	return false;
}

bool UCqsjFlowMoveComponent::IsLocalOwn(bool bIsReset)
{
	return false;
}

bool UCqsjFlowMoveComponent::IsInServer(bool bIsReset)
{
	return false;
}

bool UCqsjFlowMoveComponent::IsFlowMoveActive() const
{
	return false;
}

bool UCqsjFlowMoveComponent::IsFlowMoveStopping() const
{
	return false;
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
}

void UCqsjFlowMoveComponent::CheckActive()
{
}

void UCqsjFlowMoveComponent::CheckStop()
{
}

void UCqsjFlowMoveComponent::CheckInput(bool bReset)
{
}

void UCqsjFlowMoveComponent::CheckViewMode()
{
}

bool UCqsjFlowMoveComponent::CheckUpdateValid()
{
	return false ;
}

void UCqsjFlowMoveComponent::EnsureRMSActivation()
{
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
}

void UCqsjFlowMoveComponent::DeactivePerceptron_Server_Implementation(const FGameplayTag PerceptronTag)
{
}

void UCqsjFlowMoveComponent::ActivePerceptron_Server_Implementation(const FGameplayTag PerceptronTag)
{
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
}

void UCqsjFlowMoveComponent::Active_Server_Implementation()
{
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

