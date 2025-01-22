// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "ShooterPlayerController.h"
#include "Widgets/PlayerHUD.h"
#include "Widgets/PauseMenu.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

void AShooterPlayerController::SetHUDClass()
{
	ConstructorHelpers::FClassFinder<UPlayerHUD> HUD_Ref(TEXT("/Game/Widgets/WBP_HUD"));
	if(HUD_Ref.Succeeded())
		PlayerHUDClass = HUD_Ref.Class.Get();
}

void AShooterPlayerController::SetInputMappingContext()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_PauseREf (TEXT("/Game/Inputs/IMC_PauseMenu"));
	if (IMC_PauseREf.Succeeded())
		IMC_PauseMenu = IMC_PauseREf.Object.Get();
}

void AShooterPlayerController::SetIA_Pause()
{
	ConstructorHelpers::FObjectFinder<UInputAction> IA_PauseREf (TEXT("/Game/Inputs/IA_Pause"));
	if (IA_PauseREf.Succeeded())
		IA_Pause = IA_PauseREf.Object.Get();
}

void AShooterPlayerController::SetPauseMenuClass()
{
	ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenu_Ref(TEXT("/Game/Widgets/WBP_PauseMenu"));
	if(PauseMenu_Ref.Succeeded())
		PauseMenuClass = PauseMenu_Ref.Class.Get();
}

void AShooterPlayerController::ResumeGame()
{
	EnableInput(this);
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AShooterPlayerController::RestartGame()
{
	ResumeGame();
	//FTimerHandle TimerHandle;
	//FTimerDelegate TimerDel;
	/*TimerDel.BindLambda([&]()
	{
		
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 8, false);*/
	UGameplayStatics::OpenLevel(GetWorld(), FName("TestLevel"));
}

void AShooterPlayerController::SetEndGameClass()
{
	ConstructorHelpers::FClassFinder<UUserWidget> EndGame_Ref(TEXT("/Game/Widgets/WBP_EndGame"));
	if(EndGame_Ref.Succeeded())
		EndGameClass = EndGame_Ref.Class.Get();
}

AShooterPlayerController::AShooterPlayerController()
{
	SetHUDClass();
	SetInputMappingContext();
	SetIA_Pause();
	SetPauseMenuClass();
	SetEndGameClass();
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	bIsWon = bIsWinner;
	if (UUserWidget* WBP_EndGame = CreateWidget<UUserWidget>(this, EndGameClass))
	{
		WBP_EndGame->AddToViewport();
		SetGamePaused();
	}
}

const FString AShooterPlayerController::GetMessage()
{
	FString WinMessage = FString("     WoW!\n You Win : ) ...");
	FString LoseMessage = FString("    Oops : )\n Such A Looser ...");
	FString Message = bIsWon ? WinMessage : LoseMessage;
	return Message;
}

void AShooterPlayerController::CreateHUD()
{
	if(UPlayerHUD* WBP_HUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass))
	{
		WBP_HUD->SetOwnerCharacter(GetCharacter());
		WBP_HUD->AddToViewport();
	}
}

void AShooterPlayerController::CreateMappingContex()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_PauseMenu, 0);
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CreateHUD();
	CreateMappingContex();
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Triggered, this, &AShooterPlayerController::PauseGame);
	}
}

void AShooterPlayerController::SetGamePaused()
{
	DisableInput(this);
	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AShooterPlayerController::PauseGame()
{
	if(UPauseMenu* WBP_PauseMenu = CreateWidget<UPauseMenu>(this ,PauseMenuClass))
	{
		WBP_PauseMenu->AddToViewport();
		WBP_PauseMenu->SetupController(this);
		SetGamePaused();
	}
	
}
