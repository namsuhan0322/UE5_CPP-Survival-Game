// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatlineComponent.h"

/// ������ ������ �����Ӹ��� �����ϴ� �Լ�
void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);   // ü�� ���� ����
	Stamina.TickStat(DeltaTime);  // ���¹̳� ���� ����
	Hunger.TickStat(DeltaTime);   // ��� ���� ����
	Thirst.TickStat(DeltaTime);   // ���� ���� ����
}

UStatlineComponent::UStatlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
}

void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
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