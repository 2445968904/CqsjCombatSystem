// Copyright GanBowen 2022-2023. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "CqsjFlowMoveObjects.generated.h"

/**
 * 
 */



	struct FFlowMoveAttributeModifier;
	struct FCqsjFlowMoveState;
	struct FCqsjFlowMoveScene;
	class UCqsjFlowMoveComponent;
	class UCqsjFlowMovePerceptron_Base;
	class UCqsjFlowMoveScript_Base;
	class UCqsjFlowMoveGetFocusActor_Base;

	//开始定义结构
	UENUM(BlueprintType)
	enum class ECqsjFlowMoveEventType: uint8
	{
		Init,
		Update,
		End
	};
	UENUM(BlueprintType)//预测返回的Item类型
	enum class ECqsjPerceptionResultItemType : uint8
	{
		Bool,
		Float,
		Vector,
		Transform,
		String,
		GamePlayTag,
		Actor,
		Object,
		MovementMode
	};
	UENUM(BlueprintType)
	enum EConditionRequireType
	{
		RequireMet,
		RequireNotMet,
		NotRequire
	};
	UENUM(BlueprintType)
	enum EFlowMoveEventType
	{
		OnActive,
		OnActiveFailed,
		OnUpdate,
		OnStop,
		OnActionStart,
		OnActionUpdate,
		OnActionEnd,
		OnNoActionCanExecute,
		OnMovementModeChange,
		OnViewModeChange,
		OnFocusActorChange
	};
	UENUM(BlueprintType)
	enum EFlowMoveCharacterViewMode
	{
		TP_FreeMode,
		TP_ForwardLockMode,
		TP_ActorLockMode
	};


	USTRUCT(BlueprintType)
	struct FFlowMoveEvent
	{
		GENERATED_BODY()

		UPROPERTY()
		int InFrameNum = -1;
	
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
		TEnumAsByte<EFlowMoveEventType> EventType = EFlowMoveEventType::OnActionStart;
		//这个变量表示的是ActionTag 只能从OnActionStart OnActionEnd OnActionUpdate 中选择
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
			meta=(EditConditionHides,EditCondition="EventType == EFlowMoveEventType::OnActionStart || EventType == EFlowMoveEventType::OnActionEnd || EventType == EFlowMoveEventType::OnActionUpdate"))
		FGameplayTag ActionTag;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
			meta=(EditConditionHides,EditCondition="EventType == EFlowMoveEventType::OnMovementModeChange"))
		TEnumAsByte<EMovementMode> NewMovementMode = EMovementMode::MOVE_None;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
			meta=(EditConditionHides,EditCondition="EventType == EFlowMoveEventType::OnViewModeChange"))
		TEnumAsByte<EFlowMoveCharacterViewMode> NewViewMode = EFlowMoveCharacterViewMode::TP_FreeMode;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
			meta=(EditConditionHides,EditCondition="EventType == EFlowMoveEventType::OnFocusActorChange"))
		USceneComponent* NewFocusActorComponent = nullptr;

		//都是构造函数
		FFlowMoveEvent(){}
		FFlowMoveEvent(EFlowMoveEventType TheEventType);
		FFlowMoveEvent(EFlowMoveEventType TheEventType, FGameplayTag InActionTag);
		FFlowMoveEvent(EMovementMode TheNewMovementMode);
		FFlowMoveEvent(EFlowMoveCharacterViewMode TheNewViewMode);
		FFlowMoveEvent(USceneComponent* TheNewFocusActorComponent);
	
		bool EqualTo(const FFlowMoveEvent& OtherEvent) const;
		FString GetDebugString() const;
	};

	USTRUCT(BlueprintType)
	struct FFlowMoveInputValueTrigger_PerceptionInput
	{
		GENERATED_BODY()
		//ForceUnits ="°"这个表示的是数据是度
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Perception",
			meta=(ClampMin=0, ForceUnits = "°"))
		float UpAngleThreshold = 10.0f;
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Perception",
			meta=(ClampMin=0, ForceUnits = "°"))
		float DownAngleThreshold = 60.0f;
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Perception",
			meta=(ClampMin=0, ForceUnits = "°"))
		float LeftAndRightAngleThreshold = 30.0f;
	
		FFlowMoveInputValueTrigger_PerceptionInput(){}
	};

	USTRUCT(BlueprintType)
	struct FCqsjFlowMoveTraceSetting
	{
		GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		bool isDebug = false;
		//只有启动了iswDebug 下面的变量才能被修改
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		float DebugTime = 0;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		bool DrawTraceShape = false;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		bool DrawTraceLine = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		TArray<TEnumAsByte<ETraceTypeQuery>> TraceChannels;

		//使用筛选条件 这个UseScreeningConditions 启用了之后后面的几个变量才能被相应的进行更改
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions")
		bool UseScreeningConditions = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions",
			meta=(EditConditionHides,EditCondition="UseScreeningConditions"))
		TArray<TSubclassOf<AActor>> ActorWithClass;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions",
			meta=(EditConditionHides,EditCondition="UseScreeningConditions"))
		TArray<FName> ActorWithTag;

		FCqsjFlowMoveTraceSetting(){}
		bool IsMetScreeningConditions(const AActor* InActor);
	};


	USTRUCT(BlueprintType)
	struct FGBWFlowMoveTraceSetting
	{
		GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		bool isDebug = false;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		float DebugTime = 0;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		bool DrawTraceShape = false;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug",
			meta=(EditConditionHides,EditCondition="isDebug"))
		bool DrawTraceLine = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		TArray<TEnumAsByte<ETraceTypeQuery>> TraceChannels;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions")
		bool UseScreeningConditions = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions",
			meta=(EditConditionHides,EditCondition="UseScreeningConditions"))
		TArray<TSubclassOf<AActor>> ActorWithClass;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreeningConditions",
			meta=(EditConditionHides,EditCondition="UseScreeningConditions"))
		TArray<FName> ActorWithTag;

		FGBWFlowMoveTraceSetting(){}
		bool IsMetScreeningConditions(const AActor* InActor);
	};


	USTRUCT(BlueprintType)
	struct FCqsjPerceptionResultItem
	{
		GENERATED_BODY()

		UPROPERTY()
		ECqsjPerceptionResultItemType Type = ECqsjPerceptionResultItemType::Float;
	
		UPROPERTY()
		bool BValue = true;
		UPROPERTY()
		float FValue = 0.0f;
		UPROPERTY()
		FVector VValue = FVector::ZeroVector;
		UPROPERTY()
		FTransform TValue = FTransform();
		UPROPERTY()
		FString SValue = "";
		UPROPERTY()
		FGameplayTag GTValue = FGameplayTag::EmptyTag;
		UPROPERTY()
		AActor* AValue = nullptr;
		UPROPERTY()
		UObject* OValue = nullptr;
		UPROPERTY()
		TEnumAsByte<EMovementMode> MovementModeValue = EMovementMode::MOVE_None;
	
		bool EqualTo(const FCqsjPerceptionResultItem& OtherPerceptionResultItem) const;
		//以下全是构造函数了
		FCqsjPerceptionResultItem(){}
		FCqsjPerceptionResultItem(bool value){BValue=value;Type = ECqsjPerceptionResultItemType::Bool;}
		FCqsjPerceptionResultItem(float value){FValue=value;Type = ECqsjPerceptionResultItemType::Float;}
		FCqsjPerceptionResultItem(FVector value){VValue=value;Type = ECqsjPerceptionResultItemType::Vector;}
		FCqsjPerceptionResultItem(FTransform value){TValue=value;Type = ECqsjPerceptionResultItemType::Transform;}
		FCqsjPerceptionResultItem(FString value){SValue=value;Type = ECqsjPerceptionResultItemType::String;}
		FCqsjPerceptionResultItem(FGameplayTag value){GTValue=value;Type = ECqsjPerceptionResultItemType::GamePlayTag;}
		FCqsjPerceptionResultItem(AActor* value){AValue=value;Type = ECqsjPerceptionResultItemType::Actor;}
		FCqsjPerceptionResultItem(UObject* value){OValue=value;Type = ECqsjPerceptionResultItemType::Object;}
		FCqsjPerceptionResultItem(EMovementMode value){MovementModeValue=value;Type = ECqsjPerceptionResultItemType::MovementMode;}
	};


	USTRUCT(BlueprintType)
	struct FCqsjPerceptionResult
	{
		GENERATED_BODY()
	
		UPROPERTY()
		TArray<FName> Keys;
		UPROPERTY()
		TArray<FCqsjPerceptionResultItem> Values;
	
		FCqsjPerceptionResult(){}

		bool IsValid() const;

		bool EqualTo(FCqsjPerceptionResult OtherPerceptionResult);

		void AddBool(FName Key, bool Value);
		void AddFloat(FName Key, float Value);
		void AddVector(FName Key, FVector Value);
		void AddTransform(FName Key, FTransform Value);
		void AddString(FName Key, FString Value);
		void AddGameplayTag(FName Key, FGameplayTag Value);
		void AddActor(FName Key, AActor* Value);
		void AddObject(FName Key, UObject* Value);
		void AddMovementMode(FName Key, EMovementMode Value);

		//这里都是IsGet作为引用 然后Value作为一个传进来的值
		void GetBool(FName Key, bool& IsGet, bool& Value) const;
		void GetFloat(FName Key, bool& IsGet, float& Value) const;
		void GetVector(FName Key, bool& IsGet, FVector& Value) const;
		void GetTransform(FName Key, bool& IsGet, FTransform& Value) const;
		void GetString(FName Key, bool& IsGet, FString& Value) const;
		void GetGameplayTag(FName Key, bool& IsGet, FGameplayTag& Value) const;
		void GetActor(FName Key, bool& IsGet, AActor*& Value) const;
		void GetObject(FName Key, bool& IsGet, UObject*& Value) const;
		void GetMovementMode(FName Key, bool& IsGet, TEnumAsByte<EMovementMode>& Value) const;
	};

	UENUM(BlueprintType)
	enum EFlowMoveAnimType
	{
		Montage,
		BlendSpace
	};

	USTRUCT(BlueprintType)
	struct FCqsjFlowMoveAnimSetting
	{
		GENERATED_BODY()

		UPROPERTY()
		bool bIsPlayed = false;
		//默认是蒙太奇动画
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		TEnumAsByte<EFlowMoveAnimType> AnimType = EFlowMoveAnimType::Montage;
    
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		FName MeshTag = NAME_None;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		TArray<FName> ExcludedMeshTag = {};
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
			meta=(EditConditionHides, EditCondition="AnimType == EFlowMoveAnimType::Montage"))
		UAnimMontage* Montage = nullptr;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
			meta=(EditConditionHides, EditCondition="AnimType == EFlowMoveAnimType::Montage"))
		bool bIsLoop = false;
		/** If the array is empty, random will be attempted in all Montage Sections */
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
			meta=(EditConditionHides, EditCondition="AnimType == EFlowMoveAnimType::Montage"))
		TArray<FName> RandomSections = {};
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings",
			meta=(EditConditionHides, EditCondition="AnimType == EFlowMoveAnimType::BlendSpace"))
		UBlendSpace* AnimBlendSpace = nullptr;

		bool EqualTo(const FCqsjFlowMoveAnimSetting& Other) const;
		bool IsAnimValid() const;
		void Reset();
		FString GetDebugStr() const;
	};

