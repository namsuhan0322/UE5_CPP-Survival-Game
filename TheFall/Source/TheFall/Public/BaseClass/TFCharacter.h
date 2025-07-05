// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TFCharacter.generated.h"

UCLASS()
class THEFALL_API ATFCharacter : public ACharacter
{
	GENERATED_BODY()

	// ĳ������ ����(ü��, ���¹̳�, ���, ���� ��)�� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStatlineComponent* Statline;
public:
	ATFCharacter();

protected:
	virtual void BeginPlay() override;

	bool CanJump() const;
	void HasJumped();

	bool CanSprint() const;
	void SetSprinting(const bool& IsSprinting);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
