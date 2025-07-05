// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TFCharacter.h"
#include "TFPlayerCharacter.generated.h"

// 전방 선언: 필요한 클래스 및 구조체를 미리 선언
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * ATFPlayerCharacter
 * 플레이어가 조작하는 캐릭터 클래스.
 * 입력 액션, 카메라, 이동/회전 처리 기능 포함.
 */
UCLASS(config = Game)
class THEFALL_API ATFPlayerCharacter : public ATFCharacter
{
	GENERATED_BODY()

private:
	// 카메라 붐: 카메라를 캐릭터 뒤에 부착시키는 스프링 암
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// 팔로우 카메라: 플레이어를 따라다니는 실제 카메라
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma region Input
	// 입력 매핑 컨텍스트 (Enhanced Input 사용 시 필수)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// 점프 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// 이동 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// 카메라 회전 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 달리기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	// 웅크리기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SneakAction;

#pragma endregion

protected:

	void Move(const FInputActionValue& Value);				// 이동 처리 함수
	void Look(const FInputActionValue& Value);				// 시야 회전 처리 함수
	void PlayerJump();										// 점프 시작
	void SprintOn();										// 달리기 키 눌림
	void SprintOff();										// 달리기 키 해제
	void SneakOn();											// 웅크리기 키 눌림
	void SneakOff();										// 웅크리기 키 해제

	// 입력 컴포넌트 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 게임 시작 시 초기화
	virtual void BeginPlay();

public:
	ATFPlayerCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
