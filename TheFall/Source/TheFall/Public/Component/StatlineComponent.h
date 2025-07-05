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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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

	float GetCurrent() const
	{
		return Current;
	}
};

/// ĳ������ ���ȵ��� �����ϴ� ������Ʈ Ŭ����
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEFALL_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	class UCharacterMovementComponent* OwningCharMovementComp;

	// ���� �����͵�
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat(100, 100, -2.125);	// �⺻������ ������ �پ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -2.25);	// ������ ��⺸�� �� ������ �پ��

	// ���� ���� �÷��� �� ��ġ
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintCostMulitiplier = 2;				// �޸� �� ���¹̳� �Ҹ� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 125;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 450;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 75;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10;							// ���� �� ���¹̳� �Ҹ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SecondsForStaminaExhuastion = 5;			// ���¹̳� 0 ���� �ð� ����
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentStaminaExhuastion = 0;				// ���� Ż�� ���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StarvingHealthDamagePerSecond = 1;		// ���� Ż�� ���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DehydrationHealthDamagePerSecond = 1;		// ���� 0�� �� ü�� ���ҷ�

	// ���� ������Ʈ �Լ���
	void TickStats(const float& DeltaTime);			// ��ü ���� ƽ ����
	void TickStamina(const float& DeltaTime);		// ���¹̳� ���� ó��
	void TickHunger(const float& DeltaTime);		// ��� ó��
	void TickThirst(const float& DeltaTime);		// ���� ó��
	bool IsValidSprinting();						// ������Ʈ ��ȿ�� Ȯ��

protected:
	virtual void BeginPlay() override;

public:

	// ������
	UStatlineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ܺο��� ĳ���� �̵� ������Ʈ�� ���� ������ �� ���
	UFUNCTION(BlueprintCallable)
	void SetMovementCompReference(UCharacterMovementComponent* Comp);

	// UI ��� �ۼ�Ʈ ���� ��ȸ
	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;
	// �޸� �� �ִ��� ����
	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;
	// �޸��� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetSprinting(const bool& IsSprinting);
	// ��ũ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetSneaking(const bool& IsSneaking);
	// ���� ���� ����
	UFUNCTION(BlueprintCallable)
	bool CanJump();
	// ���� ���� �� ȣ�� (���¹̳� ���� ��)
	UFUNCTION(BlueprintCallable)
	void HasJumped();
};
