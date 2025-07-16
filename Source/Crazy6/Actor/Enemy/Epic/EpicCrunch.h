// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "EpicCrunch.generated.h"

class UEnemyAnimTemplate;
class UCapsuleComponent;
class UBoxComponent;
class UEpicAppearanceWidget;
class UNiagaraComponent;
class UNiagaraSystem;
class UParticleSystem;
class USoundBase;

/**
 * 
 */
UCLASS()
class CRAZY6_API AEpicCrunch : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AEpicCrunch();

protected:
	void BeginPlay() override;

public:
	void SetAIState(EAIState State) override;
	void Attack() override;
	void CallBack_Hit(float Damage) override;
	void CallBack_HpZERO() override;

	void Tick(float DeltaTime) override;
	
	void WhenRushEnd();

	void AttackHitBoxOn() override;
	void AttackHitBoxOff() override;


private:
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastResumeMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRushAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHandHitFX(AActor* OtherActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRushHitFX(UPrimitiveComponent* OtherComp);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastEpicDeath();

	UFUNCTION()
	void OnRep_mServerIsRushAttackReady();
	
	void SwitchTrueIsRushAttackReady();
	void ChargeEnd();
	
	UFUNCTION()
	void OnBegineOverlap_Hand(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBegineOverlap_Rush(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//UI
	UFUNCTION()
	void VisibleWidget();
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> mRightArmCapsule = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> mLeftArmCapsule = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mHandHitBoxLeft = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mHandHitBoxRight = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mRushHitBox = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mHeadShotHitBox = nullptr;

	FTimerHandle mRushAttackTimer;
	float mRushAttackCoolTime = 8.0f;

	FTimerHandle mRushAttackChargeTimer;
	float mRushAttackChargingTime = 1.0f;

	float mRushDistance = 1800.f;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget>	mWidgetClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEpicAppearanceWidget> mWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mRushAttackIndicator = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mHitParticle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mHandHitSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mChargingSound = nullptr;
	
	bool IsRushing = false;
	bool IsLeftHandAttacking = false;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_mServerIsRushAttackReady)
	bool mServerIsRushAttackReady = true;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> mRushAttackedCollection;
	
};
