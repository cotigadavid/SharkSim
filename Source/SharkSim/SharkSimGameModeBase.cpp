// Copyright Epic Games, Inc. All Rights Reserved.


#include "SharkSimGameModeBase.h"
#include "SharkCharacter.h"
#include "Fish2Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"
#include "Particles/ParticleSystem.h"
#include "CrabCharacter.h"
#include "TurtleCharacter.h"
#include "Fish2_1Character.h"

//#include "Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
// Engine / Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h

ASharkSimGameModeBase::ASharkSimGameModeBase()
{
	gameIsRunning = 0;
	nrRestart = 0;
}


void ASharkSimGameModeBase::SpawnFishAtRandomLocation(int n)
{
	int randSpace = FMath::RandRange(1, 4);
	int x, y, z;

	if (randSpace == 1) {
		x = FMath::RandRange(-50000, 50000);
		y = FMath::RandRange(-48000, -28000);
		z = FMath::RandRange(14000, 22000);
	}
	if (randSpace == 2) {
		x = FMath::RandRange(-50000, 50000);
		y = FMath::RandRange(25000, 45000);
		z = FMath::RandRange(14000, 22000);
	}
	if (randSpace == 3) {
		x = FMath::RandRange(-48000, -28000);
		y = FMath::RandRange(-50000, 50000);
		z = FMath::RandRange(14000, 22000);
	}
	if (randSpace == 4) {
		x = FMath::RandRange(28000, 48000);
		y = FMath::RandRange(-50000, 50000);
		z = FMath::RandRange(14000, 22000);
	}

	if (n == 2)
		z = FMath::RandRange(14000, 15000);

	FVector location = FVector(x, y, z);
	FRotator rotate = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;

	if (n == 1)
		GetWorld()->SpawnActor<ACharacter>(FishToSpawn, location, rotate, SpawnInfo);
	if (n == 0)
		GetWorld()->SpawnActor<ACharacter>(FishToSpawn2, location, rotate, SpawnInfo);
	if (n == 2)
		GetWorld()->SpawnActor<ACharacter>(CrabToSpawn, location, rotate, SpawnInfo);
	if (n == 3)
		GetWorld()->SpawnActor<ACharacter>(TurtleToSpawn, location, rotate, SpawnInfo);
}

void ASharkSimGameModeBase::BeginPlay()
{
	Super::BeginPlay();


	if (StartingGameHUD != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingGameHUD);

		if (StartingGameHUD != NULL)
			CurrentWidget->AddToViewport();
	}

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//FTimerHandle otherHandle;
	//GetWorldTimerManager().SetTimer(otherHandle, this, &ASharkSimGameModeBase::StartGame, 3.1f, false);

	FTimerHandle otherHandle2;
	GetWorldTimerManager().SetTimer(otherHandle2, this, &ASharkSimGameModeBase::checkGameOver, 0.1f, true);
}


void ASharkSimGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASharkSimGameModeBase::SpawnBlood(FVector Location)
{
	//ParticleSystem = Cast<UParticleSystem>((BloodToSpawn));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodToSpawn, Location, FRotator::ZeroRotator, true);

	//UNiagaraFunctionLibrabry::Spawn
}

int ASharkSimGameModeBase::StartGame()
{
	gameIsRunning = 1;
	nrRestart++;
	score = 0;
	sec = 0;
	min = 0;

	if (LevelToLoad != "")
	{
		//FLatentActionInfo LatentInfo;
		//UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		//UGameplayStatics::OpenLevel(GetWorld(), FName("D:/UnrealProjects/SharkSim/Content/Level1/Level1.umap"), true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Loaded"));
	}

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(39000, 600, 11000));

	CurrentWidget->RemoveFromParent();
	if (LoadingScreenHUD != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenHUD);

		if (LoadingScreenHUD != NULL)
			CurrentWidget->AddToViewport();
	}

	FTimerHandle otherHandle;
	GetWorldTimerManager().SetTimer(otherHandle, this, &ASharkSimGameModeBase::StarGameForReal, 1.0f, false);

	return 1;
}

