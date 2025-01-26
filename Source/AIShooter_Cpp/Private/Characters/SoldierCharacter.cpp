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
#include "ShooterGameMode.h"
#include "Blueprint/UserWidget.h"

bool ASoldierCharacter::IsDead()
{
	return Health <= 0;
}

float ASoldierCharacter::GetPlayerHealth()
{
	return Health / MaxHealth;
}

void ASoldierCharacter::RemoveSniperViewWidget()
{
	if (WBP_SniperView)
		if (WBP_SniperView->IsInViewport())
			WBP_SniperView->RemoveFromParent();
}

void ASoldierCharacter::Reload()
{
	if(bIsReloading) return;
	bIsReloading = true;
	bCanChangeGun = false;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		GetActiveGun()->Reload();
		bIsReloading = false;
		bCanChangeGun = true;
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate,Guns[ActiveGunIndex]->ReloadTime, false);
}

int ASoldierCharacter::GetMagAmmo()
{
	return (GunIsSafe() ? GetActiveGun()->CurrentMagAmmo :  0);
}

int ASoldierCharacter::GetAmmo()
{
	return (GunIsSafe() ? GetActiveGun()->CurrentAmmo :  0);
}

void ASoldierCharacter::SetAmmo(int Ammo)
{
	if (GunIsSafe())
		GetActiveGun()->SetAmmo(Ammo);
}

void ASoldierCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
	MaxHealth = NewHealth;
}

AGun* ASoldierCharacter::GetActiveGun()
{
	if(GunIsSafe())
		return Guns[ActiveGunIndex];
	return nullptr;
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

void ASoldierCharacter::SetSniperViewClass()
{
	ConstructorHelpers::FClassFinder<UUserWidget> SniperViewRef(TEXT("/Game/Widgets/WBP_SniperView"));
	if (SniperViewRef.Succeeded())
		SniperViewClass = SniperViewRef.Class.Get();
}

ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	/**
	 * Team ID Should be Set Before Setting the AI Controller Class
	 */
	SetTeamId();
	CreateSpringArm();
	CreateCamera();
	SetAIControllerClass();
	SetSniperViewClass();
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
	Guns.SetNum(GunClasses.Num());
	for (int i = 0; i < GunClasses.Num(); i++)
	{
		
		if (auto GunClass = GunClasses[i])
		{
			Guns[i] = GetWorld()->SpawnActor<AGun>(GunClass);
			if (Team == ESoldierTeam::Peace)
				GetMesh()->HideBoneByName(FName("gun"), PBO_None);
			else
			{
				Guns[i]->HideMesh();
			}
			Guns[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GunSocket"));
			Guns[i]->SetOwner(this);
		}
	}
	ManageGuns();
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

void ASoldierCharacter::SetAIControllerClass()
{
	if(!bIsLeader)
		AIControllerClass = AEnemyAIController::StaticClass();
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetTeamId();
	CreateMappingContext();
	SpawnGun();
	Health = MaxHealth;
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
		EnhancedInputComponent->BindAction(IA_ChangeGun, ETriggerEvent::Triggered, this, &ASoldierCharacter::ChangeGun);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ASoldierCharacter::Zoom);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Triggered, this, &ASoldierCharacter::Reload);
	}
}

void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindEnhancedInputActions(PlayerInputComponent);
}

void ASoldierCharacter::MoveForward(const FInputActionValue& InputActionValue)
{
	if(bIsReloading) return;
	float InputValue = InputActionValue.Get<float>();
	AddMovementInput(GetActorForwardVector() * InputValue);
}

void ASoldierCharacter::MoveRight(const FInputActionValue& InputActionValue)
{
	if(bIsReloading) return;
	float InputValue = InputActionValue.Get<float>();
	AddMovementInput(GetActorRightVector() * InputValue);
}

void ASoldierCharacter::TurnView(const FInputActionValue& InputActionValue)
{
	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	float TuneSpeed = 1;
	if (bInZoom && GunIsSafe())
		TuneSpeed = 5/(90-GetActiveGun()->FieldOfView);
	AddControllerYawInput(InputValue.X * TuneSpeed);
	AddControllerPitchInput(-1* InputValue.Y * TuneSpeed);
}

void ASoldierCharacter::Fire()
{
	if(bIsFiring || bIsReloading) return;
	bIsFiring = true;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
	{
		bIsFiring = false;
	});
	float DelayTime = GunIsSafe() ? GetActiveGun()->TriggerIntervals : 0.01;
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, DelayTime, false);
	if (GetActiveGun())
		GetActiveGun()->Shoot();
}

float ASoldierCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	if (Health <= 0)
		HandleDeath();
	return ActualDamage;
}

void ASoldierCharacter::HandleDeath()
{
	AShooterGameMode* GameMode = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->HandleKills(this);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	if (Team == ESoldierTeam::Peace && bIsLeader)
	{
		GetWorld()->GetFirstPlayerController()->StartSpectatingOnly();
	}
	//SetLifeSpan(7.0);
	//UnPossessed();
}

void ASoldierCharacter::ManageGuns()
{
	for (int i = 0; i <  Guns.Num(); i++)
	{
		if (i == ActiveGunIndex)
			Guns[i]->SetActorHiddenInGame(false);
		else
			Guns[i]->SetActorHiddenInGame(true);
	}
}

void ASoldierCharacter::ChangeGun(const FInputActionValue& InputValue)
{
	float ScrolVal = InputValue.Get<float>();
	if(ScrolVal != 0 && bCanChangeGun)
	{
		ActiveGunIndex += ScrolVal;
		if(ActiveGunIndex >= Guns.Num())
			ActiveGunIndex = 0;
		if(ActiveGunIndex <= -1)
			ActiveGunIndex = Guns.Num() -1;
		ManageGuns();
	}
}

void ASoldierCharacter::ZoomInSniperCamera()
{
	bInZoom = true;
	bCanChangeGun = false;
	SpringArm->TargetArmLength = -1*GetActiveGun()->ZoomValue;
	CameraComponent->FieldOfView = GetActiveGun()->FieldOfView;
	WBP_SniperView = CreateWidget<UUserWidget>(GetWorld(), SniperViewClass);
	if (WBP_SniperView)
		WBP_SniperView->AddToViewport();
}

void ASoldierCharacter::ZoomOutSniperCamera()
{
	bInZoom = false;
	bCanChangeGun = true;
	SpringArm->TargetArmLength = 200;
	CameraComponent->FieldOfView = 90;
	RemoveSniperViewWidget();
}

void ASoldierCharacter::Zoom()
{
	if (ActiveGunIndex != 2) return;
	if (!bInZoom)
	{
		ZoomInSniperCamera();
	}
	else
	{
		ZoomOutSniperCamera();
	}
}

bool ASoldierCharacter::GunIsSafe()
{
	if(!Guns.IsEmpty() && ActiveGunIndex < Guns.Num())
		if(IsValid(Guns[ActiveGunIndex]))
			return true;
	return false;
}

FGenericTeamId ASoldierCharacter::GetGenericTeamId() const
{
	return TeamId;
}
