// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SaveActorInterface.h"
#include "TFCharacter.generated.h"

/**
 * ATFCharacter
 * 기본 캐릭터 클래스. 공용 캐릭터 기능(스탯 관리 등)을 처리.
 * 플레이어와 AI 캐릭터 모두 이 클래스를 기반으로 확장 가능.
 */
UCLASS()
class THEFALL_API ATFCharacter : public ACharacter, public ISaveActorInterface
{
	GENERATED_BODY()

private:
	// 캐릭터의 스탯(체력, 스태미나, 허기, 갈증 등)을 관리하는 컴포넌트
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

	// 점프 관련
	bool CanJump() const;								// 점프 가능한지 여부 확인
	void HasJumped();									// 점프 시 호출 (스태미나 소모 포함)

	// 달리기 관련
	bool CanSprint() const;								// 달리기 가능 여부 확인
	void SetSprinting(const bool& IsSprinting);			// 달리기 상태 설정

	// 웅크리기 관련	
	void SetSneaking(const bool& IsSneaking);			// 웅크리기 상태 설정

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FGuid GetActorSaveID_Implementation();
	FSaveActorData GetSaveData_Implementation();
};
