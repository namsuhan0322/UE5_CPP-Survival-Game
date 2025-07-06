// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// ���� ������Ʈ�� ����ϱ� ���� �ʿ���

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent ���� �� �ʱ�ȭ (ĳ���Ϳ� ����)
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	// ĳ������ �̵� ������Ʈ�� Statline�� ����
	Statline->SetMovementCompReference(GetCharacterMovement());

	SaveActorID = FGuid::NewGuid();
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!SaveActorID.IsValid())
	{
		SaveActorID = FGuid::NewGuid();
	}
}

/// ���� ������ �������� Ȯ��
bool ATFCharacter::CanJump() const
{
	return Statline->CanJump();
}

/// ���� ���� �� ȣ��: ���¹̳� ���� �� ���� ����
void ATFCharacter::HasJumped()
{
	Statline->HasJumped();  // ���¹̳� ����
	ACharacter::Jump();     // ���� ���� ó��
}

/// �޸��� �������� ���� ��ȯ
bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

/// �޸��� ���� ���� (���¹̳� ���¿� ���� �̵� �ӵ� ����)
void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

/// ��ũ���� ���� ���� (������Ʈ�� ��ȣ ����)
void ATFCharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// �Է� ���ε� ���� (�Ļ� Ŭ�������� �������̵� �ʿ�)
void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGuid ATFCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

FSaveActorData ATFCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;

	Ret.ActorClass = this->GetClass();
	Ret.ActorTransform = this->GetTransform();
	Ret.WasSpawned = this->WasSpawned;

	return Ret;
}