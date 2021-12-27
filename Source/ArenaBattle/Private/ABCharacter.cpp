// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABCharacterSetting.h"
#include "ABHUDWidget.h"
#include "Engine/AssetManager.h"
#include "ABGameMode.h"


//public

// Sets default values
const float AABCharacter::msARM_LENGTH_SPEED(3.0f);
const float AABCharacter::msARM_ROTATION_SPEED(10.0f);

const int32 AABCharacter::msMAX_COMBO(4);

AABCharacter::AABCharacter() :
	mpCharacterStat(CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CharacterState"))),
	mpHPBarWidget(CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"))),
	mpSpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"))),
	mpCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"))),
	mCharacterAssetToLoad(FSoftObjectPath(nullptr)),
	mDirectionToMove(FVector::ZeroVector),
	mArmLengthTo(0.0f),
	mArmRotationTo(FRotator::ZeroRotator),
	mbViewChangeLength(false),
	mbViewChangeRotation(false),
	mViewChangeTime(0.0f),
	mbIsAttacking(false),
	mAttackRange(80.0f),
	mAttackRadius(50.0f),
	mPlayerWalkSpeed(600.0f),
	mNPCWalkSpeed(300.0f),
	mCurrentState(ECharacterState::PREINIT),
	mAssetIndex(4),
	mDEAD_TIMER(2.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpSpringArm->SetupAttachment(GetCapsuleComponent());
	mpCamera->SetupAttachment(mpSpringArm);
	mpHPBarWidget->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sMeshCardboard(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	ensure(sMeshCardboard.Succeeded());
	GetMesh()->SetSkeletalMesh(sMeshCardboard.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<AABWeapon> sWeaponBlueprint(TEXT("/Game/Book/Blueprints/BP_WeaponBlackKnight.BP_WeaponBlackKnight_C"));
	ensure(sWeaponBlueprint.Succeeded());
	mWeaponClass = sWeaponBlueprint.Class;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> sWorriorAnimation(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	ensure(sWorriorAnimation.Succeeded());
	GetMesh()->SetAnimInstanceClass(sWorriorAnimation.Class);

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	mpHPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	mpHPBarWidget->SetWidgetSpace(EWidgetSpace::Screen/*EWidgetSpace::World*/);

	static ConstructorHelpers::FClassFinder<UUserWidget> sHPBarWidget(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	ensure(sHPBarWidget.Succeeded());
	mpHPBarWidget->SetWidgetClass(sHPBarWidget.Class);
	mpHPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

//protected

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//const bool bIsImplemented = AABCharacter::StaticClass()->ImplementsInterface(UNavAgentInterface::StaticClass());
	// 인터페이스는 다중상속이 가능해서 3중 for

	const UABCharacterSetting* const pDefaultSetting = GetDefault<UABCharacterSetting>();
	mbIsPlayer = IsPlayerControlled();

	if (mbIsPlayer == true)
	{
		mpABPlayerController = Cast<AABPlayerController>(GetController());
		check(mpABPlayerController != nullptr);

		const AABPlayerState* const pABPlayerState = ExactCast<AABPlayerState>(GetPlayerState());
		mAssetIndex = pABPlayerState->GetCharacterMeshIndex();
	}
	else
	{
		mpABAIController = Cast<AABAIController>(GetController());
		check(mpABAIController != nullptr);

		mAssetIndex = FMath::RandRange(0, pDefaultSetting->mCharacterAssets.Num() - 1);
	}

	mCharacterAssetToLoad = pDefaultSetting->mCharacterAssets[mAssetIndex];
	mpAssetStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(mCharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
}

void AABCharacter::EndPlay(const EEndPlayReason::Type _endPlayReason)
{
	Super::EndPlay(_endPlayReason);

	if (mpCurrentWeapon != nullptr)
	{
		mpCurrentWeapon->Destroy();
		mpCurrentWeapon = nullptr;
	}
}



//public

// Called every frame
void AABCharacter::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	ViewChangeInterpolation(_deltaTime);

	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		// empty
		break;
	case EControlMode::DIABLO:
		if (mDirectionToMove.SizeSquared() > 0.0f)
		{
			AddMovementInput(mDirectionToMove);
		}
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mpABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	check(mpABAnim != nullptr);

	mpABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	mpABAnim->mOnNextAttackCheck.AddLambda([this]() -> void {
		mbCanNextCombo = false;

		if (mbIsComboInputOn == true)
		{
			AttackStartComboState();
			mpABAnim->JumpToAttackMontageSection(mCurrentCombo);
		}
		});

	mpABAnim->mOnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
}

void AABCharacter::SetCharacterState(const ECharacterState _newState)
{
	check(mCurrentState != _newState);
	mCurrentState = _newState;

	switch (mCurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (mbIsPlayer == true)
		{
			DisableInput(mpABPlayerController);

			mpABPlayerController->GetHUDWidget()->BindCharacterStat(mpCharacterStat);

			const AABPlayerState* const pABPlayerState = Cast<AABPlayerState>(GetPlayerState());
			check(pABPlayerState != nullptr);
			mpCharacterStat->SetNewLevel(pABPlayerState->GetCharacterLevel());
		}
		else
		{
			const AABGameMode* const pABGameMode = ExactCast<AABGameMode>(GetWorld()->GetAuthGameMode());
			check(pABGameMode != nullptr);

			const int32 targetLevel = FMath::CeilToInt((float)(pABGameMode->GetScore()) * 0.8f);

			const int32 finalLevel = FMath::Clamp(targetLevel, 1, 20); // change get max or min
			ABLOG(Warning, TEXT("New NPC Level : %d"), finalLevel);

			mpCharacterStat->SetNewLevel(finalLevel);
		}

		SetActorHiddenInGame(true);
		mpHPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		mpHPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;

		mpCharacterStat->mOnHPIsZero.AddLambda([this]() -> void {
			ABLOG(Warning, TEXT("%s"), TEXT("OnHPIsZero"));

			SetCharacterState(ECharacterState::DEAD);
			});

		UABCharacterWidget* pCharacterWidget = Cast<UABCharacterWidget>(mpHPBarWidget->GetUserWidgetObject());
		check(pCharacterWidget != nullptr);
		pCharacterWidget->BindCharacterState(mpCharacterStat);

		if (mbIsPlayer == true)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = mPlayerWalkSpeed;

			EnableInput(mpABPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = mNPCWalkSpeed;

			mpABAIController->RunAI();
		}

		break;
	}
	case ECharacterState::DEAD:
	{
		mpABAnim->SetDeadAnim();
		SetActorEnableCollision(false);

		GetMesh()->SetHiddenInGame(false);
		mpHPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;

		if (mbIsPlayer == false)
		{
			mpABAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(mDeadTimerHande, FTimerDelegate::CreateUObject(this, &AABCharacter::OnDead), mDEAD_TIMER, false);
		
		
		break;
	}
	default:
		checkNoEntry();
	}
}

ECharacterState AABCharacter::GetCharacterState() const
{
	return mCurrentState;
}

int32 AABCharacter::GetDropExp() const
{
	check(mpCharacterStat != nullptr);

	return mpCharacterStat->GetDropExp();
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	_playerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	_playerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABCharacter::Jump);
	_playerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

	_playerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	_playerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	_playerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	_playerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

void AABCharacter::PossessedBy(AController* _pNewController)
{
	Super::PossessedBy(_pNewController);
}

void AABCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* pAssetLoaded = Cast<USkeletalMesh>(mpAssetStreamingHandle->GetLoadedAsset());
	mpAssetStreamingHandle.Reset();
	ensure(pAssetLoaded != nullptr);

	GetMesh()->SetSkeletalMesh(pAssetLoaded);

	check(mpCurrentWeapon == nullptr);
	const FName weaponSocket(TEXT("hand_rSocket"));
	if (mbIsPlayer == false)
	{
		mpCurrentWeapon = GetWorld()->SpawnActor<AABWeapon>(mWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		check(mpCurrentWeapon != nullptr);

		mpCurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, weaponSocket);

	}
	
	SetCharacterState(ECharacterState::READY);
}

float AABCharacter::TakeDamage(float _damageAmount, FDamageEvent const& _rDamageEvent, AController* _pEventIndtigator, AActor* _pDamageCauser)
{
	float finalDamage = Super::TakeDamage(_damageAmount, _rDamageEvent, _pEventIndtigator, _pDamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damamge : %f"), *GetName(), finalDamage);

	mpCharacterStat->SetDamage(finalDamage);
	if (mCurrentState == ECharacterState::DEAD)
	{
		if (_pEventIndtigator->IsPlayerController() == true)
		{
			AABPlayerController* pABPlayerController = Cast<AABPlayerController>(_pEventIndtigator);
			check(pABPlayerController != nullptr);

			pABPlayerController->NPCKill(this);
		}
	}

	return finalDamage;
}

void AABCharacter::Attack()
{
	if (mbIsAttacking == true)
	{
		ABCHECK(FMath::IsWithinInclusive(mCurrentCombo, 1, msMAX_COMBO));
		if (mbCanNextCombo == true)
		{
			mbIsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(mCurrentCombo == 0);

		AttackStartComboState();
		mpABAnim->PlayAttackMontage();
		mpABAnim->JumpToAttackMontageSection(mCurrentCombo);
		mbIsAttacking = true;
	}
}

bool AABCharacter::GetIsAattacking() const
{
	return mbIsAttacking;
}

float AABCharacter::GetFinalAttackRange() const
{
	return (mpCurrentWeapon != nullptr) ? mpCurrentWeapon->GetAttackRange() : mAttackRange;
}

float AABCharacter::GetFinalAttackDamage() const
{
	const float attackDamage = (mpCurrentWeapon != nullptr) ?
		(mpCharacterStat->GetAttack() + mpCurrentWeapon->GetAttackDamage()) : mpCharacterStat->GetAttack();

	const float attackModifier = (mpCurrentWeapon != nullptr) ?
		mpCurrentWeapon->GetAttackModifier() : 1.0f;

	return attackDamage * attackModifier;

}

void AABCharacter::SetWeapon(AABWeapon* _pNewWeapon)
{
	check(_pNewWeapon != nullptr);
	FName weaponSocket(TEXT("hand_rSocket"));

	if (mpCurrentWeapon != nullptr)
	{
		FDetachmentTransformRules detachRules(EDetachmentRule::KeepWorld, false);
		mpCurrentWeapon->DetachAllSceneComponents(GetMesh(), detachRules);
		mpCurrentWeapon->Destroy();
		mpCurrentWeapon = nullptr;
	}

	_pNewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, weaponSocket);
	_pNewWeapon->SetOwner(this);

	check(mpCurrentWeapon == nullptr);
	mpCurrentWeapon = _pNewWeapon;
}

void AABCharacter::OnDead()
{
	GetWorld()->GetTimerManager().ClearTimer(mDeadTimerHande);

	if (mbIsPlayer == true)
	{
		mpABPlayerController->ShowResultUI();
	}
	else
	{
		Destroy();
	}
}

const UABAnimInstance* AABCharacter::GetABAnim() const
{
	return mpABAnim;
}

bool AABCharacter::GetIsDead() const
{
	return GetABAnim()->GetDeadAnim();
}



//protected



//private

void AABCharacter::SetControlMode(EControlMode _controlMode)
{
	switch (_controlMode)
	{
	case EControlMode::GTA:
		mArmLengthTo = 450.0f;
		mArmRotationTo = FRotator(-30.0f, 0.0f, 0.0f) + GetActorRotation();
		mpSpringArm->bUsePawnControlRotation = true;
		mpSpringArm->bInheritPitch = true;
		mpSpringArm->bInheritRoll = true;
		mpSpringArm->bInheritYaw = true;
		mpSpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		mArmLengthTo = 800.0f;
		mArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		mpSpringArm->bUsePawnControlRotation = false;
		mpSpringArm->bInheritPitch = false;
		mpSpringArm->bInheritRoll = false;
		mpSpringArm->bInheritYaw = false;
		mpSpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	default:
		checkNoEntry();
	}

	mCurrentControlMode = _controlMode;
	mbViewChangeLength = true;
	mbViewChangeRotation = true;
	mViewChangeTime = GetWorld()->GetTimeSeconds();
}

void AABCharacter::ViewChange()
{
	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		mpSpringArm->RelativeRotation = GetController()->GetControlRotation();
		SetControlMode(EControlMode::DIABLO);
		mDirectionToMove = GetActorForwardVector();
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(mpSpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

void AABCharacter::ViewChangeInterpolation(float _deltaTime)
{
	if (mbViewChangeLength == true)
	{
		mpSpringArm->TargetArmLength = FMath::FInterpTo(mpSpringArm->TargetArmLength, mArmLengthTo, _deltaTime, msARM_LENGTH_SPEED);
		if (mpSpringArm->TargetArmLength == mArmLengthTo)
		{
			mbViewChangeLength = false;
		}
	}

	if (mbViewChangeRotation == true)
	{
		switch (mCurrentControlMode)
		{
		case EControlMode::GTA:
			GetController()->SetControlRotation(FMath::RInterpTo(GetController()->GetControlRotation(), mArmRotationTo, _deltaTime, msARM_ROTATION_SPEED));
			if (GetWorld()->GetTimeSeconds() - mViewChangeTime > 1.0f)
			{
				mbViewChangeRotation = false;
			}
			break;
		case EControlMode::DIABLO:
			mpSpringArm->RelativeRotation = FMath::RInterpTo(mpSpringArm->RelativeRotation, mArmRotationTo, _deltaTime, msARM_ROTATION_SPEED);
			if (mpSpringArm->RelativeRotation == mArmRotationTo)
			{
				mbViewChangeRotation = false;
			}
			break;
		case EControlMode::NPC:
			// empty
			break;
		default:
			checkNoEntry();
		}
	}
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* _pMontage, bool _bInterrupred)
{
	ABCHECK(mbIsAttacking == true);
	ABCHECK(mCurrentCombo > 0);

	AttackEndComboState();
}

void AABCharacter::AttackStartComboState()
{
	mbCanNextCombo = true;
	mbIsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(mCurrentCombo, 0, msMAX_COMBO));
	mCurrentCombo = FMath::Clamp<int32>(mCurrentCombo + 1, 1, msMAX_COMBO);
}

void AABCharacter::AttackEndComboState()
{
	mbIsAttacking = false;
	mbCanNextCombo = false;
	mbIsComboInputOn = false;
	mCurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	const float finalAttackRange = GetFinalAttackRange();

	FHitResult hitResult;
	FCollisionQueryParams queryParams(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * finalAttackRange,
		FQuat::Identity,
		ECollisionChannel_TRACE_Attack,
		FCollisionShape::MakeSphere(mAttackRadius),
		queryParams,
		FCollisionResponseParams::DefaultResponseParam);	// block only

#if ENABLE_DRAW_DEBUG
	{
		FVector traceVec = GetActorForwardVector() * finalAttackRange;
		FVector center = GetActorLocation() + traceVec * 0.5f;
		float halfHeight = finalAttackRange * 0.5f + mAttackRadius;
		FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
		FColor drawColor = bResult == true ? FColor::Green : FColor::Red;
		float debugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(), center, halfHeight, mAttackRadius, capsuleRot, drawColor, false, debugLifeTime);
	}
#endif

	if (bResult == true)
	{
		if (hitResult.Actor.IsValid() == true)
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *hitResult.Actor->GetName());

			FDamageEvent damageEvent;
			hitResult.Actor->TakeDamage(GetFinalAttackDamage(), damageEvent, GetController(), this);
		}
	}
}

void AABCharacter::UpDown(float _newAxisValue)
{
	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), _newAxisValue);
		break;
	case EControlMode::DIABLO:
		mDirectionToMove.X = _newAxisValue;
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

void AABCharacter::LeftRight(float _newAxisValue)
{
	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), _newAxisValue);
		break;
	case EControlMode::DIABLO:
		mDirectionToMove.Y = _newAxisValue;
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

void AABCharacter::LookUp(float _newAxisValue)
{
	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(_newAxisValue);
		break;
	case EControlMode::DIABLO:
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

void AABCharacter::Turn(float _newAxisValue)
{
	switch (mCurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(_newAxisValue);
		break;
	case EControlMode::DIABLO:
		break;
	case EControlMode::NPC:
		// empty
		break;
	default:
		checkNoEntry();
	}
}

