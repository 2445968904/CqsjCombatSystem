// Copyright GanBowen 2022-2023. All Rights Reserved.


#include "CqsjFlowMoveFuncLib.h"

#include "Engine/SkeletalMeshEditorData.h"
#include "Kismet/GameplayStatics.h"

bool UCqsjFlowMoveFuncLib::GetSKMeshByTag(USkeletalMeshComponent*& SKMeshComp, ACharacter* Character, FName MeshTag,
                                          TArray<FName> ExcludedMeshTag, UAnimationAsset* TargetAnimationAsset)
{
	SKMeshComp = nullptr;
	if(Character && TargetAnimationAsset)
	{
		auto SKMeshes = Character->GetComponentsByTag(USkeletalMeshComponent::StaticClass(),MeshTag);
		if(SKMeshes.IsEmpty())
		{
			Character->GetComponents(USkeletalMeshComponent::StaticClass(),SKMeshes);
		}

		for(const auto SkMeshItem:SKMeshes)
		{
			bool bIsExcluded = false;
			for(const auto ETag:ExcludedMeshTag)
			{
				if(SkMeshItem->ComponentHasTag(ETag))
				{
					bIsExcluded=true;
					break;
				}
			}

			if(!bIsExcluded)
			{
				USkeletalMeshComponent * SKMC =Cast<USkeletalMeshComponent>(SkMeshItem);
				USkeletalMesh* SKMS = nullptr;
				if(SKMC)
				{
#if ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION >0
					SKMS= SKMC->GetSkeletalMeshAsset();
#else
					SKMS=SKMC->SkeletalMesh;
#endif
					
				}
#if ENGINE_MAJOR_VERSION==5 && ENGINE_MINOR_VERSION>0
#else
				if(SKMS->GetSkeleton()->IsCompatible(TargetAnimationAsset->GetSkeleton()))
#endif
				{
					if(SKMS->GetSkeleton()->IsCompatible(TargetAnimationAsset->GetSkeleton()))
					{
						SKMeshComp =SKMC;
						return true;
					}
				}
				//
			}
		}
	}
	return false;
}

bool UCqsjFlowMoveFuncLib::IsLocalOwn(AActor* Actor)
{
	bool bRes;

	if(Actor)
	{
		const ENetRole LocalRole =Actor -> GetLocalRole();
		const ENetRole RemoteRole = Actor->GetRemoteRole();
		//如果说本地是权威的(这个函数就是服务器的local)
		if(LocalRole == ENetRole::ROLE_Authority)
		{

			bRes = true;
			
		}
		//这个用于判断这个actor是不是玩家本地拥有
		else
		{
			if(LocalRole == ENetRole::ROLE_AutonomousProxy)
			{
				bRes = true;
			}
			else
			{
				bRes = false;
			}
		}
	}
	else
	{
		bRes=false;
	}
	return bRes;
}

