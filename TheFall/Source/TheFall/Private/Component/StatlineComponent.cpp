// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/// ������ ������ �����Ӹ��� �����ϴ� �Լ�
void UStatlineComponent::TickStats(const float& DeltaTime)
{
	TickStamina(DeltaTime);			// ���¹̳� ���� ����
	TickHunger(DeltaTime);			// ��� ���� ����
	TickThirst(DeltaTime);			// ���� ���� ����

	// ��⳪ ������ 0�̸� ü�� ���� �ߴ� 
	if (Thirst.GetCurrent() <= 0.0 || Hunger.GetCurrent() <= 0.0)
	{
		return;
	}

	Health.TickStat(DeltaTime);		// ü�� ���� ����
}

/// ���¹̳� ���� ƽ ó��
void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	// Ż�� ������ ��� Ÿ�̸Ӹ� ���ҽ�Ű�� ����
	if (CurrentStaminaExhuastion > 0.0)
	{
		CurrentStaminaExhuastion -= DeltaTime;
		return;
	}

	// �޸��� ���̸� ���¹̳��� ������ �Ҹ�
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - abs(DeltaTime * SprintCostMulitiplier)); // ������ ����

		// ���¹̳��� 0 ������ ��� ������Ʈ �ߴ� �� Ż�� ���·� ����
		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
			CurrentStaminaExhuastion = SecondsForStaminaExhuastion;
		}
		return;
	}
	// �޸��� �ʴ� ���̸� ���¹̳� �ڿ� ȸ��
	Stamina.TickStat(DeltaTime);
}

/// ��� ���� ƽ ó��
void UStatlineComponent::TickHunger(const float& DeltaTime)
{
	// ��Ⱑ 0�� ���, ü���� ���ҽ�Ŵ
	if (Hunger.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(StarvingHealthDamagePerSecond * DeltaTime));
		return;
	}

	// ��� �ڿ� ����
	Hunger.TickStat(DeltaTime);
}

/// ���� ���� ƽ ó��
void UStatlineComponent::TickThirst(const float& DeltaTime)
{
	// ������ 0�� ���, ü���� ���ҽ�Ŵ
	if (Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(DehydrationHealthDamagePerSecond * DeltaTime));
		return;
	}

	// ���� �ڿ� ����
	Thirst.TickStat(DeltaTime);
}

/// ������Ʈ ������ ��Ȳ���� Ȯ�� (�̵� ���̰� ������ �ƴ�)
bool UStatlineComponent::IsValidSprinting()
{
	return OwningCharMovementComp->Velocity.Length() > WalkSpeed && !OwningCharMovementComp->IsFalling();
}

UStatlineComponent::UStatlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();

	// �⺻ �ȱ� �ӵ��� ����
	OwningCharMovementComp->MaxWalkSpeed = WalkSpeed;
}

/// �� �����Ӹ��� ȣ��Ǵ� ������Ʈ�� ƽ �Լ�
void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ������ �Ͻ����� ���°� �ƴ϶�� ������ ����
	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

/// �ܺο��� ĳ���� �̵� ������Ʈ�� ������ ����
void UStatlineComponent::SetMovementCompReference(UCharacterMovementComponent* Comp)
{
	OwningCharMovementComp = Comp;
}

/// �ܺο��� ���� �ۼ�Ʈ ���� ��û�� �� ��� (UI ������ � Ȱ�� ����)
float UStatlineComponent::GetStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:
		return Health.Percentile(); // ü�� ���� ��ȯ
	case ECoreStat::CS_HUNGER:
		return Hunger.Percentile(); // ��� ���� ��ȯ
	case ECoreStat::CS_STAMINA:
		return Stamina.Percentile(); // ���¹̳� ���� ��ȯ
	case ECoreStat::CS_THIRST:
		return Thirst.Percentile(); // ���� ���� ��ȯ
	default:
		break;
	}

	// �ش���� �ʴ� ������ ��� -1 ��ȯ
	return -1;
}

/// ������Ʈ �������� Ȯ�� (���¹̳��� �־�� ����)
bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0f;
}

/// ������Ʈ ���� ����
void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;

	// ��ũ���� �߿��� ������Ʈ ����
	if (bIsSneaking && !bIsSprinting)
	{
		return;
	}

	// ��ũ���� ����
	bIsSneaking = false;

	// �̵� �ӵ� ����
	OwningCharMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

void UStatlineComponent::SetSneaking(const bool& IsSneaking)
{
	/// ��ũ���� ���� ����
	bIsSneaking = IsSneaking;

	// ������Ʈ �߿��� ��ũ�� �� ����
	if (bIsSprinting && !bIsSneaking)
	{
		return;
	}
	// ������Ʈ ����
	bIsSprinting = false;

	// �̵� �ӵ� ����
	OwningCharMovementComp->MaxWalkSpeed = bIsSneaking ? SneakSpeed : WalkSpeed;
}

/// ���� ���� ����
bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost;
}

/// ���� ���� �� ���¹̳� ����
void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
