// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CqsjPowerfulAnimInstance.h"
#include "CqsjAnimComponent.generated.h"


UENUM(BlueprintType)
enum ECqsjAnimAssetType
{
	None,
	AnimSequence,
	AnimBlendSpace,
	AnimMontage
};
UENUM(BlueprintType)
enum ECqsjAnimPlayStateType
{
	Active,
	Deactivated,
	BlendIn,
	BlendOut
};


USTRUCT(BlueprintType)
struct FCqsjAnimPlayState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimInstance* Instance = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TEnumAsByte<ECqsjAnimAssetType> AssetType = ECqsjAnimAssetType::None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimationAsset* AnimAsset = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 AnimNodeIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TEnumAsByte<ECqsjAnimPlayStateType> PlayState = ECqsjAnimPlayStateType::Deactivated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsReplay = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float BlendInTime = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float BlendOutTime = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimPlayLength = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimPlayDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimAllPlayLength = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimPlayPosition = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimBlendWeight = 0.0f;
	
	FCqsjAnimPlayState(){}
	FCqsjAnimPlayState(
		UAnimInstance* InInstance,
		ECqsjAnimAssetType InAssetType,
		UAnimationAsset* InAnimAsset,
		int32 InAnimNodeIndex,
		float InBlendInTime,
		float InBlendOutTime,
		float InAnimPlayLength,
		float InAnimPlayDuration,
		float InAnimAllPlayLength,
		float InAnimPlayPosition,
		float InAnimBlendWeight
		)
	{
		Instance = InInstance;
		AssetType = InAssetType;
		AnimAsset = InAnimAsset;
		AnimNodeIndex = InAnimNodeIndex;
		BlendInTime = InBlendInTime;
		BlendOutTime = InBlendOutTime;
		AnimPlayLength = InAnimPlayLength;
		AnimPlayDuration = InAnimPlayDuration;
		AnimAllPlayLength = InAnimAllPlayLength;
		AnimPlayPosition = InAnimPlayPosition;
		AnimBlendWeight = InAnimBlendWeight;
	}
};
USTRUCT(BlueprintType)
struct FCqsjAnimCurveValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName CurveName = FName();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Value = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaValue = 0.0f;

	FCqsjAnimCurveValue(){}
	FCqsjAnimCurveValue(FName InCurveName, float InValue, float InDeltaValue)
	{
		CurveName = InCurveName;
		Value = InValue;
		DeltaValue = InDeltaValue;
	}
};

USTRUCT(BlueprintType)
struct FCqsjAnimStateMachineNodeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimInstance* Instance = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 NodeIndex = -1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName CurrentStateName = FName();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float CurrentStateWeight = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsActive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float ElapsedTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName LastStateName = FName();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float LastStateElapsedTime = 0.0f;
	
	FCqsjAnimStateMachineNodeInfo(){}
	FCqsjAnimStateMachineNodeInfo(
		UAnimInstance* InInstance,
		int32 InNodeIndex,
		FName InCurrentStateName,
		FName InLastStateName,
		float InCurrentStateWeight,
		bool InIsActive,
		float InElapsedTime,
		float InLastStateElapsedTime)
	{
		Instance = InInstance;
		NodeIndex = InNodeIndex;
		CurrentStateName = InCurrentStateName;
		LastStateName = InLastStateName;
		CurrentStateWeight = InCurrentStateWeight;
		bIsActive = InIsActive;
		ElapsedTime = InElapsedTime;
		LastStateElapsedTime = InLastStateElapsedTime;
	}
};

USTRUCT(BlueprintType)
struct FCqsjAnimPlayedCache
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int MaxAnimSequencePlayedCacheCount = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimMontage* LastMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<UAnimSequence*> AnimSequencePlayed = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int AnimSequencePlayedCacheIndexNow = -1;

	FCqsjAnimPlayedCache(){}
	void AddAnimSequenceCache(UAnimSequence* InAnimSequence);
	bool GetLastAnimSequenceFromArr(TArray<UAnimSequence*> AnimArr, UAnimSequence*& LastAnim, int& LastAnimIndex);
	UAnimSequence* GetAnimSequenceNow();
	UAnimSequence* GetPreviousAnimSequence(int Offset = 0);
	FString GetAnimSequencePlayedCacheDebugString(int CacheCount = -1);
};

