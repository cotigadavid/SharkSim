// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish2AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "SharkCharacter.h"
#include "BrainComponent.h"
#include "SharkSimGameModeBase.h"

AFish2AIController::AFish2AIController()
{
	PrimaryActorTick.bCanEverTick = true; 

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config")); 

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AFish2AIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

}

void AFish2AIController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle anotherHandle3;
	GetWorldTimerManager().SetTimer(anotherHandle3, this, &AFish2AIController::Restart, 30.0f, true);

}

void AFish2AIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}

void AFish2AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());


	FVector Location = GetPawn()->GetActorLocation();

	if (Location.Z > 22391) {
		GetPawn()->Destroy();


		int n = FMath::RandRange(0, 100);

		if (n < 33)
			GameMode->SpawnFishAtRandomLocation(0);
		else if (n < 66)
			GameMode->SpawnFishAtRandomLocation(1);
		else if (n < 88)
			GameMode->SpawnFishAtRandomLocation(2);
		else
			GameMode->SpawnFishAtRandomLocation(3);
	}
}

FRotator AFish2AIController::GetControlRotation() const
{
	if (GetPawn() == NULL) 
	{
		return FRotator(0, 0, 0);
	}
	

	return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
}

void AFish2AIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	ASharkCharacter* CastingShark = Cast<ASharkCharacter>(DetectedPawns[0]);

	if (CastingShark) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pawn Detected"));
 
		bCanSeePlayer = true;
	}
}

bool AFish2AIController::getCanSeePlayer()
{
	return bCanSeePlayer;
}

void AFish2AIController::Restart()
{ 
	GetBrainComponent()->RestartLogic();
}