bool UCqsjFlowMoveFuncLib::IsAIPlayer(AActor* Actor)
{
	bool bRes;

	if(Actor)
	{
		const ENetRole LocalRole = Actor ->GetLocalRole();
		const ENetRole RemoteRole = Actor->GetRemoteRole();
	//在服务器上是权威,客户端没有代理权,且不是玩家的角色 就可以判断这是AI了
		bRes =LocalRole == ENetRole::ROLE_Authority
		&& RemoteRole != ENetRole::ROLE_AutonomousProxy
		&& Actor != UGameplayStatics::GetPlayerCharacter(Actor,0);
	}
	else
	{
		bRes = false ;
	}
	
	return bRes;
}
//执行多行扫描
FHitResult UCqsjFlowMoveFuncLib::SweepMulti(const UWorld* World, TArray<FHitResult>& OutHits, const FVector& Start,
	const FVector& End, const FQuat& Rot, FCqsjFlowMoveTraceSetting TraceSetting, const FCollisionShape& CollisionShape,
	const FCollisionQueryParams& Params)
{
	FHitResult Res = FHitResult();
	if(!World)
	{
		return Res;
	}
	if(TraceSetting.TraceObjectTypes.IsEmpty() && TraceSetting.TraceChannels.IsEmpty())
	{
		return Res;
	}
	//以上都表示没有有效的设置
	TArray <FHitResult> ObjectTypeOutHits;
	FCollisionObjectQueryParams ObjectQueryParams;
	for(const auto ObjectType : TraceSetting.TraceObjectTypes)
	{
		ObjectQueryParams.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false,ObjectType));
	}

	World->SweepMultiByObjectType(
		ObjectTypeOutHits,
		Start,
		End,
		Rot,
		ObjectQueryParams,
		CollisionShape,
		Params
	);
	
	TArray<FHitResult> ChannelOutHits;
	for(const auto Channel:TraceSetting.TraceChannels)
	{
		TArray<FHitResult> TempChannelOutHits;
		World->SweepMultiByChannel(
			ChannelOutHits,
			Start,
			End,
			Rot,
			UEngineTypes::ConvertToCollisionChannel(Channel),
			CollisionShape,
			Params
		);
		TempChannelOutHits.Append(TempChannelOutHits);
	}
	//确保不会重复添加一个对象到数组里去
	TArray<AActor*> ResultOutHits;
	OutHits.Empty();
	for(auto Item : ObjectTypeOutHits)
	{
		if(!ResultOutHits.Contains(Item.GetActor()))
		{
			OutHits.Add(Item);
			ResultOutHits.Add(Item.GetActor());
		}
	}

	for(auto Item : ChannelOutHits)
	{
		if(Item.IsValidBlockingHit()&&!ResultOutHits.Contains(Item.GetActor()))
		{
			OutHits.Add(Item);
			ResultOutHits.Add(Item.GetActor());
		}
	}

	if(TraceSetting.isDebug && TraceSetting.DebugTime>=0)
	{
		if(TraceSetting.DrawTraceShape)
		{
			DrawDebugShape(World,Start,FQuat::Identity,CollisionShape,
				!OutHits.IsEmpty()? FColor::Red : FColor::White,
				TraceSetting.DebugTime,1);
			DrawDebugShape(World,End,FQuat::Identity,CollisionShape,
				!OutHits.IsEmpty()?FColor::Red : FColor::White,
				TraceSetting.DebugTime,1);
		}

		if(!OutHits.IsEmpty())
		{
			DrawDebugSphere(World,OutHits[0].ImpactPoint,5,8,
				FColor::Green,
				false,TraceSetting.DebugTime,0,10);
		}

		if(TraceSetting.DrawTraceLine)
		{
			DrawDebugLine(World,Start,End,
							!OutHits.IsEmpty()? FColor::Red : FColor::White,
							false , TraceSetting.DebugTime,0,10);
		}
	}

	if(!OutHits.IsEmpty())
	{
		Res = OutHits [0];
	}
	else
	{
		Res.TraceStart = Start;
		Res.TraceEnd = End;
	}
	for(auto Hit : OutHits)
	{
		if(Hit.IsValidBlockingHit())
		{
			Res = Hit ;
			break;
		}
	}

	if(Res.GetActor() && !TraceSetting.IsMetScreeningConditions(Res.GetActor()))
	{
		Res = FHitResult();
	}
	return Res;
}

bool UCqsjFlowMoveFuncLib::OverlapAnyTest(const UWorld* World, const FVector& Pos, const FQuat& Rot,
	FCqsjFlowMoveTraceSetting TraceSetting, const FCollisionShape& CollisionShape, const FCollisionQueryParams& Params)
{
	if(!World)
	{
		return false;
	}
	if(TraceSetting.TraceObjectTypes.IsEmpty() && TraceSetting.TraceChannels.IsEmpty())
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for(const auto ObjectType:TraceSetting.TraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false,ObjectType));
	}
	if(World->OverlapAnyTestByObjectType(
		Pos,
		Rot,
		ObjectQueryParameters,
		CollisionShape,
		Params))
	{
		return true;
	}

	for(auto Item : TraceSetting.TraceChannels)
	{
		if(World->OverlapAnyTestByObjectType(
			Pos,
			Rot,
			UEngineTypes::ConvertToCollisionChannel(Item),
			CollisionShape,
			Params))
		{
			return true;
		}
	}
	return false;
}

