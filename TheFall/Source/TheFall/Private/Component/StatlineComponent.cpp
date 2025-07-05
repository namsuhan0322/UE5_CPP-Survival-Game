// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/// ������ ������ �����Ӹ��� �����ϴ� �Լ�
void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);   // ü�� ���� ����
	TickStamina(DeltaTime);		  // ���¹̳� ���� ����
	Hunger.TickStat(DeltaTime);   // ��� ���� ����
	Thirst.TickStat(DeltaTime);   // ���� ���� ����
}

void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMulitiplier));
		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
		}
		return;
	}
	Stamina.TickStat(DeltaTime);
}

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

bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0f;
}

void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;
	OwningCharMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost;
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
