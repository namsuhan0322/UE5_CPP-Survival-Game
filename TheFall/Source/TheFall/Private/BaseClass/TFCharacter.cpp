// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFCharacter.h"
#include "Component/StatlineComponent.h"					// 스탯 컴포넌트를 사용하기 위해 필요함

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// StatlineComponent 생성 및 캐릭터에 부착
	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	// 캐릭터의 이동 컴포넌트를 Statline에 연결
	Statline->SetMovementCompReference(GetCharacterMovement());

	// 고유 식별자 생성
	SaveActorID = FGuid::NewGuid();
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 저장용 식별자가 유효하지 않으면 새로 생성
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

/// 점프 실행 시 호출됨: 스태미나를 차감하고 점프 수행
void ATFCharacter::HasJumped()
{
	Statline->HasJumped();  // 스태미나 차감
	ACharacter::Jump();     // 실제 점프 동작 수행
}

/// 달릴 수 있는지 여부 반환
bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

/// 달리기 상태 설정 (스태미나 상태에 따라 이동 속도 결정)
void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

/// 웅크리기 상태 설정 (달리기와 상호 배제됨)
void ATFCharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임마다 실행되는 로직은 여기에 추가
}

/// 입력 바인딩 설정 (파생 클래스에서 오버라이드 필요)
void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 바인딩 설정은 파생 클래스에서 수행
}

/// 저장용 식별자 반환
FGuid ATFCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

/// 저장할 데이터 반환
FSaveActorData ATFCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;

	Ret.ActorClass = this->GetClass();           // 클래스 정보
	Ret.ActorTransform = this->GetTransform();   // 현재 위치 및 회전
	Ret.WasSpawned = this->WasSpawned;           // 스폰 여부

	return Ret;
}

/// 저장용 식별자 설정
void ATFCharacter::SetActorGUID_Implementation(const FGuid& NewGuid)
{
	// 기존 식별자가 유효하면 무효화
	if (SaveActorID.IsValid())
	{
		SaveActorID.Invalidate();
	}

	// 새 식별자 설정
	SaveActorID = NewGuid;
}