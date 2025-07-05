// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// ���� ������Ʈ�� ����ϱ� ���� �ʿ���

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent ���� �� �ʱ�ȭ (ĳ���Ϳ� ����)
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}