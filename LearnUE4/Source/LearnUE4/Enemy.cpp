// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#define DEMO_ENEMY_SIGHT_LENGTH 100
DEFINE_LOG_CATEGORY(MyLog);

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
	
	const FVector startPos = GetActorLocation();
	const FRotator rotation = GetActorRotation();

	FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
	direction = direction.RotateAngleAxis(-45.f, FRotationMatrix(rotation).GetUnitAxis(EAxis::Z));
	float angleOffset = -45.f;
	const float angleStep = 1.f;

	TArray<AActor*> ignoreList;
	FHitResult result;
	while (angleOffset < 45.f)
	{
		if (UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			startPos,
			startPos + DEMO_ENEMY_SIGHT_LENGTH * direction,
			ETraceTypeQuery::TraceTypeQuery3,
			true,
			ignoreList,
			EDrawDebugTrace::None,
			result,
			false
		))
		{
			if (result.Actor->ActorHasTag("Player"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Warning! Player is Exposed!"));
				m_AIController->StopMovement();
				GetWorldTimerManager().ClearTimer(m_IdleTimer);
			}
		}

#if !UE_BUILD_SHIPPING 
		DrawDebugLine(
			GetWorld(),
			startPos,
			startPos + DEMO_ENEMY_SIGHT_LENGTH * direction,
			FColor(0, 0, 255),
			false, // sets weather or not the line is in the world permanently
			1.f, 0,
			1
		);

		UE_LOG(MyLog, Warning, TEXT("%s"), *direction.ToString());
#endif

		angleOffset += angleStep;
		
		direction = direction.RotateAngleAxis(angleStep, FRotationMatrix(rotation).GetUnitAxis(EAxis::Z));
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnReachDestination(FAIRequestID requestID, EPathFollowingResult::Type result)
{

#if !UE_BUILD_SHIPPING 
	UE_LOG(MyLog, Warning, TEXT("Reach Destination: index %d"), m_ListIndex);
#endif

	//TODO: idle for a while and go to next position

	GetWorldTimerManager().SetTimer(
		m_IdleTimer, 
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