//这个类有4个的蓝图引用需要研究
UCLASS(Blueprintable, meta = (DisplayName = "CqsjFlowMoveObject_Base"))
class CQSJFLOWMOVE_API  UCqsjFlowMoveObject_Base : public UObject
{
	GENERATED_BODY()

	virtual bool IsNameStableForNetworking() const override;
	virtual bool IsSupportedForNetworking() const override;
protected:
	struct FWorldContext* WorldContext;
public:
	UCqsjFlowMoveObject_Base();

	UPROPERTY()
	FGuid Guid = FGuid();

	// Allows the Object to use BP_Functions
	UFUNCTION(BlueprintCallable, Category="WorldContext")
	void SetWorldContext(UObject* NewWorldContext);

	UFUNCTION(BlueprintCallable, Category = "WorldContext")
	UObject* GetWorldContextObject();

	//~ Begin UObject Interface
	virtual class UWorld* GetWorld() const override final;
	struct FWorldContext* GetWorldContext() const { return WorldContext; };
};

UENUM(BlueprintType)
enum class ECqsjFlowMoveDirectionType : uint8
{
	MM UMETA(DisplayName="◈"),
	MU UMETA(DisplayName="↑"),
	MD UMETA(DisplayName="↓"),
	LM UMETA(DisplayName="←"),
	RM UMETA(DisplayName="→"),
	LU UMETA(DisplayName="↖"),
	RU UMETA(DisplayName="↗"),
	LD UMETA(DisplayName="↙"),
	RD UMETA(DisplayName="↘")
};

