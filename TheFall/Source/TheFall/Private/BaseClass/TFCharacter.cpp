// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// 스탯 컴포넌트를 사용하기 위해 필요함

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent 생성 및 초기화 (캐릭터에 부착)
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