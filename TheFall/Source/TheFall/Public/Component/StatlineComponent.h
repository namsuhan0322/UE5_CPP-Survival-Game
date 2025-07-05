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
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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
};

/// 캐릭터의 스탯들을 관리하는 컴포넌트 클래스
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEFALL_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// 체력 스탯
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	// 스태미나 스탯
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	// 허기 스탯 (초당 감소)
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat(100, 100, -0.125);
	// 갈증 스탯 (초당 더 빠르게 감소)
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -0.25);

	// 모든 스탯을 프레임마다 갱신하는 내부 함수
	void TickStats(const float& DeltaTime);

public:	
	// 생성자
	UStatlineComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 외부에서 특정 스탯의 퍼센트 값을 얻는 함수 (UI 바 등에 사용 가능)
	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;
};
