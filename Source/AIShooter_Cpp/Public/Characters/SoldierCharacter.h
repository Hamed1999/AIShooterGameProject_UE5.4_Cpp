// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

UCLASS()
class AISHOOTER_CPP_API ASoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASoldierCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
/**
 * Methods
 */
	void CreateSpringArm();
	void CreateCamera();
	void CreateMappingContext();
	void BindEnhancedInputActions(UInputComponent* PlayerInputComponent);
	void MoveForward(const struct FInputActionValue& InputActionValue);
	void MoveRight(const struct FInputActionValue& InputActionValue);
	void TurnView(const struct FInputActionValue& InputActionValue);
	
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
};
