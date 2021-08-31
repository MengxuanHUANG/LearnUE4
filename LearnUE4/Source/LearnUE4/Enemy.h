// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"

#include "Enemy.generated.h"

UCLASS()
class LEARNUE4_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Born();

	UFUNCTION(BlueprintCallable)
		void OnReachDestination(FAIRequestID requestID, EPathFollowingResult::Type result);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector m_StartPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector m_EndPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 m_SectionNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float m_MaxIdleTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float m_MinIdleTime;

private:
	//hold positions between start position & end position
	TArray<FVector> m_PosList;

	//hold index of current position (start from 0)
	int32 m_ListIndex;

	void MoveToNext();

	AAIController* m_AIController;
};
