// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interface/InteractionInterface.h"
#include "Logger.h"

void ATFPlayerCharacter::TraceForInteraction()
{
	FCollisionQueryParams LTParmas = FCollisionQueryParams(FName(TEXT("InteractionTrace")), true, this);
	LTParmas.bReturnPhysicalMaterial = false;
	LTParmas.bReturnFaceIndex = false;
	GetWorld()->DebugDrawTraceTag = DEBUG_SHOW_INTERACTION_TRACE ? TEXT("InteractionTrace") : TEXT("NONE");
	FHitResult LTHit(ForceInit);
	FVector LTStart = FollowCamera->GetComponentLocation();
	float SearchLength = (FollowCamera->GetComponentLocation() - CameraBoom->GetComponentLocation()).Length();
	SearchLength += InteractionTraceLength;
	FVector LTEnd = (FollowCamera->GetForwardVector() * SearchLength) + LTStart;

	GetWorld()->LineTraceSingleByChannel(LTHit, LTStart, LTEnd, ECC_Visibility, LTParmas);

	UpdateInteractionText_Implementation();
	if (!LTHit.bBlockingHit || !LTHit.GetActor()->Implements<UInteractionInterface>())
	{
		InteractionActor = nullptr;
		return;
	}
	InteractionActor = LTHit.GetActor();
}

/// �÷��̾� �̵� ó��
void ATFPlayerCharacter::Move(const FInputActionValue& Value)
{
	// �Է°��� 2D ���ͷ� ���� (X: �¿�, Y: ����)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// ī�޶� ���� �������� �̵� ���� ���
		const FRotator Rotation = Controller->GetControlRotation();								// ī�޶� ȸ����
		const FRotator YawRotation(0, Rotation.Yaw, 0);											// ���� ȸ���� ����
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// ���� ����
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// ������ ����

		// �Է� ���� �������� �̵� ó��
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/// ī�޶� ȸ�� ó�� (���콺 �Ǵ� ������ ��ƽ)
void ATFPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);   // �¿� ȸ��
		AddControllerPitchInput(LookAxisVector.Y); // ���� ȸ��
	}
}

/// ���� Ű�� ������ �� ó��
void ATFPlayerCharacter::PlayerJump()
{
	if (ATFCharacter::CanJump()) // ���¹̳� �� ���� üũ
	{
		ATFCharacter::HasJumped(); // ���� ���� �� ���¹̳� ����
	}
}

/// �޸��� Ű ������ �� ó��
void ATFPlayerCharacter::SprintOn()
{
	SetSprinting(true);
}

/// �޸��� Ű ���� �� ó��
void ATFPlayerCharacter::SprintOff()
{
	SetSprinting(false);
}

/// ��ũ���� Ű ������ �� ó��
void ATFPlayerCharacter::SneakOn()
{
	SetSneaking(true);
}

/// ��ũ���� Ű ���� �� ó��
void ATFPlayerCharacter::SneakOff()
{
	SetSneaking(false);
}

void ATFPlayerCharacter::OnInteract()
{
	if (InteractionActor == nullptr)
	{
		return;
	}
	IInteractionInterface* Inter = Cast<IInteractionInterface>(InteractionActor);
	if (Inter == nullptr)
	{
		Logger::GetInstance()->AddMessage("ATFPlayerCharacter::OnInteract - Failed to cast to InteractionInterface", ERRORLEVEL::EL_ERROR);
		return;
	}
	Inter->Execute_Interact(InteractionActor, this);
}

/// �Է� ���ε� ó��
void ATFPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// �Է� ����ý��ۿ� ���� ���ؽ�Ʈ �߰�
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// �� �׼ǿ� �Լ� ���ε�
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::PlayerJump);				// ���� ���� �� PlayerJump()
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);					// ���� Ű ���� �� ���� ����
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATFPlayerCharacter::Move);					// �̵�
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATFPlayerCharacter::Look);					// ���콺 �Ǵ� ���̽�ƽ ȸ��
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::SprintOn);				// �޸��� ����
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATFPlayerCharacter::SprintOff);			// �޸��� ����
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Started, this, &ATFPlayerCharacter::SneakOn);				// ��ũ���� ����
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Completed, this, &ATFPlayerCharacter::SneakOff);				// ��ũ���� ����
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ATFPlayerCharacter::OnInteract);		// ��ȣ�ۿ�
	}
}

/// ���� �� �����ϴ� �Լ�
void ATFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ATFCharacter::BeginPlay();
	SaveActorID.Invalidate();
}

/// ������: ������Ʈ �ʱ�ȭ �� ����
ATFPlayerCharacter::ATFPlayerCharacter()
{
	// �ݸ��� ĸ�� �ʱ�ȭ
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// ī�޶� ȸ���� ���� ĳ���� ȸ�� ��Ȱ��ȭ (ī�޶�� ���� ȸ��)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �̵� ������ �ٶ󺸰� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// ȸ�� �ӵ�
	GetCharacterMovement()->JumpZVelocity = 700.f;							// ���� ����
	GetCharacterMovement()->AirControl = 0.35f;								// ���� ���
	GetCharacterMovement()->MaxWalkSpeed = 500.f;							// �⺻ �̵� �ӵ�
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;						// �ּ� �Ƴ��α� �̵� �ӵ�
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;			// �ȱ� ����
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;			// ���� ����

	// ī�޶� �� (ĳ���� �ڿ��� ������� ī�޶�)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent); // ��Ʈ�� ����
	CameraBoom->TargetArmLength = 400.0f;       // �Ÿ� ����
	CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ� ȸ�� ���

	// �ȷο� ī�޶� (CameraBoom ���� ����)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);		// �� ���� ����
	FollowCamera->bUsePawnControlRotation = false;									// ī�޶�� ���� ���� ȸ��

	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Trigger Volume"));
	InteractionTrigger->SetupAttachment(RootComponent);
	InteractionTrigger->SetRelativeScale3D(FVector(10));
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATFPlayerCharacter::OnInteractionTriggerOverlapBegin);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ATFPlayerCharacter::OnInteractionTriggerOverlapEnd);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void ATFPlayerCharacter::Tick(float DeltaTime)
{
	if (bEnableRayTrace)
	{
		TraceForInteraction();
	}
}

void ATFPlayerCharacter::OnInteractionTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UInteractionInterface>())
	{
		return;
	}
	InteractableActors.Add(OtherActor);
	bEnableRayTrace = true;
}

void ATFPlayerCharacter::OnInteractionTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex)
{
	if (!OtherActor->Implements<UInteractionInterface>())
	{
		return;
	}
	InteractableActors.Remove(OtherActor);
	bEnableRayTrace = InteractableActors.Num() > 0;
}

void ATFPlayerCharacter::UpdateInteractionText_Implementation()
{
	UpdateInteractionText();
}
