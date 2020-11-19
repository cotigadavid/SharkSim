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

	if (bCanSeePlayer) {
		//MoveToLocation(FVector(-45000, -4600, 5000));
	}

	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	/*if (!GameMode->gameIsRunning && GetBrainComponent()->IsActive()) {
		GetBrainComponent()->StopLogic("AFs");
	}
	if (GameMode->gameIsRunning && !GetBrainComponent()->IsActive()) {
		GetBrainComponent()->RestartLogic();
	}*/
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

		//if (GetBrainComponent())
			//GetBrainComponent()->StopLogic(FString("No reason"));
			//GetBrainComponent()->Cleanup();

		/*ASharkCharacter* Player = Cast<ASharkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		GetCharacter()->SetActorRotation(Player->GetActorRotation());
		Direction = FRotationMatrix(Player->GetActorRotation()).GetUnitAxis(EAxis::X);

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [this]() {
			GetCharacter()->AddMovementInput(Direction, 1.0f);
			}, 0.01f, false);*/

		//GetCharacter()->AddMovementInput(Direction, 1.0f);

		//GetCharacter()->SetActorLocation(FVector(-45000, -46000, 5000));
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%f %f %f \n"), GetControlRotation().Roll, GetControlRotation().Pitch, GetControlRotation().Yaw));
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
