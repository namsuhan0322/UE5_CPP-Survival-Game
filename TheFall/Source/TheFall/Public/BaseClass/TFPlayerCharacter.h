// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TFCharacter.h"
#include "TFPlayerCharacter.generated.h"

// ���� ����: �ʿ��� Ŭ���� �� ����ü�� �̸� ����
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * ATFPlayerCharacter
 * �÷��̾ �����ϴ� ĳ���� Ŭ����.
 * �Է� �׼�, ī�޶�, �̵�/ȸ�� ó�� ��� ����.
 */
UCLASS(config = Game)
class THEFALL_API ATFPlayerCharacter : public ATFCharacter
{
	GENERATED_BODY()

private:
	// ī�޶� ��: ī�޶� ĳ���� �ڿ� ������Ű�� ������ ��
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// �ȷο� ī�޶�: �÷��̾ ����ٴϴ� ���� ī�޶�
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma region Input
	// �Է� ���� ���ؽ�Ʈ (Enhanced Input ��� �� �ʼ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// �̵� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// ī�޶� ȸ�� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// �޸��� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	// ��ũ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SneakAction;

#pragma endregion

protected:

	void Move(const FInputActionValue& Value);				// �̵� ó�� �Լ�
	void Look(const FInputActionValue& Value);				// �þ� ȸ�� ó�� �Լ�
	void PlayerJump();										// ���� ����
	void SprintOn();										// �޸��� Ű ����
	void SprintOff();										// �޸��� Ű ����
	void SneakOn();											// ��ũ���� Ű ����
	void SneakOff();										// ��ũ���� Ű ����

	// �Է� ������Ʈ ���ε�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� ���� �� �ʱ�ȭ
	virtual void BeginPlay();

public:
	ATFPlayerCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