UENUM(BlueprintType)
enum EGBWFlowMoveScenePointType
{
	TargetPoint,
	ActorPoint,
	LeftSidePoint,
	RightSidePoint,
	FloorPoint,
	RoofPoint,
	FloorForwardLedgePoint,
	FloorBackwardLedgePoint,
	NearestFloorLedgePoint,
	RoofForwardLedgePoint,
	RoofBackwardLedgePoint,
	NearestRoofLedgePoint,
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveSceneChangeInfo
{
	GENERATED_BODY()
	
	bool SceneType = false;
	bool ActorLocationScene = false;
	bool TargetLocationScene = false;
	bool LeftLocationScene = false;
	bool RightLocationScene = false;
	bool TargetActor = false;
	bool Slope = false;
	bool PerceptionResult = false;

	FCqsjFlowMoveSceneChangeInfo(){}
};

//向地板和天花板移动
USTRUCT(BlueprintType)
struct FCqsjFlowMoveFloorRoofScene
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector Floor = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector FloorLedge_Forward = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector FloorLedge_Backward = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector Roof = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector RoofLedge_Forward = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector RoofLedge_Backward = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float HeightFromGround = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float HeightFromRoof = 0.0f;

	

	FCqsjFlowMoveFloorRoofScene(){}
	bool EqualTo(const FCqsjFlowMoveFloorRoofScene& Other) const;
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveBlockWallScene
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector WallPoint = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector WallLedge_Up = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector WallLedge_Down = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector WallLedge_Left = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector WallLedge_Right = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DistanceFromActor = 0.0f;

