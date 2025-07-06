// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// 스탯 컴포넌트를 사용하기 위해 필요함

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent 생성 및 초기화 (캐릭터에 부착)
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	// 캐릭터의 이동 컴포넌트를 Statline에 연결
	Statline->SetMovementCompReference(GetCharacterMovement());

	SaveActorID = FGuid::NewGuid();
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!SaveActorID.IsValid())
	{
		SaveActorID = FGuid::NewGuid();
	}
}

/// 점프 가능한 상태인지 확인
bool ATFCharacter::CanJump() const
{
	return Statline->CanJump();
}

/// 점프 실행 시 호출: 스태미나 차감 후 점프 실행
void ATFCharacter::HasJumped()
{
	Statline->HasJumped();  // 스태미나 차감
	ACharacter::Jump();     // 실제 점프 처리
}

/// 달리기 가능한지 여부 반환
bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

/// 달리기 상태 설정 (스태미나 상태에 따라 이동 속도 조절)
void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

/// 웅크리기 상태 설정 (스프린트와 상호 배제)
void ATFCharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// 입력 바인딩 설정 (파생 클래스에서 오버라이드 필요)
void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGuid ATFCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

FSaveActorData ATFCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;

	Ret.ActorClass = this->GetClass();
	Ret.ActorTransform = this->GetTransform();
	Ret.WasSpawned = this->WasSpawned;

	return Ret;
}