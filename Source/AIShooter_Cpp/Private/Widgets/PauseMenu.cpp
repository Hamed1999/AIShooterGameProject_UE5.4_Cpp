// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "ShooterPlayerController.h"

bool UPauseMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeGame);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenu::RestartGame);
	ExitButton->OnClicked.AddDynamic(this, &UPauseMenu::ExitGame);
	return true;
}



void UPauseMenu::ResumeGame()
{
	RemoveFromParent();
	if(PlayerController)
		PlayerController->ResumeGame();
}

void UPauseMenu::RestartGame()
{
	RemoveFromParent();
	if(PlayerController)
		PlayerController->RestartGame();
}

void UPauseMenu::ExitGame()
{
	PlayerController->ConsoleCommand("Quit");
}

void UPauseMenu::SetupController(AShooterPlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;
}
