// Fill out your copyright notice in the Description page of Project Settings.


#include "ABSection.h"
#include "ABCharacter.h"
#include "ABItemBox.h"
#include "ABPlayerController.h"
#include "ABGameMode.h"


const float AABSection::mENEMY_SPAWN_TIME(2.0f);
const float AABSection::mITEM_BOX_SPAWN_TIME(5.0f);

//public

// Sets default values
AABSection::AABSection() :
	mpMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
	mpTrigger(CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"))),
	mCurrentState(ESectionState::READY),
	mbNoBattle(false),
	mSpawnNPCTimerHandle({ }),
	mSpawnItemBoxTimerHandle({ })
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = mpMesh;
	const FString meshSssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sMeshSquare(*meshSssetPath);
	ensure(sMeshSquare.Succeeded());
	mpMesh->SetStaticMesh(sMeshSquare.Object);

	mpTrigger->SetupAttachment(mpMesh);
	mpTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	mpTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	mpTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
	mpTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);

	const FString gateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sGateMeshGate(*gateAssetPath);
	ensure(sGateMeshGate.Succeeded());

	static const FName sGateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (const FName& rGateSocket : sGateSockets)
	{
		ensure(mpMesh->DoesSocketExist(rGateSocket));
		
		UStaticMeshComponent* pNewGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(*rGateSocket.ToString());
		pNewGateMesh->SetStaticMesh(sGateMeshGate.Object);
		pNewGateMesh->SetupAttachment(mpMesh, rGateSocket);
		pNewGateMesh->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));

		mGateMeshes.Add(pNewGateMesh);

		UBoxComponent* pNewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*rGateSocket.ToString().Append(TEXT("Trigger")));
		pNewGateTrigger->SetupAttachment(mpMesh, rGateSocket);
		pNewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		pNewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		pNewGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		
		pNewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		pNewGateTrigger->ComponentTags.Add(rGateSocket);

		mGateTriggers.Add(pNewGateTrigger);
	}
}

void AABSection::OnConstruction(const FTransform& _rTransform)
{
	Super::OnConstruction(_rTransform);

	SetState(mbNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}


//protected

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(mbNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}


//private

void AABSection::SetState(const ESectionState _newState)
{
	FName triggerProfile;
	FName gateTriggerProfile;
	bool bOpenGates;

	switch (_newState)
	{
	case ESectionState::READY:
		triggerProfile = TEXT("ABTrigger");
		gateTriggerProfile = TEXT("NoCollision");
		bOpenGates = true;
		break;
	case ESectionState::BATTLE:
		triggerProfile = TEXT("NoCollision");
		gateTriggerProfile = TEXT("NoCollision");
		bOpenGates = false;

		GetWorld()->GetTimerManager().SetTimer(mSpawnNPCTimerHandle, 
			FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn), mENEMY_SPAWN_TIME, false);
		GetWorld()->GetTimerManager().SetTimer(mSpawnItemBoxTimerHandle, 
			FTimerDelegate::CreateUObject(this, &AABSection::OnItemBoxSpawn), mITEM_BOX_SPAWN_TIME, false);
		break;
	case ESectionState::COMPLETE:
		triggerProfile = TEXT("NoCollision");
		gateTriggerProfile = TEXT("ABTrigger");
		bOpenGates = true;
		break;
	default:
		checkNoEntry();
	}

	mpTrigger->SetCollisionProfileName(triggerProfile);
	for (UBoxComponent* pGateTrigger : mGateTriggers)
	{
		pGateTrigger->SetCollisionProfileName(gateTriggerProfile);
	}

	OperateGates(bOpenGates);

	mCurrentState = _newState;
}

void AABSection::OperateGates(const bool _bOpen)
{
	for (UStaticMeshComponent* pGateMesh : mGateMeshes)
	{
		pGateMesh->SetRelativeRotation(_bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AABSection::OnTriggerBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor,
	UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep,
	const FHitResult& _rSweepResult)
{
	if (mCurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void AABSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor,
	UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep,
	const FHitResult& _rSweepResult)
{
	ensure(_pOverlappedComponent->ComponentTags.Num() == 1);

	const FName componentTag = _pOverlappedComponent->ComponentTags[0];
	const FName socketName = FName(*componentTag.ToString().Left(2));

	if (mpMesh->DoesSocketExist(socketName) == false)
	{
		return;
	}

	const FVector newLocation = mpMesh->GetSocketLocation(socketName);

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams objectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		overlapResults,
		newLocation,
		FQuat::Identity,
		objectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		collisionQueryParam);

	if (bResult == false)
	{
		AABSection* pNewSection = GetWorld()->SpawnActor<AABSection>(newLocation, FRotator::ZeroRotator);
	}
#if DO_CHECK
	else
	{
		ABLOG(Warning, TEXT("%s"), TEXT("is not Empty"));
	}
#endif // DO_CHECK
}

void AABSection::OnNPCSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(mSpawnNPCTimerHandle);

	AABCharacter* pKeyNPC = GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);
	check(pKeyNPC != nullptr);

	pKeyNPC->OnDestroyed.AddDynamic(this, &AABSection::OnKeyNPCDestroyed);
}

void AABSection::OnItemBoxSpawn()
{
	const float circleRaadius = 600.0f;
	const FVector2D randXY = FMath::RandPointInCircle(circleRaadius);
	GetWorld()->SpawnActor<AABItemBox>(GetActorLocation() + FVector(randXY, 30.0f), FRotator::ZeroRotator);
}

void AABSection::OnKeyNPCDestroyed(AActor* _pDestroyedActor)
{
	AABCharacter* pABCharacter = Cast<AABCharacter>(_pDestroyedActor);
	check(pABCharacter != nullptr);

	AABPlayerController* pABPlayerController = Cast<AABPlayerController>(pABCharacter->LastHitBy);
	check(pABPlayerController != nullptr);

	AABGameMode* pABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
	check(pABGameMode != nullptr);
	pABGameMode->AddScore(pABPlayerController);

	SetState(ESectionState::COMPLETE);
}