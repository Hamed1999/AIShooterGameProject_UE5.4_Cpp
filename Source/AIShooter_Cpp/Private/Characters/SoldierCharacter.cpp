// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Characters/SoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"


void ASoldierCharacter::CreateSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
}

void ASoldierCharacter::CreateCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraComponent->SetupAttachment(SpringArm);
}

ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateSpringArm();
	CreateCamera();
}

void ASoldierCharacter::CreateMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_DefaultInputs, 0);
		}
	}
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
}

void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoldierCharacter::BindEnhancedInputActions(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ASoldierCharacter::MoveForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ASoldierCharacter::MoveRight);
		EnhancedInputComponent->BindAction(IA_TurnCamerView, ETriggerEvent::Triggered, this, &ASoldierCharacter::TurnView);
	}
}

void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindEnhancedInputActions(PlayerInputComponent);
}

void ASoldierCharacter::MoveForward(const FInputActionValue& InputActionValue)
{
	float InputValue = InputActionValue.Get<float>();
	AddMovementInput(GetActorForwardVector() * InputValue);
}

void ASoldierCharacter::MoveRight(const FInputActionValue& InputActionValue)
{
	float InputValue = InputActionValue.Get<float>();
	AddMovementInput(GetActorRightVector() * InputValue);
}

void ASoldierCharacter::TurnView(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-1* InputValue.Y);
}