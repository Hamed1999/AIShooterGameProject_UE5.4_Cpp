// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Characters/SoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Gun.h"
#include "AIs/EnemyAIController.h"
#include "Components/CapsuleComponent.h"


bool ASoldierCharacter::IsDead()
{
	return Health <= 0;
}

float ASoldierCharacter::GetPlayerHealth()
{
	return Health / MaxHealth;
}

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

void ASoldierCharacter::SpawnGun()
{
	if (GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		if (Team == ESoldierTeam::Peace)
			GetMesh()->HideBoneByName(FName("gun"), PBO_None);
		else
		{
			Gun->HideMesh();
		}
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GunSocket"));
		Gun->SetOwner(this);
	}
}

void ASoldierCharacter::SetTeamId()
{
	if (Team == ESoldierTeam::Peace)
		TeamId = FGenericTeamId(0);
	else if (Team == ESoldierTeam::Devil)
		TeamId = FGenericTeamId(1);
	else
		TeamId = FGenericTeamId(255);
	SetGenericTeamId(TeamId);
}

void ASoldierCharacter::SetAIController()
{
	if (!bIsLeader)
		AIControllerClass = AEnemyAIController::StaticClass();
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
	SpawnGun();
	Health = MaxHealth;
	SetTeamId();
	SetAIController();
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
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASoldierCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ASoldierCharacter::StopJumping);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ASoldierCharacter::Fire);
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

void ASoldierCharacter::Fire()
{
	bIsFiring = true;
	if (Gun)
		Gun->Shoot();

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
	{
		bIsFiring = false;
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.01, false);
}

float ASoldierCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	if (Health <= 0)
		HandleDeath();
	UE_LOG(LogTemp, Error, TEXT("Health of %s: %f"), *GetName(), Health);
	return ActualDamage;
}

void ASoldierCharacter::HandleDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	if (Team == ESoldierTeam::Peace && bIsLeader)
	{
		GetWorld()->GetFirstPlayerController()->StartSpectatingOnly();
	}
	
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDel;
	DestroyTimerDel.BindLambda([&]()
	{
		Destroy();
		Gun->Destroy();
	});
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDel, 7.0, false);
}

FGenericTeamId ASoldierCharacter::GetGenericTeamId() const
{
	return TeamId;
}