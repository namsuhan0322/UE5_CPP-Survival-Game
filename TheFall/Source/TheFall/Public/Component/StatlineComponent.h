// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

/// 핵심 스탯의 종류를 정의하는 열거형 (블루프린트에서도 사용 가능)
UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName = "Health"),  // 체력
	CS_STAMINA UMETA(DisplayName = "Stamina"), // 스태미나
	CS_HUNGER UMETA(DisplayName = "Hunger"),   // 허기
	CS_THIRST UMETA(DisplayName = "Thirst")    // 갈증
};

/// 각 스탯을 구성하는 구조체 (블루프린트 사용 가능)
USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	// 현재 수치
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;

	// 최대 수치
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;

	// 초당 변화량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

public:

	// 기본 생성자
	FCoreStat() {}

	// 지정된 값으로 초기화하는 생성자
	FCoreStat(const float& current, const float& max, const float& tick)
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
	}

	// 프레임 단위로 스탯을 증가/감소시킴
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	// 외부에서 지정한 양만큼 수치를 조정
	void Adjust(const float& Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Max);
	}

	// 현재 수치의 백분율 반환 (UI 게이지 등에서 사용 가능)
	float Percentile() const
	{
		return Current / Max;
	}

	// 초당 변화량 설정
	void AbjustTick(const float& NewTick)
	{
		PerSecondTick = NewTick;
	}

	float GetCurrent() const
	{
		return Current;
	}
};

/// 캐릭터의 스탯들을 관리하는 컴포넌트 클래스
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEFALL_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	class UCharacterMovementComponent* OwningCharMovementComp;

	// 스탯 데이터들
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat(100, 100, -2.125);	// 기본적으로 서서히 줄어듦
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -2.25);	// 갈증은 허기보다 더 빠르게 줄어듦

	// 현재 상태 플래그 및 수치
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintCostMulitiplier = 2;				// 달릴 때 스태미나 소모 배율
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 125;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 450;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 75;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10;							// 점프 시 스태미나 소모량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SecondsForStaminaExhuastion = 5;			// 스태미나 0 유지 시간 기준
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentStaminaExhuastion = 0;				// 현재 탈진 지속 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StarvingHealthDamagePerSecond = 1;		// 현재 탈진 지속 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DehydrationHealthDamagePerSecond = 1;		// 갈증 0일 때 체력 감소량

	// 내부 업데이트 함수들
	void TickStats(const float& DeltaTime);			// 전체 스탯 틱 갱신
	void TickStamina(const float& DeltaTime);		// 스태미나 전용 처리
	void TickHunger(const float& DeltaTime);		// 허기 처리
	void TickThirst(const float& DeltaTime);		// 갈증 처리
	bool IsValidSprinting();						// 스프린트 유효성 확인

protected:
	virtual void BeginPlay() override;

public:

	// 생성자
	UStatlineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 외부에서 캐릭터 이동 컴포넌트를 참조 연결할 때 사용
	UFUNCTION(BlueprintCallable)
	void SetMovementCompReference(UCharacterMovementComponent* Comp);

	// UI 등에서 퍼센트 스탯 조회
	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;
	// 달릴 수 있는지 여부
	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;
	// 달리기 상태 설정
	UFUNCTION(BlueprintCallable)
	void SetSprinting(const bool& IsSprinting);
	// 웅크리기 상태 설정
	UFUNCTION(BlueprintCallable)
	void SetSneaking(const bool& IsSneaking);
	// 점프 가능 여부
	UFUNCTION(BlueprintCallable)
	bool CanJump();
	// 점프 실행 시 호출 (스태미나 차감 등)
	UFUNCTION(BlueprintCallable)
	void HasJumped();
};
