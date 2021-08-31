// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_CameraBoom->SetupAttachment(RootComponent);

	m_CameraBoom->TargetArmLength = 300.f;
	m_CameraBoom->bUsePawnControlRotation = true;


	m_FollowedCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Followed Camera"));
	m_FollowedCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	m_FollowedCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

}

void AHero::MoveForward(float axis)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector dir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(dir, axis);
	}
}

void AHero::MoveRight(float axis)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector dir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(dir, axis);
	}
}

