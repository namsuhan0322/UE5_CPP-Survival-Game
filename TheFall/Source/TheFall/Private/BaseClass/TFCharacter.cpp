// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// ���� ������Ʈ�� ����ϱ� ���� �ʿ���

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent ���� �� �ʱ�ȭ (ĳ���Ϳ� ����)
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementCompReference(GetCharacterMovement());
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

}

bool ATFCharacter::CanJump() const
{
	return Statline->CanJump();
}

void ATFCharacter::HasJumped()
{
	Statline->HasJumped();
	ACharacter::Jump();
}

bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}