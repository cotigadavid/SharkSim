// Fill out your copyright notice in the Description page of Project Settings.


#include "CrabCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ACrabCharacter::ACrabCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(RootComponent);

	Alive = 1;
}

// Called when the game starts or when spawned
void ACrabCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrabCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

