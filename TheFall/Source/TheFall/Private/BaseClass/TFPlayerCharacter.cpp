// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

/// 플레이어 이동 처리
void ATFPlayerCharacter::Move(const FInputActionValue& Value)
{
	// 입력값을 2D 벡터로 받음 (X: 좌우, Y: 전후)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라 방향 기준으로 이동 방향 계산
		const FRotator Rotation = Controller->GetControlRotation();								// 카메라 회전값
		const FRotator YawRotation(0, Rotation.Yaw, 0);											// 수평 회전만 추출
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 전방 방향
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 오른쪽 방향

		// 입력 벡터 방향으로 이동 처리
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/// 카메라 회전 처리 (마우스 또는 오른쪽 스틱)
void ATFPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);   // 좌우 회전
		AddControllerPitchInput(LookAxisVector.Y); // 상하 회전
	}
}

/// 점프 키를 눌렀을 때 처리
void ATFPlayerCharacter::PlayerJump()
{
	if (ATFCharacter::CanJump()) // 스태미나 등 조건 체크
	{
		ATFCharacter::HasJumped(); // 점프 실행 및 스태미나 차감
	}
}

/// 달리기 키 눌렀을 때 처리
void ATFPlayerCharacter::SprintOn()
{
	SetSprinting(true);
}

/// 달리기 키 뗐을 때 처리
void ATFPlayerCharacter::SprintOff()
{
	SetSprinting(false);
}

/// 웅크리기 키 눌렀을 때 처리
void ATFPlayerCharacter::SneakOn()
{
	SetSneaking(true);
}

/// 웅크리기 키 뗐을 때 처리
void ATFPlayerCharacter::SneakOff()
{
	SetSneaking(false);
}

/// 입력 바인딩 처리
void ATFPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 입력 서브시스템에 매핑 컨텍스트 추가
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 각 액션에 함수 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::PlayerJump);				// 점프 시작 → PlayerJump()
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);					// 점프 키 해제 → 점프 중지
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATFPlayerCharacter::Move);					// 이동
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATFPlayerCharacter::Look);					// 마우스 또는 조이스틱 회전
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::SprintOn);				// 달리기 시작
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATFPlayerCharacter::SprintOff);			// 달리기 해제
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::SneakOn);				// 웅크리기 시작
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Completed, this, &ATFPlayerCharacter::SneakOff);				// 웅크리기 해제
	}
}

/// 시작 시 실행하는 함수
void ATFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ATFCharacter::BeginPlay();
	SaveActorID.Invalidate();
}

/// 생성자: 컴포넌트 초기화 및 설정
ATFPlayerCharacter::ATFPlayerCharacter()
{
	// 콜리전 캡슐 초기화
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 카메라 회전에 따른 캐릭터 회전 비활성화 (카메라는 따로 회전)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 이동 방향을 바라보게 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// 회전 속도
	GetCharacterMovement()->JumpZVelocity = 700.f;							// 점프 높이
	GetCharacterMovement()->AirControl = 0.35f;								// 공중 제어도
	GetCharacterMovement()->MaxWalkSpeed = 500.f;							// 기본 이동 속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;						// 최소 아날로그 이동 속도
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;			// 걷기 감속
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;			// 낙하 감속

	// 카메라 붐 (캐릭터 뒤에서 따라오는 카메라)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent); // 루트에 부착
	CameraBoom->TargetArmLength = 400.0f;       // 거리 조절
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 회전 사용

	// 팔로우 카메라 (CameraBoom 끝에 부착)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);		// 붐 끝에 부착
	FollowCamera->bUsePawnControlRotation = false;									// 카메라는 붐을 통해 회전
}