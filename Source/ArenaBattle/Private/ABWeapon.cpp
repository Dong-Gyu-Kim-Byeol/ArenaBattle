// Fill out your copyright notice in the Description page of Project Settings.


#include "ABWeapon.h"

// Sets default values
AABWeapon::AABWeapon() :
	mpWeapon(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"))),
	mAttackRange(150.0f),
	mAttackDamageMin(-2.5f),
	mAttackDamageMax(10.0f),
	mAttackModifierMin(0.85f),
	mAttackModifierMax(1.25f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = mpWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sWeaponMesh(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword22/SK_Blade_HeroSword22.SK_Blade_HeroSword22"));
	ensure(sWeaponMesh.Succeeded());
	mpWeapon->SetSkeletalMesh(sWeaponMesh.Object);

	mpWeapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AABWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	mAttackDamage = FMath::RandRange(mAttackDamageMin, mAttackDamageMax);
	mAttackModifier = FMath::RandRange(mAttackModifierMin, mAttackModifierMax);

	ABLOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), mAttackDamage, mAttackModifier)
}

void AABWeapon::EndPlay(const EEndPlayReason::Type _endPlayReason)
{
	Super::EndPlay(_endPlayReason);
}

float AABWeapon::GetAttackRange() const
{
	return mAttackRange;
}

float AABWeapon::GetAttackDamage() const
{
	return mAttackDamage;
}

float AABWeapon::GetAttackModifier() const
{
	return mAttackModifier;
}