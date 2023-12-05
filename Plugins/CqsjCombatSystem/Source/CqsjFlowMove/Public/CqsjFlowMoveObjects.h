// Copyright GanBowen 2022-2023. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "CqsjAnimFuncLib.h"
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
enum ECqsjFlowMoveScenePointType
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
//这里就升级为Transform了
USTRUCT(BlueprintType)
struct FCqsjFlowMoveTransformScope
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	FCqsjFlowMoveVectorScope Location_Scope = FCqsjFlowMoveVectorScope();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveVectorScope Rotation_Scope = FCqsjFlowMoveVectorScope();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveVectorScope Scale_Scope = FCqsjFlowMoveVectorScope();

	FCqsjFlowMoveTransformScope(){}
	bool InScope(const FTransform InValue) const;
};

USTRUCT(BlueprintType)
struct FCqsjPerceptionResultItemScope
{
	GENERATED_BODY()
	//最重要的ItemType
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	ECqsjPerceptionResultItemType Type = ECqsjPerceptionResultItemType::Float;

	//以下的变量都是根据上面的ItemType来打开的
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Bool"))
	FString BValueScope = "Ture/False";
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Float"))
	FCqsjFlowMoveFloatScope FValueScope = FCqsjFlowMoveFloatScope();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Vector"))
	FCqsjFlowMoveVectorScope VValueScope = FCqsjFlowMoveVectorScope();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Transform"))
	FCqsjFlowMoveTransformScope TValueScope = FCqsjFlowMoveTransformScope();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::String"))
	TSet<FString> SValueScope;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::GameplayTag"))
	TSet<FGameplayTag> GTValueScope;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Actor"))
	FString AValueScope = "Currently not supported";
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::Object"))
	FString OValueScope = "Currently not supported";
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings",meta=(EditConditionHides,EditCondition="Type==EGBWPerceptionResultItemType::MovementMode"))
	TSet<TEnumAsByte<EMovementMode>> MovementModeValueScope = {
		EMovementMode::MOVE_None,
		EMovementMode::MOVE_Walking,
		EMovementMode::MOVE_NavWalking,
		EMovementMode::MOVE_Flying,
		EMovementMode::MOVE_Falling,
		EMovementMode::MOVE_Swimming,
		EMovementMode::MOVE_Custom
	};
	
	FCqsjPerceptionResultItemScope(){}
};

//各种perception需要的变量整体储存的结构体
USTRUCT(BlueprintType)
struct FCqsjPerceptionResultScope
{
	GENERATED_BODY()

	//这是上面那个结构体的映射
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TMap<FName,FCqsjPerceptionResultItemScope> Scope;

	FCqsjPerceptionResultScope(){}
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveSceneScope
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settins")
	TSet<FName> SceneTypeScope;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	FCqsjFlowMoveFloatScope SceneHeightScope = FCqsjFlowMoveFloatScope();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TSet<TEnumAsByte<EMovementMode>> MovementModeScope = {
		EMovementMode::MOVE_None,
		EMovementMode::MOVE_Walking,
		EMovementMode::MOVE_NavWalking,
		EMovementMode::MOVE_Flying,
		EMovementMode::MOVE_Falling,
		EMovementMode::MOVE_Swimming,
		EMovementMode::MOVE_Custom
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveFloatScope HeightFromGround = FCqsjFlowMoveFloatScope();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjPerceptionResultScope PerceptionResultScope = FCqsjPerceptionResultScope();

	FCqsjFlowMoveSceneScope(){}
	
};

UENUM(BlueprintType)
enum EInclusivityType
{
	Include_,
	NotInclude_
};


USTRUCT(BlueprintType)
struct FFlowMoveSceneTypeCondition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TEnumAsByte<EInclusivityType> ConditionType = EInclusivityType::Include_;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TSet<FName> SceneTypeCondition ; 
};

USTRUCT(BlueprintType)
struct FFlowMoveMomentModeCondition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TEnumAsByte<EInclusivityType> ConditionType = EInclusivityType::Include_;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TSet<TEnumAsByte<EMovementMode>> EMovementMode;
	
	
};

UENUM(BlueprintType)
enum EInputValueConditionType
{
	HasValue,
	HasNoValue,
	Range
};

USTRUCT(BlueprintType)
struct FFlowMoveMoveVectorCondition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TEnumAsByte<EInputValueConditionType> ConditionType = EInputValueConditionType::HasValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
		meta=(EditConditionHides,EditCondition="ConditionType == EInputValueConditionType::Range"))
	TArray<FCqsjFlowMoveFloatScope> MoveVectorSizeRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
		meta=(EditConditionHides,EditCondition="ConditionType == EInputValueConditionType::Range"))
	TArray<FCqsjFlowMoveVectorScope> MoveVectorRange;

	bool IsMet(const FCqsjFlowMoveState& FlowMoveState);
	bool IsMet(const FVector& MoveVector);
};

