// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SharkSimGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHARKSIM_API ASharkSimGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	ASharkSimGameModeBase();

	void SpawnFishAtRandomLocation(int);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> FishToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> FishToSpawn2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> CrabToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> TurtleToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* BloodToSpawn;
	//UParticleSystem* BloodToSpawn;

	void SpawnBlood(FVector Location);

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;

	UFUNCTION(BlueprintPure)
		int StartGame();

	void StarGameForReal();

	void increaseScore(int value);

	void StopGame();

	void checkGameOver();

	void Pause();

	bool gameIsRunning;

	int nrRestart;

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"));
	TSubclassOf<class UUserWidget> InGameHUD;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"));
	TSubclassOf<class UUserWidget> StartingGameHUD;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"));
	TSubclassOf<class UUserWidget> GameOverHUD;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"));
	TSubclassOf<class UUserWidget> LoadingScreenHUD;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

	int min = 0, sec = 0, score = 0;
	void incrementTime();

	UFUNCTION(BlueprintPure, Category = "Time")
		int GetSec();

	UFUNCTION(BlueprintPure, Category = "Time")
		int GetMin();

	UFUNCTION(BlueprintPure, Category = "Score")
		int GetScore();

private:
	
};