FHitResult UCqsjFlowMoveFuncLib::LineTrace(const UWorld* World, TArray<FHitResult>& OutHits, const FVector& Start,
	const FVector& End, FCqsjFlowMoveTraceSetting TraceSetting, const FCollisionQueryParams& Params)
{
	return FHitResult();
}

bool UCqsjFlowMoveFuncLib::GetTheLastValidHitResult(TArray<FHitResult> HitResultArr, FHitResult& Result)
{
	return true;
}

void UCqsjFlowMoveFuncLib::FindFloorAndRoof(ACharacter* Character, FVector FromLocation, float FloorTraceDistance,
	float RoofTraceDistance, bool bFindLedge, float LedgeTraceDistance, FCqsjFlowMoveTraceSetting TraceSetting,
	bool& Floor, FHitResult& OutHit_Floor, bool& Roof, FHitResult& OutHit_Roof,
	FCqsjFlowMoveFloorRoofScene& FloorAndRoofRes)
{
}

void UCqsjFlowMoveFuncLib::FindBlockWall(const ACharacter* Character, FVector ForwardVector, float WallTraceDistance,
	bool bFindLedge, float LedgeTraceDistance, FCqsjFlowMoveTraceSetting TraceSetting,
	FCqsjFlowMoveBlockWallScene& WallRes)
{
}

void UCqsjFlowMoveFuncLib::AddBool(FCqsjPerceptionResult PerceptionResult, FName Key, bool Value)
{
}

void UCqsjFlowMoveFuncLib::AddFloat(FCqsjPerceptionResult PerceptionResult, FName Key, float Value)
{
}

void UCqsjFlowMoveFuncLib::AddVector(FCqsjPerceptionResult PerceptionResult, FName Key, FVector Value)
{
}

void UCqsjFlowMoveFuncLib::AddTransform(FCqsjPerceptionResult PerceptionResult, FName Key, FTransform Value)
{
}

void UCqsjFlowMoveFuncLib::AddString(FCqsjPerceptionResult PerceptionResult, FName Key, FString Value)
{
}

void UCqsjFlowMoveFuncLib::AddGameplayTag(FCqsjPerceptionResult PerceptionResult, FName Key, FGameplayTag Value)
{
}

void UCqsjFlowMoveFuncLib::AddActor(FCqsjPerceptionResult PerceptionResult, FName Key, AActor* Value)
{
}

void UCqsjFlowMoveFuncLib::AddObject(FCqsjPerceptionResult PerceptionResult, FName Key, UObject* Value)
{
}

void UCqsjFlowMoveFuncLib::AddMovementMode(FCqsjPerceptionResult PerceptionResult, FName Key, EMovementMode Value)
{
}

void UCqsjFlowMoveFuncLib::GetBool(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, bool& Value)
{
}

void UCqsjFlowMoveFuncLib::GetFloat(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, float& Value)
{
}

void UCqsjFlowMoveFuncLib::GetVector(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, FVector& Value)
{
	
}

void UCqsjFlowMoveFuncLib::GetTransform(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet,
	FTransform& Value)
{
}

void UCqsjFlowMoveFuncLib::GetString(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, FString& Value)
{
}

void UCqsjFlowMoveFuncLib::GetGameplayTag(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet,
	FGameplayTag& Value)
{
}

void UCqsjFlowMoveFuncLib::GetActor(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, AActor*& Value)
{
}

void UCqsjFlowMoveFuncLib::GetObject(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet, UObject*& Value)
{
}

void UCqsjFlowMoveFuncLib::GetMovementMode(FCqsjPerceptionResult PerceptionResult, FName Key, bool& IsGet,
	TEnumAsByte<EMovementMode>& Value)
{
}

FCqsjFlowMovePointScene UCqsjFlowMoveFuncLib::MakeFlowMovePointScene(ACharacter* InCharacter, const FVector& InPoint,
	bool bFindLedge, float LedgeTraceDistance, const FCqsjFlowMoveTraceSetting& TraceSetting)
{
	return FCqsjFlowMovePointScene();
}

FVector UCqsjFlowMoveFuncLib::RadianToDirectionXY(float Radian)
{
	return FVector();
}

FVector UCqsjFlowMoveFuncLib::AngleToDirectionXY(float Angle)
{
	return FVector();
}

