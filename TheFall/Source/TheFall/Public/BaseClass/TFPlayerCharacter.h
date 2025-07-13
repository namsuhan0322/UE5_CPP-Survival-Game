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

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionTrigger;

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

	// ��ȣ�ۿ� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
#pragma endregion

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bEnableRayTrace = false;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> InteractableActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float InteractionTraceLength = 200;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AActor* InteractionActor = nullptr;

	void TraceForInteraction();

protected:

	void Move(const FInputActionValue& Value);				// �̵� ó�� �Լ�
	void Look(const FInputActionValue& Value);				// �þ� ȸ�� ó�� �Լ�
	void PlayerJump();										// ���� ����
	void SprintOn();										// �޸��� Ű ����
	void SprintOff();										// �޸��� Ű ����
	void SneakOn();											// ��ũ���� Ű ����
	void SneakOff();										// ��ũ���� Ű ����
	void OnInteract();										// ��ȣ�ۿ� Ű

	// �Է� ������Ʈ ���ε�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� ���� �� �ʱ�ȭ
	virtual void BeginPlay();

public:
	ATFPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void OnInteractionTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateInteractionText();
	void UpdateInteractionText_Implementation();
};
