// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"

// Sets default values
AABPawn::AABPawn() :
	Capsule(CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"))),
	Mesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"))),
	Movement(CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"))),
	SpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"))),
	Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA")))
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = Capsule;
	Mesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sMeshCardboard(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	ensure(sMeshCardboard.Succeeded());
	Mesh->SetSkeletalMesh(sMeshCardboard.Object);

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> sAnimationWarrior(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	ensure(sAnimationWarrior.Succeeded());
	Mesh->SetAnimInstanceClass(sAnimationWarrior.Class);
}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AABPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* _newController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(_newController);
}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	_playerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	_playerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::UpDown(float _newAxisValue)
{
	AddMovementInput(GetActorForwardVector(), _newAxisValue);
}

void AABPawn::LeftRight(float _newAxisValue)
{
	AddMovementInput(GetActorRightVector(), _newAxisValue);
}

