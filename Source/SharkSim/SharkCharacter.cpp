// Fill out your copyright notice in the Description page of Project Settings.
#include "SharkCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Fish2Character.h"
#include "CrabCharacter.h"
#include "SharkSimGameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "TurtleCharacter.h"



// Sets default values
ASharkCharacter::ASharkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASharkCharacter::BeginOverlap);
	
	SharkEatAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EatAudioComponent"));
	SharkEatAudioComponent->SetupAttachment(RootComponent);

	BackgroundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundAudioComponent"));
	BackgroundAudioComponent->SetupAttachment(RootComponent);

	BoostAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BoostAudioComponent"));
	BoostAudioComponent->SetupAttachment(RootComponent);

	SplashAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SplashAudioComponent"));
	SplashAudioComponent->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<USoundCue> EatSoundCueObject(TEXT("SoundCue'/Game/Sounds/SharkEatingCue.SharkEatingCue'"));

	//if (EatSoundCueObject.Succeeded())
		//EatSoundCue = EatSoundCueObject.Object;

}

// Called when the game starts or when spawned
void ASharkCharacter::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::OpenLevel(this, FName("D:/UnrealProjects/SharkSim/Content/Level1/Level1.umap"), true);
	//UGameplayStatics::OpenLevel(this, "Test");

	if (EatSoundCue) {
		SharkEatAudioComponent->SetSound(EatSoundCue);
	}
	if (BackgroundCue) {
		BackgroundAudioComponent->SetSound(BackgroundCue);
	}
	if (BoostAudioCue) {
		BoostAudioComponent->SetSound(BoostAudioCue);
	}
	if (SplashAudioComponent) {
		SplashAudioComponent->SetSound(SplashCue);
	}

	isEatingAccepted = false;
	
}

// Called every frame
void ASharkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// To add increasing stamina and decreasing hp
	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->gameIsRunning) {
		MoveForward(0.25f);

		if (CurrentStamina < 100)
			UpdateCurrentStamina(0.05f);

		if (StartedGame)
			UpdateCurrentHealth(-0.025f * healthMult);

		if (GetCurrentHealth() > 100)
			UpdateCurrentHealth(100 - GetCurrentHealth());

		if (GetActorLocation().Z > 22391 && GetCharacterMovement()->MovementMode == GetCharacterMovement()->DefaultLandMovementMode) {
			GetCharacterMovement()->SetMovementMode(GetCharacterMovement()->DefaultWaterMovementMode);
			healthMult = 4;
			BackgroundAudioComponent->Stop();
			mult = 20;
		}

		if (GetActorLocation().Z < 22300 && GetCharacterMovement()->MovementMode == GetCharacterMovement()->DefaultWaterMovementMode) {
			GetCharacterMovement()->SetMovementMode(GetCharacterMovement()->DefaultLandMovementMode);
			healthMult = 1;
			mult = 10;

			SplashAudioComponent->Play(0.0f);

			FTimerHandle handle1;
			GetWorldTimerManager().SetTimer(handle1, [&]() {
				BackgroundAudioComponent->Play(0.0f);
				}, 0.76f, false);
		}
	}

	if (!GameMode->gameIsRunning )
		BackgroundAudioComponent->Stop();
	
}

// Called to bind functionality to input
void ASharkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &ASharkCharacter::Boost);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASharkCharacter::Attack2);
	PlayerInputComponent->BindAction("Pause", IE_Released, this, &ASharkCharacter::Pause);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASharkCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASharkCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASharkCharacter::MoveUp);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASharkCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASharkCharacter::LookUpAtRate);
}

void ASharkCharacter::MoveForward(float Value)
{
	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if ((Controller != NULL) && (Value != 0.0f) && (GameMode->gameIsRunning))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value / mult);
	}
}

void ASharkCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);                                                                                 

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement in that direction
		AddMovementInput(Direction, Value / 1.0f);
	}
}

void ASharkCharacter::MoveUp(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator PitchRotation(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);

		// get right vector 
		const FVector Direction = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Z);
		// add movement in that direction
		AddMovementInput(Direction, Value / 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%f %f %f \n"), Direction.X, Direction.Y, Direction.Z));
	}
}

void ASharkCharacter::Pause()
{
	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode && GameMode->gameIsRunning)
		GameMode->Pause();
}

void ASharkCharacter::Attack()
{

	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if (!isAttacking && GameMode->gameIsRunning) {
		isAttacking = true;

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ASharkCharacter::PlaySound, 0.3f, false);
		//PlaySound();


		FTimerHandle handle1;
		GetWorldTimerManager().SetTimer(handle1, [this]() {
			isAttacking = false;
			}, 0.7f, false);
	}
}

