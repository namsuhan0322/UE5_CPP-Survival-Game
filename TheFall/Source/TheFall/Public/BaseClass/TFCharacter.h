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

protected:
	// 이 캐릭터를 고유하게 식별하기 위한 저장용 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	FGuid SaveActorID;

	// 이 캐릭터가 저장된 상태에서 스폰된 것인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	bool WasSpawned = false;

	// 게임 시작 시 초기화 처리
	virtual void BeginPlay() override;

	// 점프 관련 함수들
	bool CanJump() const;								// 점프 가능한 상태인지 확인
	void HasJumped();									// 실제 점프 시 호출됨 (스태미나 소모 등 처리)

	// 달리기 관련 함수들
	bool CanSprint() const;			 					// 달릴 수 있는 상태인지 확인
	void SetSprinting(const bool& IsSprinting);			// 달리기 상태 설정

	// 웅크리기 관련 함수
	void SetSneaking(const bool& IsSneaking);			// 웅크리기 상태 설정

public:
	// 생성자
	ATFCharacter();

	// 매 프레임마다 호출되는 함수
	virtual void Tick(float DeltaTime) override;

	// 입력 바인딩 설정
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 저장 인터페이스 구현부들
	FGuid GetActorSaveID_Implementation();						// 이 캐릭터의 저장용 고유 ID 반환
	FSaveActorData GetSaveData_Implementation();				// 이 캐릭터의 저장 데이터 반환
	void SetActorGUID_Implementation(const FGuid& NewGuid);		// 저장용 ID 설정
};