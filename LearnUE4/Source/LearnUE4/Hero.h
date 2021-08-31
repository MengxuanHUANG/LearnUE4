// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LEARNUE4_API AHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* m_FollowedCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* m_CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float m_RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float m_WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float m_CrouchedSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool b_IsRunning = false;

public:

	UFUNCTION()
		void MoveForward(float axis);

	UFUNCTION()
		void MoveRight(float axis);

	UFUNCTION()
		void SwitchWalkAndRun();


};
