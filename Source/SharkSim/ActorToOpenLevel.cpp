// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorToOpenLevel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActorToOpenLevel::AActorToOpenLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorToOpenLevel::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("D:/UnrealProjects/SharkSim/Content/Level1/Level1.umap"), true);
	UGameplayStatics::OpenLevel(this, "Level1");
}

// Called every frame
void AActorToOpenLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

