// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void ResumeGame();
	UFUNCTION(BlueprintCallable)
		void RestartGame();
	void SetEndGameClass();
	AShooterPlayerController();
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	UFUNCTION(BlueprintPure, Category="EndGame")
	  const FString GetMessage();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetGamePaused();

private:
	/**
	* Methods
	*/
	void SetHUDClass();
	void SetInputMappingContext();
	void SetIA_Pause();
	void SetPauseMenuClass();
	void PauseGame();
	void CreateHUD();
	void CreateMappingContex();
	/**
	 * Global Var
	 */
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;
	TSubclassOf<class UPauseMenu> PauseMenuClass;
	TSubclassOf<UUserWidget> EndGameClass;
	bool bIsWon = false;
	// Default Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu Inputs", meta=(AllowPrivateAccess=true))
		class UInputMappingContext* IMC_PauseMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu Inputs", meta=(AllowPrivateAccess=true))
		class UInputAction* IA_Pause;
};
