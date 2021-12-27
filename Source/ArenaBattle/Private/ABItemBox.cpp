// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox() :
	mpTrigger(CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"))),
	mpBox(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"))),
	mpEffect(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"))),
	mWeaponItemClass(AABWeapon::StaticClass())
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = mpTrigger;
	mpBox->SetupAttachment(RootComponent);
	mpEffect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sBoxMesh(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	ensure(sBoxMesh.Succeeded());
	mpBox->SetStaticMesh(sBoxMesh.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> sEffectParticle(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	ensure(sEffectParticle.Succeeded());
	mpEffect->SetTemplate(sEffectParticle.Object);
	mpEffect->bAutoActivate = false;

	mpTrigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	mpBox->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	mpTrigger->SetCollisionProfileName(TEXT("ItemBox"));
	mpBox->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FClassFinder<AABWeapon> sWeaponBlueprint(TEXT("/Game/Book/Blueprints/BP_WeaponAxe.BP_WeaponAxe_C"));
	ensure(sWeaponBlueprint.Succeeded());
	mWeaponItemClass = sWeaponBlueprint.Class;
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();

}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mpTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}


//private

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor,
	UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _rSweepResult)
{
	ABLOG_S(Warning);

	AABCharacter* pABCharacter = CastChecked<AABCharacter>(_pOtherActor);
	check(pABCharacter != nullptr);
	pABCharacter = ExactCast<AABCharacter>(_pOtherActor);
	check(pABCharacter != nullptr);

	check(mWeaponItemClass != nullptr);
	AABWeapon* pABWeapon = GetWorld()->SpawnActor<AABWeapon>(mWeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	pABCharacter->SetWeapon(pABWeapon);

	mpBox->SetHiddenInGame(true, true);
	SetActorEnableCollision(false);

	mpEffect->Activate(true);
	mpEffect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* _pParticleSystem)
{
	Destroy();
}