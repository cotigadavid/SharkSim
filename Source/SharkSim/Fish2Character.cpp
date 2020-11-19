// Fill out your copyright notice in the Description page of Project Settings.


// BASE CLASS FOR FISH 2 AND 1(2_1)

#include "Fish2Character.h"
#include "Fish2AIController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "SharkCharacter.h"
#include "Engine/StaticMesh.h"

// Sets default values
AFish2Character::AFish2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFish2Character::BeginOverlap);
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFish2Character::BeginOverlap);

	FRotator Rot = { 0, 45, 0 };
	SetActorRotation(Rot);

	Alive = 1;
}

// Called when the game starts or when spawned
void AFish2Character::BeginPlay()
{
	Super::BeginPlay();
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Some debug message!"));
}

// Called every frame
void AFish2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FRotator Rot = { 0, 45, 0 };
	//SetActorRotation(Rot);
}

// Called to bind functionality to input

void AFish2Character::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*ASharkCharacter* OverlappingShark = Cast<ASharkCharacter>(OtherActor);

	if (OtherComp == OverlappingShark->SphereComponent) {
		//OverlappingShark->Attack();
		//Destroy();
	}*/

	/*UStaticMesh* island = Cast<UStaticMesh>(OtherActor);

	if (island) {
		AFish2AIController* Controller = Cast<AFish2AIController>(GetController());

		Controller->GetBrainComponent()->RestartLogic();

	}*/

}

bool AFish2Character::setCanSeePlayer(bool a)
{
	return false;
}

