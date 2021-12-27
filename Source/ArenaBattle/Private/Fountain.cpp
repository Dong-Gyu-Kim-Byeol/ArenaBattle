// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"

// Sets default values
AFountain::AFountain() : 
	Body(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"))),
	Water(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"))),
	Light(CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"))),
	Splash(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"))),
	Movement(CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"))),
	mRotateSpeed(30.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = Body;
	Water->SetupAttachment(RootComponent);
	Light->SetupAttachment(RootComponent);
	Splash->SetupAttachment(RootComponent);

	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sBodyStaticMesh(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	ensure(sBodyStaticMesh.Succeeded());
	Body->SetStaticMesh(sBodyStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sWaterStaticMesh(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	ensure(sWaterStaticMesh.Succeeded());
	Water->SetStaticMesh(sWaterStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> sSplashParticleSystem(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));
	ensure(sSplashParticleSystem.Succeeded());
	Splash->SetTemplate(sSplashParticleSystem.Object);

	Movement->RotationRate = FRotator(0.0f, mRotateSpeed, 0.0f);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

