// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// ���� ������Ʈ�� ����ϱ� ���� �ʿ���

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent ���� �� ĳ���Ϳ� ����
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	// ĳ������ �̵� ������Ʈ�� Statline�� ����
	Statline->SetMovementCompReference(GetCharacterMovement());

	// ���� �ĺ��� ����
	SaveActorID = FGuid::NewGuid();
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ����� �ĺ��ڰ� ��ȿ���� ������ ���� ����
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

/// ���� ���� �� ȣ���: ���¹̳��� �����ϰ� ���� ����
void ATFCharacter::HasJumped()
{
	Statline->HasJumped();  // ���¹̳� ����
	ACharacter::Jump();     // ���� ���� ���� ����
}

/// �޸� �� �ִ��� ���� ��ȯ
bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

/// �޸��� ���� ���� (���¹̳� ���¿� ���� �̵� �ӵ� ����)
void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

/// ��ũ���� ���� ���� (�޸���� ��ȣ ������)
void ATFCharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� �����Ӹ��� ����Ǵ� ������ ���⿡ �߰�
}

/// �Է� ���ε� ���� (�Ļ� Ŭ�������� �������̵� �ʿ�)
void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �Է� ���ε� ������ �Ļ� Ŭ�������� ����
}

/// ����� �ĺ��� ��ȯ
FGuid ATFCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

/// ������ ������ ��ȯ
FSaveActorData ATFCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;

	Ret.ActorClass = this->GetClass();           // Ŭ���� ����
	Ret.ActorTransform = this->GetTransform();   // ���� ��ġ �� ȸ��
	Ret.WasSpawned = this->WasSpawned;           // ���� ����

	return Ret;
}

/// ����� �ĺ��� ����
void ATFCharacter::SetActorGUID_Implementation(const FGuid& NewGuid)
{
	// ���� �ĺ��ڰ� ��ȿ�ϸ� ��ȿȭ
	if (SaveActorID.IsValid())
	{
		SaveActorID.Invalidate();
	}

	// �� �ĺ��� ����
	SaveActorID = NewGuid;
}