USTRUCT(BlueprintType)
struct FCqsjAnimPlayStateSet
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int FrameCount = -1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimInstance* Instance = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<UAnimInstance*> LinkedInstances = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjAnimPlayState MainAnimPlayState = FCqsjAnimPlayState();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<FCqsjAnimPlayState> AnimPlayStateSet = {};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsBlendFinished = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<UAnimNotifyState*> NotifyStateSet = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<FCqsjAnimCurveValue> CurveValueState = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TArray<FCqsjAnimStateMachineNodeInfo> StateMachineState = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjAnimPlayedCache AnimPlayedCache = FCqsjAnimPlayedCache();

	FCqsjAnimPlayStateSet(){}
	FCqsjAnimPlayStateSet(
		UAnimInstance* InInstance,
		const TArray<UAnimInstance*>& InLinkedInstances,
		const FCqsjAnimPlayState& InMainAnimPlayState,
		const TArray<FCqsjAnimPlayState>& InAnimPlayStateSet,
		const TArray<FCqsjAnimStateMachineNodeInfo>& InStateMachineState,
		bool InIsBlendFinished
		)
	{
		Instance = InInstance;
		LinkedInstances = InLinkedInstances;
		MainAnimPlayState = InMainAnimPlayState;
		AnimPlayStateSet = InAnimPlayStateSet;
		StateMachineState = InStateMachineState;
		bIsBlendFinished = InIsBlendFinished;
	}
	FCqsjAnimPlayStateSet(
		UAnimInstance* InInstance,
		const TArray<UAnimInstance*>& InLinkedInstances,
		const FCqsjAnimPlayState& InMainAnimPlayState,
		const TArray<FCqsjAnimPlayState>& InAnimPlayStateSet,
		bool InIsBlendFinished,
		const TArray<UAnimNotifyState*>& InNotifyStateSet,
		const TArray<FCqsjAnimCurveValue>& InCurveValueState,
		const TArray<FCqsjAnimStateMachineNodeInfo>& InStateMachineState
		)
	{
		Instance = InInstance;
		LinkedInstances = InLinkedInstances;
		MainAnimPlayState = InMainAnimPlayState;
		AnimPlayStateSet = InAnimPlayStateSet;
		bIsBlendFinished = InIsBlendFinished;
		NotifyStateSet = InNotifyStateSet;
		CurveValueState = InCurveValueState;
		StateMachineState = InStateMachineState;
	}

	bool IsValid() const;
};
USTRUCT(BlueprintType, meta=(DisplayName="GBWConditionMontage"))
struct FCqsjAnimStateGetType
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool IncludeMainAnimInstance = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool IncludeLinkedAnimInstance = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
		meta=(EditCondition="IncludeLinkedAnimInstance"))
	TArray<TSubclassOf<UAnimInstance>> IncludeLinkedAnimInstanceClassSet = {UCqsjPowerfulAnimInstance::StaticClass()};
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
		meta=(EditCondition="IncludeMainAnimInstance"))
	TSet<FName> IncludeLinkedAnimInstanceTagSet = {};*/

	FCqsjAnimStateGetType(){}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CQSJANIM_API UCqsjAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCqsjAnimComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	int TickCount = 0;
	
