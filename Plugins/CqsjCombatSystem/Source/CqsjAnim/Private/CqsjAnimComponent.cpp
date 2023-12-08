// Fill out your copyright notice in the Description page of Project Settings.


#include "CqsjAnimComponent.h"


FString FCqsjAnimPlayedCache::GetAnimSequencePlayedCacheDebugString(int CacheCount)
{
	return "None";
}

// Sets default values for this component's properties
UCqsjAnimComponent::UCqsjAnimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCqsjAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


// Called when the game starts
void UCqsjAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCqsjAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCqsjAnimComponent::UpdateAnimPlayState(float DeltaTime)
{
}

ACharacter* UCqsjAnimComponent::GetCharacterOwner()
{
	return nullptr;
}

bool UCqsjAnimComponent::IsLocalOwn() const
{
	return false ;
}

bool UCqsjAnimComponent::IsInServer()
{
	return false ;
}

bool UCqsjAnimComponent::IsReadyToUse()
{
	return false ;
}

bool UCqsjAnimComponent::GetAnimPlayState(FCqsjAnimPlayStateSet& AnimPlayStateSet)
{
	return false ;
}

bool UCqsjAnimComponent::GetAnimPlayState_Local(FCqsjAnimPlayStateSet& AnimPlayStateSet)
{
	return false ;
}

void UCqsjAnimComponent::SetIncludeMainInstance(bool bInclude)
{
}

void UCqsjAnimComponent::SetIncludeLinkedInstance(bool bInclude)
{
}

void UCqsjAnimComponent::AddIncludeLinkedInstanceClass(
	TArray<TSubclassOf<UAnimInstance>> IncludeLinkedAnimInstanceClassSet)
{
}

void UCqsjAnimComponent::PlayMontage(USkeletalMeshComponent* SKMComponent, UAnimMontage* MontageToPlay, float PlayRate,
	float StartingPosition, FName StartingSection)
{
}

void UCqsjAnimComponent::PlayMontage_Imp(const USkeletalMeshComponent* SKMComponent, UAnimMontage* MontageToPlay,
	float PlayRate, float StartingPosition, FName StartingSection)
{
}

void UCqsjAnimComponent::StopMontage(USkeletalMeshComponent* SKMComponent)
{
}

void UCqsjAnimComponent::StopMontage_Imp(const USkeletalMeshComponent* SKMComponent)
{
}

UAnimMontage* UCqsjAnimComponent::GetNextMontageFromList(TArray<UAnimMontage*> MontageList)
{
	return nullptr;
}

UAnimSequence* UCqsjAnimComponent::GetNextAnimSequenceFromList(bool bHoldNowWhenAnimActive,
	bool bHoldNowWhenAnimIsPreAnim, TArray<UAnimSequence*> AnimSequenceList)
{
	return nullptr;
}

ECqsjAnimPlayStateType UCqsjAnimComponent::GetAnimPlayStateByNodeIndex(int32 NodeIndex, bool& bIsReplay)
{
	return ECqsjAnimPlayStateType::Active;
}

bool UCqsjAnimComponent::IsAnimStateIsConsistentWithServer() const
{
	return false ;
}

bool UCqsjAnimComponent::GetStateMachineInfoByNodeIndex(int32 NodeIndex, FCqsjAnimStateMachineNodeInfo& SMInfo) const
{
	return false ;
}

void UCqsjAnimComponent::StopMontage_Multicast_Implementation(USkeletalMeshComponent* SKMComponent)
{
}

void UCqsjAnimComponent::StopMontage_Server_Implementation(USkeletalMeshComponent* SKMComponent)
{
}

void UCqsjAnimComponent::PlayMontage_Multicast_Implementation(USkeletalMeshComponent* SKMComponent,
                                                              UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection)
{
}

void UCqsjAnimComponent::PlayMontage_Server_Implementation(USkeletalMeshComponent* SKMComponent,
                                                           UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection)
{
}

void UCqsjAnimComponent::AddIncludeLinkedInstanceClass_Server_Implementation(
	const TArray<TSubclassOf<UAnimInstance>>& IncludeLinkedAnimInstanceClassSet)
{
}

void UCqsjAnimComponent::SetIncludeLinkedInstance_Server_Implementation(bool bInclude)
{
}

void UCqsjAnimComponent::SetIncludeMainInstance_Server_Implementation(bool bInclude)
{
}

void UCqsjAnimComponent::SetAnimPlayState_Multicast_Implementation(FCqsjAnimPlayStateSet NewAnimPlayState)
{
}

void UCqsjAnimComponent::SetAnimPlayState_Server_Implementation(FCqsjAnimPlayStateSet NewAnimPlayState)
{
}