void ASharkSimGameModeBase::StarGameForReal()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	CurrentWidget->RemoveFromParent();
	if (InGameHUD != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), InGameHUD);

		if (InGameHUD != NULL)
			CurrentWidget->AddToViewport();
	}

	//if (nrRestart == 1)
	for (int i = 0; i < 60; ++i)
		SpawnFishAtRandomLocation(0);
	for (int i = 0; i < 60; ++i)
		SpawnFishAtRandomLocation(1);
	for (int i = 0; i < 40; ++i)
		SpawnFishAtRandomLocation(2);
	for (int i = 0; i < 20; ++i)
		SpawnFishAtRandomLocation(3);

	//ASharkCharacter* Shark = Cast<ASharkCharacter>(GetWorld()->GetFirstPlayerController());
	ASharkCharacter* Player1 = Cast<ASharkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player1) {
		Player1->StartedGame = true;
		Player1->startAudio();
		Player1->healthMult = 1;
		//Player1->UpdateCurrentHealth(100 - Player1->GetCurrentHealth());
		//Player1->UpdateCurrentStamina(100 - Player1->GetCurrentStamina());
	}
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ASharkSimGameModeBase::incrementTime, 1.0f, true);
}

void ASharkSimGameModeBase::increaseScore(int value)
{
	score += value;
}

void ASharkSimGameModeBase::StopGame()
{
	for (TObjectIterator<AFish2Character> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(AFish2Character::StaticClass()))
		{
			Itr->Destroy();
		}
	}

	for (TObjectIterator<AFish2_1Character> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(AFish2_1Character::StaticClass()))
		{
			Itr->Destroy();
		}
	}

	for (TObjectIterator<ACrabCharacter> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(ACrabCharacter::StaticClass()))
		{
			Itr->Destroy();
		}
	}

	for (TObjectIterator<ATurtleCharacter> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(ATurtleCharacter::StaticClass()))
		{
			Itr->Destroy();
		}
	}

	ASharkCharacter* Player1 = Cast<ASharkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player1) {
		Player1->UpdateCurrentHealth(100 - Player1->GetCurrentHealth());
		Player1->UpdateCurrentStamina(100 - Player1->GetCurrentStamina());
	}

	CurrentWidget->RemoveFromParent();
	if (GameOverHUD != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverHUD);

		if (GameOverHUD != NULL)
			CurrentWidget->AddToViewport();
	}

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	gameIsRunning = 0;

	if (LevelToLoad != "") {
		//FLatentActionInfo LatentInfo;
		//UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, 0);
		//UnloadStreamLevel(const UObject * WorldContextObject, FName LevelName, FLatentActionInfo LatentInfo, bool bShouldBlockOnUnload);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GATA"));
}

void ASharkSimGameModeBase::checkGameOver()
{
	ASharkCharacter* Player = Cast<ASharkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GATA"));
		if (Player->GetCurrentHealth() <= 0) {
			FTimerHandle anotherHandle3;
			GetWorldTimerManager().SetTimer(anotherHandle3, this, &ASharkSimGameModeBase::StopGame, 1.0f, false);
		}
	}
}

void ASharkSimGameModeBase::Pause()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pauza"));

	//if (!IsPaused())
		UGameplayStatics::SetGamePaused(this, true);
	//else
		//UGameplayStatics::SetGamePaused(this, false);
}

void ASharkSimGameModeBase::incrementTime()
{
	if (gameIsRunning) {
		sec++;

		if (sec > 59) {
			sec = 0;
			min++;
		}
	}
}

int ASharkSimGameModeBase::GetSec()
{
	return sec;
}

int ASharkSimGameModeBase::GetMin()
{
	return min;
}

int ASharkSimGameModeBase::GetScore()
{
	return score;
}