void ASharkCharacter::Attack2()
{
	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if (!isAttacking && GameMode->gameIsRunning) {
		isAttacking = true;
		FTimerHandle handle1;
		GetWorldTimerManager().SetTimer(handle1, [this]() {
			isAttacking = false;
			}, 0.7f, false);
	}
}

void ASharkCharacter::PlaySound()
{
	if (EatSoundCue && SharkEatAudioComponent)
		SharkEatAudioComponent->Play(0.f);
}

bool ASharkCharacter::GetIsAttacking()
{
	return isAttacking;
}

void ASharkCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASharkCharacter::Boost()
{
	if (CurrentStamina >= 100) {
		UpdateCurrentStamina(-100);
		mult = 1;

		if (BoostAudioCue && BoostAudioComponent)
			BoostAudioComponent->Play(0.5f);

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [this]() {
			mult = 10;
			}, 1.0f, false);
	}

}

void ASharkCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

float ASharkCharacter::GetInitialStamina()
{

	return InitialStamina;
}

float ASharkCharacter::GetCurrentStamina()
{

	return CurrentStamina;
}

void ASharkCharacter::UpdateCurrentStamina(float Change)
{
	CurrentStamina = CurrentStamina + Change;
}

float ASharkCharacter::GetInitialHealth()
{

	return InitialHealth;
}

float ASharkCharacter::GetCurrentHealth()
{

	return CurrentHealth;
}

void ASharkCharacter::UpdateCurrentHealth(float Change)
{
	CurrentHealth = CurrentHealth + Change;
}

void ASharkCharacter::PlayBackgroundSound()
{
	if (BackgroundCue && BackgroundAudioComponent)
		BackgroundAudioComponent->Play(0.f);
}

void ASharkCharacter::startAudio()
{
	FTimerHandle handle7;
	GetWorldTimerManager().SetTimer(handle7, this, &ASharkCharacter::PlayBackgroundSound, 28.25f, true);
}

void ASharkCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFish2Character* OverlappedFish = Cast<AFish2Character>(OtherActor);
	ACrabCharacter* OverlappedCrab = Cast<ACrabCharacter>(OtherActor);
	ATurtleCharacter* OverlappedTurtle = Cast<ATurtleCharacter>(OtherActor);

	ASharkSimGameModeBase* GameMode = Cast<ASharkSimGameModeBase>(GetWorld()->GetAuthGameMode());

	if (OverlappedFish && GameMode->gameIsRunning && OverlappedFish->Alive) {
		Attack();
		OverlappedFish->Alive = false;

		//OverlappedFish->Destroy();
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [OverlappedFish]() {                 //VARIABILA PE AICI
			OverlappedFish->Destroy(); 
			}, 0.1f, false);
		
		GameMode->SpawnBlood(OverlappedFish->GetActorLocation());
		if (GameMode) {
			GameMode->increaseScore(100);
		}

		UpdateCurrentHealth(10);
	}

	if (OverlappedCrab && GameMode->gameIsRunning && OverlappedCrab->Alive) {
		Attack();
		OverlappedCrab->Alive = false;

		OverlappedCrab->Destroy();
		FTimerHandle handle1;
		GetWorldTimerManager().SetTimer(handle1, [OverlappedCrab]() {
			OverlappedCrab->Destroy();
			}, 0.1f, false);
		
		GameMode->SpawnBlood(OverlappedCrab->GetActorLocation());
		if (GameMode) {
			GameMode->increaseScore(50);
		}

		UpdateCurrentHealth(5);
	}

	if (OverlappedTurtle && GameMode->gameIsRunning && OverlappedTurtle->Alive) {
		Attack();
		OverlappedTurtle->Alive = false;

		OverlappedTurtle->Destroy();
		FTimerHandle handle1;
		GetWorldTimerManager().SetTimer(handle1, [OverlappedTurtle]() {
			OverlappedTurtle->Destroy();
			}, 0.1f, false);

		GameMode->SpawnBlood(OverlappedTurtle->GetActorLocation());
		if (GameMode) {
			GameMode->increaseScore(150);
		}

		UpdateCurrentHealth(15);
	}

	int n = FMath::RandRange(0, 100);

	if (n < 38)
		GameMode->SpawnFishAtRandomLocation(0);
	else if (n < 76)
		GameMode->SpawnFishAtRandomLocation(1);
	else if (n < 95)
		GameMode->SpawnFishAtRandomLocation(2);
	else
		GameMode->SpawnFishAtRandomLocation(3);
}

void ASharkCharacter::KillFish(AActor* fish)
{
	fish->Destroy();
}