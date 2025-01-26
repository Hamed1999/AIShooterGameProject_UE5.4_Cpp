// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

UENUM(BlueprintType)
enum class ESoldierTeam : uint8
{
	Peace UMETA(DisplayName = "Peace Team"),
	Devil UMETA(DisplayName = "Devil Team"),
	Neutral UMETA(DisplayName = "NPC")
};

UCLASS()
class AISHOOTER_CPP_API ASoldierCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASoldierCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		bool bIsFiring = false;
	UFUNCTION(BlueprintPure)
		bool IsDead();
	virtual FGenericTeamId GetGenericTeamId() const override;
	void Fire();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		bool bIsLeader = false;
	UFUNCTION(BlueprintPure)
		float GetPlayerHealth();
	void RemoveSniperViewWidget();
	UFUNCTION(BlueprintCallable)
		void Reload();
	UFUNCTION(BlueprintPure)
		int GetMagAmmo();
	UFUNCTION(BlueprintPure)
	    int GetAmmo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	    bool bIsReloading = false;
	void SetAmmo(int Ammo);
	void SetHealth(float NewHealth);
	class AGun* GetActiveGun();
private:
/**
 * Methods
 */
	void CreateSpringArm();
	void CreateCamera();
	void SetSniperViewClass();
	void CreateMappingContext();
	void SpawnGun();
	void SetTeamId();
	void SetAIControllerClass();
	void BindEnhancedInputActions(UInputComponent* PlayerInputComponent);
	void MoveForward(const struct FInputActionValue& InputActionValue);
	void MoveRight(const struct FInputActionValue& InputActionValue);
	void TurnView(const struct FInputActionValue& InputActionValue);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	void HandleDeath();
	void ManageGuns();
    void ChangeGun(const  FInputActionValue& InputValue);
	void ZoomInSniperCamera();
	void ZoomOutSniperCamera();
	void Zoom();
	bool GunIsSafe();
/**
* Global Properties
*/
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		class UCameraComponent* CameraComponent;
	// Default Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
		class UInputMappingContext* IMC_DefaultInputs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
	class UInputAction* IA_MoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
		UInputAction* IA_MoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
		UInputAction* IA_TurnCamerView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
		UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Inputs", meta=(AllowPrivateAccess=true))
		UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInputAction* IA_ChangeGun;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInputAction* IA_Zoom;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInputAction* IA_Reload;
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun", meta=(AllowPrivateAccess=true))
		TArray<TSubclassOf<AGun>> GunClasses;
	TArray<AGun*> Guns;
	UPROPERTY(EditAnywhere, Category= "Gun", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		int ActiveGunIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta=(AllowPrivateAccess=true, ClampMin = "0"))
		float MaxHealth = 100;
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta=(AllowPrivateAccess=true))
		ESoldierTeam Team = ESoldierTeam::Peace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta=(AllowPrivateAccess=true))
		FGenericTeamId TeamId = FGenericTeamId(0);
	bool bInZoom = false;
	bool bCanChangeGun = true;
	TSubclassOf<UUserWidget> SniperViewClass = nullptr;
	UUserWidget* WBP_SniperView = nullptr;
};