	FCqsjFlowMoveBlockWallScene(){}
	bool EqualTo(const FCqsjFlowMoveBlockWallScene& Other) const;
	bool IsWall() const;
	bool IsLedge_Up() const;
	bool IsLedge_Down() const;
	bool IsLedge_Left() const;
	bool IsLedge_Right() const;
};

USTRUCT(BlueprintType)
struct FCqsjFlowMovePointScene
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector Point = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Height = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Right = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Forward = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Angle = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Direction = 0.f;
	//MM 是中心的意思
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	ECqsjFlowMoveDirectionType SceneDirectionMark = ECqsjFlowMoveDirectionType::MM;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveFloorRoofScene FloorAndRoof = FCqsjFlowMoveFloorRoofScene();

	FCqsjFlowMovePointScene(){}
	FCqsjFlowMovePointScene(ACharacter* InCharacter, const FVector& InPoint, bool bFindLedge, float LedgeTraceDistance, FGBWFlowMoveTraceSetting TraceSetting);
	bool EqualTo(const FCqsjFlowMovePointScene& Other) const;
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveScene
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName SceneType = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMovePointScene ActorLocationScene = FCqsjFlowMovePointScene();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMovePointScene TargetLocationScene = FCqsjFlowMovePointScene();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMovePointScene LeftLocationScene = FCqsjFlowMovePointScene();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMovePointScene RightLocationScene = FCqsjFlowMovePointScene();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Slope = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjPerceptionResult PerceptionResult = FCqsjPerceptionResult();

	FCqsjFlowMoveScene(){}
	FCqsjFlowMoveScene(ACharacter* InCharacter,
		FName InSceneType,
		AActor* InTargetActor,
		float InSlope,
		const FVector& InTargetPoint,
		const FVector& LeftTargetPoint,
		const FVector& RightTargetPoint,
		const FCqsjFlowMoveTraceSetting& TraceSetting,
		float LedgeTraceDistance);

	bool IsValid() const;
	bool EqualTo(const FCqsjFlowMoveScene& OtherScene);
	bool CheckSceneChange(
		const FCqsjFlowMoveScene& OtherScene,
		FCqsjFlowMoveSceneChangeInfo& ChangeInfo);

	bool GetPlaneConstraintSetting(FVector& PlaneNormal, FVector& PlaneOrigin) const;
};

USTRUCT(BlueprintType)
struct FCqsjSupplementaryFlowMoveScene
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FGameplayTag SceneSlot = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveScene Scene = FCqsjFlowMoveScene();
};

//这个枚举用来设置>=还是>
UENUM(BlueprintType)
enum class ECqsjFlowMoveValueScopePointType : uint8
{
	Contain UMETA(DisplayName="Contain"),
	NotContain UMETA(DisplayName="NotContain"),
	Unrestricted UMETA(DisplayName="∞")
};


USTRUCT(BlueprintType)
struct FCqsjFlowMoveFloatScopePoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	ECqsjFlowMoveValueScopePointType Type = ECqsjFlowMoveValueScopePointType::Unrestricted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings", meta=(EditConditionHides, EditCondition="Type!=EGBWFlowMoveValueScopePointType::Unrestricted"))
	float Value = 0.0f;

	FCqsjFlowMoveFloatScopePoint(){}
	
};

//用于定义浮点数范围的数据结构
USTRUCT(BlueprintType)
struct FCqsjFlowMoveFloatScope
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveFloatScopePoint Min = FCqsjFlowMoveFloatScopePoint();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveFloatScopePoint Max = FCqsjFlowMoveFloatScopePoint();

	FCqsjFlowMoveFloatScope(){}
	bool InScope(const float InValue) const;
};

//这个结构体就是定义有范围限制的一个向量了
USTRUCT(BlueprintType)
struct FCqsjFlowMoveVectorScope
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	FCqsjFlowMoveFloatScope X_Scope = FCqsjFlowMoveFloatScope();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	FCqsjFlowMoveFloatScope Y_Scope = FCqsjFlowMoveFloatScope();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	FCqsjFlowMoveFloatScope Z_Scope = FCqsjFlowMoveFloatScope();

	FCqsjFlowMoveVectorScope(){}
	bool InScope(const FVector InValue) const ;//这个函数进行检查是否在范围内
};



UCLASS()
class CQSJFLOWMOVE_API UCqsjFlowMoveObjects : public UObject
{
	GENERATED_BODY()
};