public:
	UPROPERTY(Replicated)
	ACharacter* CharacterOwner = nullptr;
	UPROPERTY(BlueprintReadOnly, Replicated, Category="AnimState")
	FCqsjAnimPlayStateSet AnimPlayState = FCqsjAnimPlayStateSet();
	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	FCqsjAnimPlayStateSet AnimPlayState_Local = FCqsjAnimPlayStateSet();
	UPROPERTY(BlueprintReadOnly, Replicated, Category="AnimState")
	FCqsjAnimStateGetType AnimStateGetType = FCqsjAnimStateGetType();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateAnimPlayState(float DeltaTime);

	ACharacter* GetCharacterOwner();
	bool IsLocalOwn() const;
	bool IsInServer();

	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	bool IsReadyToUse();

	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void SetAnimPlayState_Server(FCqsjAnimPlayStateSet NewAnimPlayState);
	UFUNCTION(Reliable, NetMulticast, Category = "Cqsj|Anim")
	void SetAnimPlayState_Multicast(FCqsjAnimPlayStateSet NewAnimPlayState);
	
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	bool GetAnimPlayState(FCqsjAnimPlayStateSet& AnimPlayStateSet);
	UFUNCTION(BlueprintCallable, Category = "Cqsj|Anim")
	bool GetAnimPlayState_Local(FCqsjAnimPlayStateSet& AnimPlayStateSet);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	void SetIncludeMainInstance(bool bInclude = false);
	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void SetIncludeMainInstance_Server(bool bInclude = false);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	void SetIncludeLinkedInstance(bool bInclude = false);
	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void SetIncludeLinkedInstance_Server(bool bInclude = false);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	void AddIncludeLinkedInstanceClass(TArray<TSubclassOf<UAnimInstance>> IncludeLinkedAnimInstanceClassSet);
	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void AddIncludeLinkedInstanceClass_Server(const TArray<TSubclassOf<UAnimInstance>>& IncludeLinkedAnimInstanceClassSet);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	void PlayMontage(USkeletalMeshComponent* SKMComponent,
		UAnimMontage* MontageToPlay,
		float PlayRate=1.0f,
		float StartingPosition=0.0f,
		FName StartingSection=NAME_None);
	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void PlayMontage_Server(USkeletalMeshComponent* SKMComponent,
		UAnimMontage* MontageToPlay,
		float PlayRate=1.0f,
		float StartingPosition=0.0f,
		FName StartingSection=NAME_None);
	UFUNCTION(Reliable, NetMulticast, Category = "Cqsj|Anim")
	void PlayMontage_Multicast(USkeletalMeshComponent* SKMComponent,
		UAnimMontage* MontageToPlay,
		float PlayRate=1.0f,
		float StartingPosition=0.0f,
		FName StartingSection=NAME_None);
	void PlayMontage_Imp(const USkeletalMeshComponent* SKMComponent,
	                            UAnimMontage* MontageToPlay,
	                            float PlayRate=1.0f,
	                            float StartingPosition=0.0f,
	                            FName StartingSection=NAME_None);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	void StopMontage(USkeletalMeshComponent* SKMComponent);
	UFUNCTION(Reliable, Server, Category = "Cqsj|Anim")
	void StopMontage_Server(USkeletalMeshComponent* SKMComponent);
	UFUNCTION(Reliable, NetMulticast, Category = "Cqsj|Anim")
	void StopMontage_Multicast(USkeletalMeshComponent* SKMComponent);
	void StopMontage_Imp(const USkeletalMeshComponent* SKMComponent);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	UAnimMontage* GetNextMontageFromList(
		TArray<UAnimMontage*> MontageList);

	UFUNCTION(BlueprintCallable,Category = "Cqsj|Anim")
	UAnimSequence* GetNextAnimSequenceFromList(
		bool bHoldNowWhenAnimActive,
		bool bHoldNowWhenAnimIsPreAnim,
		TArray<UAnimSequence*> AnimSequenceList);

	ECqsjAnimPlayStateType GetAnimPlayStateByNodeIndex(int32 NodeIndex, bool& bIsReplay);

	bool IsAnimStateIsConsistentWithServer() const;
	bool GetStateMachineInfoByNodeIndex(int32 NodeIndex, FCqsjAnimStateMachineNodeInfo& SMInfo) const;
};

USTRUCT(BlueprintType, meta=(DisplayName="CqsjMontage"))
struct FCqsjMontage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	UAnimMontage* MontageToPlay = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float PlayRate=1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float StartingPosition=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName StartingSection=NAME_None;
};

USTRUCT(BlueprintType, meta=(DisplayName="CqsjConditionMontage"))
struct FCqsjConditionMontage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool Condition = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjMontage MontageSetting = FCqsjMontage();

	bool CanPlay() const;
};


