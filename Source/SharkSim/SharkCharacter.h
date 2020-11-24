// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SharkCharacter.generated.h"

UCLASS()
class SHARKSIM_API ASharkCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))  //meta
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) // meta
		class UCameraComponent* FollowCamera;

public:

	// Sets default values for this character's properties
	ASharkCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		class USphereComponent* SphereComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void MoveUp(float Value);

	void Pause();

	void Attack();
	void Attack2();
	bool isAttacking = false;

	void KillFish(AActor* fish);

	bool StartedGame = false;

	bool isEatingAccepted;

	void PlaySound();

	UFUNCTION(BlueprintPure, Category = "boolean")
		bool GetIsAttacking();

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Boost();
	float mult = 10;
	float healthMult = 1;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetInitialStamina();

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetCurrentStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void UpdateCurrentStamina(float Change);


	UFUNCTION(BlueprintPure, Category = "Health")
		float GetInitialHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateCurrentHealth(float Change);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* EatSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* BackgroundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* BoostAudioCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* SplashCue;

	void PlayBackgroundSound();

	void startAudio();

private:

	class UAudioComponent* SharkEatAudioComponent;
	class UAudioComponent* BackgroundAudioComponent;
	class UAudioComponent* BoostAudioComponent;
	class UAudioComponent* SplashAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float InitialStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Health")
		float InitialHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
		float CurrentHealth;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