float UCqsjFlowMoveFuncLib::GetVelocityYawAngle(FVector Velocity)
{
	return 0.0f;
}

float UCqsjFlowMoveFuncLib::GetInputYawAngle(FVector InputDirection)
{
	return 0.0f;
}

float UCqsjFlowMoveFuncLib::GetPointToLineDistance(FVector LineStart, FVector LineEnd, FVector Point)
{
	return 0.0f;
}

FVector UCqsjFlowMoveFuncLib::GetPointToLineFoot(const FVector& LineStart, const FVector& LineEnd, const FVector& Point)
{
	return FVector();
}

float UCqsjFlowMoveFuncLib::GetMoveDirection(const ACharacter* OwnerCharacter, FVector DirectionVector,
	bool bIsUseActorSpeedVector)
{
	return 0.0f;
}

FVector UCqsjFlowMoveFuncLib::GetMoveVectorFromCharacterInputVector(const ACharacter* OwnerCharacter)
{
	return FVector();
}

void UCqsjFlowMoveFuncLib::Boxplot_Float(const TArray<float>& InDataArr, TArray<float>& OutDataArr)
{
	return ;
}

UCqsjFlowMoveComponent* UCqsjFlowMoveFuncLib::FM_GetFlowMoveComponent(const AActor* InActor)
{
	return nullptr;
}

void UCqsjFlowMoveFuncLib::FM_ActiveFlowMove(const AActor* InActor)
{
	return ;
}

void UCqsjFlowMoveFuncLib::FM_StopFlowMove(const AActor* InActor, bool WaitForCurrentActionFinished)
{
	
}

bool UCqsjFlowMoveFuncLib::FM_SetMoveVector(const AActor* InActor, FVector NewMoveVector,
	float ZeroFaultToleranceDuration)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_SetControlVector(const AActor* InActor, FVector NewControlVector)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_SetPerceptionVector(const AActor* InActor, FVector NewPerceptionVector)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_SetFocusActor(AActor* InActor, USceneComponent* FocusActorComponent,
	bool bSetViewModeAsActorLock)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_SetViewMode(AActor* InActor, EFlowMoveCharacterViewMode NewViewMode,
	bool bAsDefaultViewMode)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_TryLockActor(AActor* InActor, FGameplayTag GetTargetFunctionTag)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneType(AActor* InActor, FGameplayTag TargetScene,
	FFlowMoveSceneTypeCondition SceneTypeCondition, bool SetAsCurrentTargetSceneWhenMet)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneType(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	const FFlowMoveSceneTypeCondition& SceneTypeCondition, bool SetAsCurrentTargetSceneWhenMet)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneType_Simple(AActor* InActor, FGameplayTag TargetScene,
	TEnumAsByte<EInclusivityType> ConditionType, FName SceneType, bool SetAsCurrentTargetSceneWhenMet)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneType_Simple(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	TEnumAsByte<EInclusivityType> ConditionType, FName SceneType, bool SetAsCurrentTargetSceneWhenMet)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsActionLock(AActor* InActor, FGameplayTagContainer ActionToLock)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsActionLock(const UAnimInstance* AnimInstance,
	const FGameplayTagContainer& ActionToLock)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsActionLock_Simple(AActor* InActor, FGameplayTag ActionToLock)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsActionLock_Simple(const UAnimInstance* AnimInstance, FGameplayTag ActionToLock)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsAnyActionLock(AActor* InActor)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsAnyActionLock(const UAnimInstance* AnimInstance)
{
	return true;
}

void UCqsjFlowMoveFuncLib::FM_IsSide(AActor* InActor, FGameplayTag TargetScene, bool& IsLeft, bool& IsRight,
	float& LeftDistance, float& RightDistance, bool& IsLeftCloser, bool& IsRightCloser)
{
	return ;
}