UENUM(BlueprintType)
enum EDirectionConditionType
{
	ValueRange,
	MarkInclude,
	MarkNotInclude
};

USTRUCT(BlueprintType)
struct FCqsjMoveSceneDirectionCondition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings")
	TEnumAsByte<EDirectionConditionType> ConditionType = EDirectionConditionType::MarkInclude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
		meta=(EditConditionHides,EditCondition="ConditionType == EDirectionConditionType::ValueRange"))
	TArray<FCqsjFlowMoveFloatScope> SceneDirectionRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings",
		meta=(EditConditionHides,EditCondition="ConditionType == EDirectionConditionType::MarkInclude || ConditionType == EDirectionConditionType::MarkNotInclude"))
	TSet<ECqsjFlowMoveDirectionType> DirectionMark;

	bool IsMet(const FCqsjFlowMoveScene& FlowMoveScene) const;
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveActionLockState
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	bool bHasLock = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FGameplayTagQuery ActionToLock = FGameplayTagQuery();

	bool IsLock(FGameplayTag ForActionTag) const;
};

UENUM(BlueprintType)
enum ECqsjFlowMoveActionLockStateSettingsType
{
	DoNothing,
	Set,
	ClearAll
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveActionLockStateSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FGameplayTagQuery ActionToLock = FGameplayTagQuery();

	FCqsjFlowMoveActionLockStateSettings(){}
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveAnimCurveState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Value = 0.0f;
	
	FCqsjFlowMoveAnimCurveState(){}
	FCqsjFlowMoveAnimCurveState(FName InName, float InValue)
	{
		Name = InName;
		Value = InValue;
	}
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveAnimPlayRateAdjustSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bUseSpeedAdjustAnimPlayRate = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimPlayRateAdjustReferenceSpeed = 1500.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AdjustAnimPlayRateScale = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Min_AnimPlayRate = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Max_AnimPlayRate = 3.0f;
	
	FCqsjFlowMoveAnimPlayRateAdjustSettings(){}
};

UENUM(BlueprintType)
enum EFlowMoveDirectionType
{
	ActorForward,
	Velocity,
	Controller,
	CurrentMoveToDirection,
	TargetMoveToDirection,
	ActorToTarget,
	StartPointToTarget
};

USTRUCT(BlueprintType)
struct FCqsjFlowMoveControlParam
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsActive = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Action")
	FGuid ActionGuid = FGuid();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Action")
	FGameplayTag ActionTag = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Action")
	FGameplayTag ActionTargetScene = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Action")
	TEnumAsByte<ECqsjFlowMoveScenePointType> ActionTargetPoint = ECqsjFlowMoveScenePointType::TargetPoint;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjAnimPlayStateSet AnimPlayStateSet = FCqsjAnimPlayStateSet();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FCqsjFlowMoveAnimPlayRateAdjustSettings AnimPlayRateAdjust = FCqsjFlowMoveAnimPlayRateAdjustSettings();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float AnimPlayRateNow = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float ToTargetPointRate = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float TargetGravity = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float GravitySmoothSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Gravity = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float RotationLockToMoveDirectionYaw = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TEnumAsByte<EFlowMoveDirectionType> RotationLockTo = EFlowMoveDirectionType::CurrentMoveToDirection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float RotationLockToSmoothSpeed = 12.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float TargetMoveSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MoveSpeedSmoothSpeed = 6.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MoveSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MaxMoveSpeed = 5000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float ConstrainMoveTime = 0.3f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float InertiaTime = 0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float CompensateLostSpeedToCurrentSpeedDirection = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MoveToDirectionSmoothSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector PathOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FVector MoveToDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	float PostureAdjustAmplitude = 30.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	float PostureAdjustReferenceSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaToTargetPointRate = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaGravity = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaRotationLockToMoveDirectionYaw = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaMoveSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float DeltaMoveToDirectionSmoothSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FVector DeltaPathOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FRotator DeltaRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_Gravity = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_RotationLockToMoveDirectionYaw = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_MoveSpeed = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_MoveToDirectionSmoothSpeed = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_MaxSpeed = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_ConstrainMoveToTargetPlane = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_UseInertia = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsLock_UsePostureAdjust = false;
	
	FCqsjFlowMoveControlParam(){}

	bool HasActiveAnimCurveState(FName InName) const;
};

UCLASS()
class CQSJFLOWMOVE_API UCqsjFlowMoveObjects : public UObject
{
	GENERATED_BODY()
};