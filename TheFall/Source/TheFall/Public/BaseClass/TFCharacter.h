// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SaveActorInterface.h"
#include "TFCharacter.generated.h"

/**
 * ATFCharacter
 * �⺻ ĳ���� Ŭ����. ���� ĳ���� ���(���� ���� ��)�� ó��.
 * �÷��̾�� AI ĳ���� ��� �� Ŭ������ ������� Ȯ�� ����.
 */
UCLASS()
class THEFALL_API ATFCharacter : public ACharacter, public ISaveActorInterface
{
	GENERATED_BODY()

private:
	// ĳ������ ����(ü��, ���¹̳�, ���, ���� ��)�� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	class UStatlineComponent* Statline;

protected:
	// �� ĳ���͸� �����ϰ� �ĺ��ϱ� ���� ����� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	FGuid SaveActorID;

	// �� ĳ���Ͱ� ����� ���¿��� ������ ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	bool WasSpawned = false;

	// ���� ���� �� �ʱ�ȭ ó��
	virtual void BeginPlay() override;

	// ���� ���� �Լ���
	bool CanJump() const;								// ���� ������ �������� Ȯ��
	void HasJumped();									// ���� ���� �� ȣ��� (���¹̳� �Ҹ� �� ó��)

	// �޸��� ���� �Լ���
	bool CanSprint() const;			 					// �޸� �� �ִ� �������� Ȯ��
	void SetSprinting(const bool& IsSprinting);			// �޸��� ���� ����

	// ��ũ���� ���� �Լ�
	void SetSneaking(const bool& IsSneaking);			// ��ũ���� ���� ����

public:
	// ������
	ATFCharacter();

	// �� �����Ӹ��� ȣ��Ǵ� �Լ�
	virtual void Tick(float DeltaTime) override;

	// �Է� ���ε� ����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� �������̽� �����ε�
	FGuid GetActorSaveID_Implementation();						// �� ĳ������ ����� ���� ID ��ȯ
	FSaveActorData GetSaveData_Implementation();				// �� ĳ������ ���� ������ ��ȯ
	void SetActorGUID_Implementation(const FGuid& NewGuid);		// ����� ID ����
};