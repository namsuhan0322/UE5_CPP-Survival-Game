// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/// 각각의 스탯을 프레임마다 갱신하는 함수
void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);   // 체력 스탯 갱신
	TickStamina(DeltaTime);		  // 스태미나 스탯 갱신
	Hunger.TickStat(DeltaTime);   // 허기 스탯 갱신
	Thirst.TickStat(DeltaTime);   // 갈증 스탯 갱신
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

/// 매 프레임마다 호출되는 컴포넌트의 틱 함수
void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 게임이 일시정지 상태가 아니라면 스탯을 갱신
	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UStatlineComponent::SetMovementCompReference(UCharacterMovementComponent* Comp)
{
	OwningCharMovementComp = Comp;
}

/// 외부에서 스탯 퍼센트 값을 요청할 때 사용 (UI 게이지 등에 활용 가능)
float UStatlineComponent::GetStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:
		return Health.Percentile(); // 체력 비율 반환
	case ECoreStat::CS_HUNGER:
		return Hunger.Percentile(); // 허기 비율 반환
	case ECoreStat::CS_STAMINA:
		return Stamina.Percentile(); // 스태미나 비율 반환
	case ECoreStat::CS_THIRST:
		return Thirst.Percentile(); // 갈증 비율 반환
	default:
		break;
	}

	// 해당되지 않는 스탯일 경우 -1 반환
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
