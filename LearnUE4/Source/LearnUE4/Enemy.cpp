// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Actor.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Born();
}

void AEnemy::Born()
{
	//Controller->ClientSetLocation(m_StartPos, FRotator(0.f));

	FVector tempVec = (m_EndPos - m_StartPos) / m_SectionNumber;
	//TODO: generate position list
	for (int i = 0; i < m_SectionNumber + 1; i++)
	{
		m_PosList.Add(m_StartPos + i * tempVec);
	}
	m_ListIndex = 1;

	//TODO: add dynamic for AI Controller
	m_AIController = Cast<AAIController>(GetController());
	m_AIController->ReceiveMoveCompleted.AddDynamic(this, &AEnemy::OnReachDestination);

	MoveToNext();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnReachDestination(FAIRequestID requestID, EPathFollowingResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("Reach Destination: index %d"), m_ListIndex);
	//TODO: idle for a while and go to next position
	FTimerHandle unusedHandler;
	GetWorldTimerManager().SetTimer(
		unusedHandler, 
		this, 
		&AEnemy::MoveToNext,
		FMath::RandRange(m_MinIdleTime, m_MaxIdleTime),
		false
	);
}

void AEnemy::MoveToNext()
{
	m_AIController->MoveToLocation(m_PosList[m_ListIndex++]);
	m_ListIndex = m_ListIndex % m_PosList.Num();
}

