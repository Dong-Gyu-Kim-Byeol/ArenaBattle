// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type _endPlayReason) override;

public:
	UPROPERTY(VisibleAnywhere, Category = State)
		class UABCharacterStatComponent* mpCharacterStat;

	UPROPERTY(EditAnywhere, Category = State)
		float mPlayerWalkSpeed;

	UPROPERTY(EditAnywhere, Category = State)
		float mNPCWalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* mpHPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* mpSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* mpCamera;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<class AABWeapon> mWeaponClass;

	


	// Called every frame
	virtual void Tick(float _deltaTime) override;

	virtual void PostInitializeComponents() override;

	void SetCharacterState(const ECharacterState _newState);

	ECharacterState GetCharacterState() const;
	int32 GetDropExp() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* _pNewController) override;

	void OnAssetLoadCompleted();

	virtual float TakeDamage(float _damageAmount, struct FDamageEvent const& _rDamageEvent, class AController* _pEventIndtigator, AActor* _pDamageCauser) override;

	void Attack();
	bool GetIsAattacking() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

	void SetWeapon(class AABWeapon* _pNewWeapon);

	void OnDead();

	const class UABAnimInstance* GetABAnim() const;
	bool GetIsDead() const;

protected:


private:
	FSoftObjectPath mCharacterAssetToLoad;
	TSharedPtr<struct FStreamableHandle> mpAssetStreamingHandle;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};
	EControlMode mCurrentControlMode;
	FVector mDirectionToMove;
	
	float mArmLengthTo;
	FRotator mArmRotationTo;
	static const float msARM_LENGTH_SPEED;
	static const float msARM_ROTATION_SPEED;
	bool mbViewChangeLength;
	bool mbViewChangeRotation;
	float mViewChangeTime;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool mbIsAttacking;
	UPROPERTY()
		class UABAnimInstance* mpABAnim;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool mbCanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool mbIsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 mCurrentCombo;
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	static const int32 msMAX_COMBO;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float mAttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float mAttackRadius;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AABWeapon* mpCurrentWeapon;

	uint32 mAssetIndex;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState mCurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool mbIsPlayer;
	UPROPERTY()
		class AABAIController* mpABAIController;
	UPROPERTY()
		class AABPlayerController* mpABPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float mDEAD_TIMER;
	FTimerHandle mDeadTimerHande;

	
	void SetControlMode(EControlMode _controlMode);

	void ViewChange();
	void ViewChangeInterpolation(float _deltaTime);
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* _pMontage, bool _bInterrupred);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	void UpDown(float _newAxisValue);
	void LeftRight(float _newAxisValue);
	void LookUp(float _newAxisValue);
	void Turn(float _newAxisValue);

	

	
};
