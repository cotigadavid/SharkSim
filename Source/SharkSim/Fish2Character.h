// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fish2Character.generated.h"

UCLASS()
class SHARKSIM_API AFish2Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFish2Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		class USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool Alive;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//AI SECTOR

	bool CanSeePlayer;
	UFUNCTION(BlueprintPure, Category = "AI")
		bool setCanSeePlayer(bool a);
};
