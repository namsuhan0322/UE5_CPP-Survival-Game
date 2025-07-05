// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

/// �ٽ� ������ ������ �����ϴ� ������ (�������Ʈ������ ��� ����)
UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName = "Health"),  // ü��
	CS_STAMINA UMETA(DisplayName = "Stamina"), // ���¹̳�
	CS_HUNGER UMETA(DisplayName = "Hunger"),   // ���
	CS_THIRST UMETA(DisplayName = "Thirst")    // ����
};

/// �� ������ �����ϴ� ����ü (�������Ʈ ��� ����)
USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	// ���� ��ġ
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;
	// �ִ� ��ġ
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;
	// �ʴ� ��ȭ��
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

public:

	// �⺻ ������
	FCoreStat() {}

	// ������ ������ �ʱ�ȭ�ϴ� ������
	FCoreStat(const float& current, const float& max, const float& tick)
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
	}

	// ������ ������ ������ ����/���ҽ�Ŵ
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	// �ܺο��� ������ �縸ŭ ��ġ�� ����
	void Adjust(const float& Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Max);
	}

	// ���� ��ġ�� ����� ��ȯ (UI ������ ��� ��� ����)
	float Percentile() const
	{
		return Current / Max;
	}

	// �ʴ� ��ȭ�� ����
	void AbjustTick(const float& NewTick)
	{
		PerSecondTick = NewTick;
	}
};

/// ĳ������ ���ȵ��� �����ϴ� ������Ʈ Ŭ����
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEFALL_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// ü�� ����
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	// ���¹̳� ����
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	// ��� ���� (�ʴ� ����)
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat(100, 100, -0.125);
	// ���� ���� (�ʴ� �� ������ ����)
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -0.25);

	// ��� ������ �����Ӹ��� �����ϴ� ���� �Լ�
	void TickStats(const float& DeltaTime);

public:	
	// ������
	UStatlineComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ܺο��� Ư�� ������ �ۼ�Ʈ ���� ��� �Լ� (UI �� � ��� ����)
	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;
};
