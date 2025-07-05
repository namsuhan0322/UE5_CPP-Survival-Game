// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/// 각각의 스탯을 프레임마다 갱신하는 함수
void UStatlineComponent::TickStats(const float& DeltaTime)
{
	TickStamina(DeltaTime);			// 스태미나 스탯 갱신
	TickHunger(DeltaTime);			// 허기 스탯 갱신
	TickThirst(DeltaTime);			// 갈증 스탯 갱신

	// 허기나 갈증이 0이면 체력 갱신 중단 
	if (Thirst.GetCurrent() <= 0.0 || Hunger.GetCurrent() <= 0.0)
	{
		return;
	}

	Health.TickStat(DeltaTime);		// 체력 스탯 갱신
}

/// 스태미나 관련 틱 처리
void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	// 탈진 상태일 경우 타이머만 감소시키고 종료
	if (CurrentStaminaExhuastion > 0.0)
	{
		CurrentStaminaExhuastion -= DeltaTime;
		return;
	}

	// 달리는 중이면 스태미나를 빠르게 소모
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - abs(DeltaTime * SprintCostMulitiplier)); // 음수로 적용

		// 스태미나가 0 이하일 경우 스프린트 중단 및 탈진 상태로 진입
		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
			CurrentStaminaExhuastion = SecondsForStaminaExhuastion;
		}
		return;
	}
	// 달리지 않는 중이면 스태미나 자연 회복
	Stamina.TickStat(DeltaTime);
}

/// 허기 관련 틱 처리
void UStatlineComponent::TickHunger(const float& DeltaTime)
{
	// 허기가 0일 경우, 체력을 감소시킴
	if (Hunger.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(StarvingHealthDamagePerSecond * DeltaTime));
		return;
	}

	// 허기 자연 감소
	Hunger.TickStat(DeltaTime);
}

/// 갈증 관련 틱 처리
void UStatlineComponent::TickThirst(const float& DeltaTime)
{
	// 갈증이 0일 경우, 체력을 감소시킴
	if (Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(DehydrationHealthDamagePerSecond * DeltaTime));
		return;
	}

	// 갈증 자연 감소
	Thirst.TickStat(DeltaTime);
}

/// 스프린트 가능한 상황인지 확인 (이동 중이고 공중이 아님)
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

	// 기본 걷기 속도로 설정
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

/// 외부에서 캐릭터 이동 컴포넌트를 참조로 전달
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

/// 스프린트 가능한지 확인 (스태미나가 있어야 가능)
bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0f;
}

/// 스프린트 상태 설정
void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;

	// 웅크리는 중에는 스프린트 못함
	if (bIsSneaking && !bIsSprinting)
	{
		return;
	}

	// 웅크리기 해제
	bIsSneaking = false;

	// 이동 속도 변경
	OwningCharMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

void UStatlineComponent::SetSneaking(const bool& IsSneaking)
{
	/// 웅크리기 상태 설정
	bIsSneaking = IsSneaking;

	// 스프린트 중에는 웅크릴 수 없음
	if (bIsSprinting && !bIsSneaking)
	{
		return;
	}
	// 스프린트 해제
	bIsSprinting = false;

	// 이동 속도 변경
	OwningCharMovementComp->MaxWalkSpeed = bIsSneaking ? SneakSpeed : WalkSpeed;
}

/// 점프 가능 여부
bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost;
}

/// 점프 실행 시 스태미나 차감
void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
