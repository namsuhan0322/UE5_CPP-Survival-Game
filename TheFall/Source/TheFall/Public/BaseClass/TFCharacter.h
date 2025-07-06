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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	FGuid SaveActorID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	bool WasSpawned = false;

public:
	ATFCharacter();

protected:
	virtual void BeginPlay() override;

	// ���� ����
	bool CanJump() const;								// ���� �������� ���� Ȯ��
	void HasJumped();									// ���� �� ȣ�� (���¹̳� �Ҹ� ����)

	// �޸��� ����
	bool CanSprint() const;								// �޸��� ���� ���� Ȯ��
	void SetSprinting(const bool& IsSprinting);			// �޸��� ���� ����

	// ��ũ���� ����	
	void SetSneaking(const bool& IsSneaking);			// ��ũ���� ���� ����

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FGuid GetActorSaveID_Implementation();
	FSaveActorData GetSaveData_Implementation();
};