void UCqsjFlowMoveFuncLib::FM_Pure_IsSide(const UAnimInstance* AnimInstance, FGameplayTag TargetScene, bool& IsLeft,
	bool& IsRight, float& LeftDistance, float& RightDistance, bool& IsLeftCloser, bool& IsRightCloser)
{
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneHeight(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneHeightRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneHeight(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneHeightRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneRight(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneRightRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneRight(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneRightRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneForward(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneForwardRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneForward(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneForwardRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneAngle(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneAngleRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneAngle(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SceneAngleRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSceneDirection(AActor* InActor, FGameplayTag TargetScene,
	FFlowMoveSceneDirectionCondition SceneDirectionCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSceneDirection(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	const FFlowMoveSceneDirectionCondition& SceneDirectionCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsMovementMode(AActor* InActor, FFlowMoveMovementModeCondition MovementModeCondition)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsMovementMode(const UAnimInstance* AnimInstance,
	const FFlowMoveMovementModeCondition& MovementModeCondition)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsHeightFromGround(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromGroundRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsHeightFromGround(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromGroundRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsHeightFromRoof(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromRoofRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsHeightFromRoof(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromRoofRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsFloor(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromFloorRange, bool bIsAnyRequireLedge, FCqsjFlowMoveFloatScope FloorLedgeRange,
	bool bIsRequireForwardLedge, FCqsjFlowMoveFloatScope ForwardFloorLedgeRange, bool bIsRequireBackwardLedge,
	FCqsjFlowMoveFloatScope BackwardFloorLedgeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsFloor(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromFloorRange, bool bIsAnyRequireLedge, FCqsjFlowMoveFloatScope FloorLedgeRange,
	bool bIsRequireForwardLedge, FCqsjFlowMoveFloatScope ForwardFloorLedgeRange, bool bIsRequireBackwardLedge,
	FCqsjFlowMoveFloatScope BackwardFloorLedgeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsRoof(AActor* InActor, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromRoofRange, bool bIsAnyRequireLedge, FCqsjFlowMoveFloatScope RoofLedgeRange,
	bool bIsRequireForwardLedge, FCqsjFlowMoveFloatScope ForwardRoofLedgeRange, bool bIsRequireBackwardLedge,
	FCqsjFlowMoveFloatScope BackwardRoofLedgeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsRoof(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope HeightFromRoofRange, bool bIsAnyRequireLedge, FCqsjFlowMoveFloatScope RoofLedgeRange,
	bool bIsRequireForwardLedge, FCqsjFlowMoveFloatScope ForwardRoofLedgeRange, bool bIsRequireBackwardLedge,
	FCqsjFlowMoveFloatScope BackwardRoofLedgeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsSlope(AActor* InActor, FGameplayTag TargetScene, FCqsjFlowMoveFloatScope SlopeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsSlope(const UAnimInstance* AnimInstance, FGameplayTag TargetScene,
	FCqsjFlowMoveFloatScope SlopeRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsMoveVector(AActor* InActor, FFlowMoveMoveVectorCondition MoveVectorRange)
{
	return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsMoveVector(const UAnimInstance* AnimInstance,
	const FFlowMoveMoveVectorCondition& MoveVectorRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsHasMoveVector(AActor* InActor)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsHasMoveVector(const UAnimInstance* AnimInstance)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsLastActionTag(AActor* InActor, FGameplayTagQuery LastActionCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsLastActionTag(const UAnimInstance* AnimInstance,
	const FGameplayTagQuery& LastActionCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsCurrentActionTag(AActor* InActor, FGameplayTagQuery CurrentActionCondition,
	float MinTimeCondition, float MaxTimeCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsCurrentActionTag(const UAnimInstance* AnimInstance,
	const FGameplayTagQuery& CurrentActionCondition, float MinTimeCondition, float MaxTimeCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsCurrentActionTagAndActionLock(AActor* InActor, FGameplayTagQuery CurrentActionCondition,
	float MinTimeCondition, float MaxTimeCondition, bool IsRequireNoActionLock, bool IsNegateActionLock,
	FGameplayTagContainer ActionLockCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsCurrentActionTagAndActionLock(const UAnimInstance* AnimInstance,
	const FGameplayTagQuery& CurrentActionCondition, float MinTimeCondition, float MaxTimeCondition,
	bool IsRequireNoActionLock, bool IsNegateActionLock, FGameplayTagContainer ActionLockCondition)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsFlowMoveSystemEvent(AActor* InActor, FFlowMoveEvent FlowMoveSystemEvent)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsCharacterSpeed(AActor* InActor, bool OnlyForHorizontalSpeed,
	bool OnlyForMoveToDirectionSpeed, FCqsjFlowMoveFloatScope CharacterSpeedRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsCharacterSpeed(const UAnimInstance* AnimInstance, bool OnlyForHorizontalSpeed,
	bool OnlyForMoveToDirectionSpeed, FCqsjFlowMoveFloatScope CharacterSpeedRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsViewMode(AActor* InActor, TEnumAsByte<EInclusivityType> ConditionType,
	TSet<TEnumAsByte<EFlowMoveCharacterViewMode>> ViewModeSet)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsViewMode(const UAnimInstance* AnimInstance,
	TEnumAsByte<EInclusivityType> ConditionType, TSet<TEnumAsByte<EFlowMoveCharacterViewMode>> ViewModeSet)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsAngle(AActor* InActor, EFlowMoveDirectionType From, EFlowMoveDirectionType To,
	FCqsjFlowMoveFloatScope AngleRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_Pure_IsAngle(const UAnimInstance* AnimInstance, EFlowMoveDirectionType From,
	EFlowMoveDirectionType To, FCqsjFlowMoveFloatScope AngleRange)
{return true;
}

bool UCqsjFlowMoveFuncLib::FM_IsFlowMoveActive(AActor* InActor, bool& IsStopping)
{return true;
}

void UCqsjFlowMoveFuncLib::FM_Pure_IsFlowMoveActive(const UAnimInstance* AnimInstance, bool& IsActive, bool& IsStopping)
{
}

bool UCqsjFlowMoveFuncLib::FM_GetMoveControlParam(AActor* InActor, FCqsjFlowMoveControlParam& MoveControlParam)
{return true;
}

void UCqsjFlowMoveFuncLib::FM_GetMoveParamForBlendSpace(AActor* InActor, UBlendSpace* ForBS, float& Direction,
	float& Speed, float& PlayRate)
{
}

void UCqsjFlowMoveFuncLib::FM_Pure_GetMoveParamForBlendSpace(const UAnimInstance* AnimInstance, UBlendSpace* ForBS,
	float& Direction, float& Speed, float& PlayRate)
{
}

void UCqsjFlowMoveFuncLib::FM_GetAnimExpectedPlayRate(const AActor* InActor, float& PlayRate)
{
}

void UCqsjFlowMoveFuncLib::FM_Pure_GetAnimExpectedPlayRate(const UAnimInstance* AnimInstance, float& PlayRate)
{
}

void UCqsjFlowMoveFuncLib::FM_SetCurrentTargetScene(AActor* InActor, FGameplayTag ActionTargetScene)
{
}

void UCqsjFlowMoveFuncLib::FM_Pure_SetCurrentTargetScene(bool InCondition, bool ExecuteWhenNotMeetCondition,
	const UAnimInstance* AnimInstance, const FGameplayTag& ActionTargetScene, bool& OutCondition)
{
}

void UCqsjFlowMoveFuncLib::FM_SetCurrentActionTag(AActor* InActor, FGameplayTag ActionTag)
{
}

void UCqsjFlowMoveFuncLib::FM_Pure_SetCurrentActionTag(bool InCondition, bool ExecuteWhenNotMeetCondition,
	const UAnimInstance* AnimInstance, const FGameplayTag& ActionTag, bool& OutCondition)
{
}

void UCqsjFlowMoveFuncLib::FM_ClearCurrentActionTag(AActor* InActor)
{
}

void UCqsjFlowMoveFuncLib::FM_Pure_ClearCurrentActionTag(bool InCondition, bool ExecuteWhenNotMeetCondition,
	const UAnimInstance* AnimInstance, bool& OutCondition)
{
}

void UCqsjFlowMoveFuncLib::DrawDebugShape(const UObject* WorldContext, const FVector Location, const FQuat Rotation,
	const FCollisionShape CollisionShape, const FColor Color, float Duration, float Thickness)
{
}

void UCqsjFlowMoveFuncLib::PrintDebugString(TArray<FString> DebugStrArr, FName Key, const FColor Color, float Duration)
{
}

void UCqsjFlowMoveFuncLib::PrintDebugStringNet(const AActor* WorldActor, TArray<FString> DebugStrArr, FName Key,
	const FColor ServerColor, const FColor ClientColor, float Duration)
{
}
