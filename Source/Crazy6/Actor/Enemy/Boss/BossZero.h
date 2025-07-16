// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "BossZero.generated.h"

class UBossAttackCameraShake;
class UBossAISpawner;
class UNiagaraSystem;
class UNiagaraComponent;
class UParticleSystem;
class USoundBase;
class ABomb;
class UBoxComponent;
class USphereComponent;
class UBeam;
class AEnergyBall;
/**
 * 
 */
UCLASS()
class CRAZY6_API ABossZero : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABossZero();

protected:
	void BeginPlay() override;

public:
	void PossessedBy(AController* NewController) override;
	void Tick(float DeltaTime) override;

public:
	void SetAIState(EAIState State) override;

	void Attack() override;
	float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
	void CallBack_Hit(float Damage) override;
	void CallBack_HpZERO() override;
	
	void Shooting();
	void LaunchEnergyBall(const AEnergyBall* EnergyBall);
	void Laser();
	void Rolling();
	void Slam();
	void Bomb();
	void SpawnBomb();
	void AdvancedLaser();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastResumeAttackMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastCameraShake();

private:
	void SwitchAttackModeRandom();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastBossDeath();

	UFUNCTION()
	void OnBegineOverlap_Rolling(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLaser();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRollingSound();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRollingHitFX(AActor* OtherActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSlamLandingFX();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSlamHitFX(AActor* OtherActor);

	UFUNCTION()
	void OnRep_mCurrentPhase();

	UFUNCTION()
	void OnRep_mServerAttackMode();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// ==== Chaos
public:
	//void SpawnGeometryCollection(const FVector& Location);

public:
	void TimeHandleSpawnGC(float DT);

	UFUNCTION(Server, Reliable)
	void Server_SpawnGeometrycollection();



private:
	UPROPERTY()
	TSubclassOf<class ABossGeometryCollectionActor> mGCActorClass = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> mBossHitBox = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mArmorHitBox = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> mRollingHitBox = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> mSlamHitBox = nullptr;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_mCurrentPhase)
	EBossPhase mCurrentPhase = EBossPhase::Phase1;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_mServerAttackMode)
	EBossAttackMode mServerAttackMode = EBossAttackMode::Shooting;

	EBossAttackMove mAttackMove = EBossAttackMove::Nothing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> mTarget = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnergyBall> mEnergyBallClass = nullptr;
	UPROPERTY(EditAnywhere)
	float mEnergyBallLaunchSpeed = 2500.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBeam> mBeam;
	
	FVector mSlamDestination = FVector::ZeroVector;

	FTimerHandle mBombTimer;
	float mBombTime = 0.5f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABomb> mBombClass = nullptr;

	FTimerHandle mAdvancedLaserMoveTimer;
	float mAdvancedLaserMovingTime = 2.f;

	TObjectPtr<UBossAISpawner> mAISpawner = nullptr;
	FTimerHandle mAISpawnTimer;
	float mAISpawnTime = 5.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mPhase2Visual = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mPhase3Visual = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mLaserChargeSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mRollingHitParticle = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mRollingSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mRollingHitSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> mSlamFX = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mSlamSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mSlamHitParticle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mSlamHitSound = nullptr;

	TSubclassOf<UBossAttackCameraShake> mCameraShakeClass = nullptr;

	// ========= Boss HP ========
	TSubclassOf<UUserWidget> mBossHPWidgetclass = nullptr;
	TObjectPtr<class UMainBossHPbar> mBossHPWidget = nullptr;
	UPROPERTY(Replicated)
	float CurrentHP;

	// ======== Mission Clear Widget =======
	TSubclassOf<UUserWidget> mMissionClearWidgetclass = nullptr;
	TObjectPtr<UUserWidget> mMissionClearWidget = nullptr;
	FTimerHandle mRestartTimer;
	
public:
	// ==  UI Percent = 0.0~1.0 ===== 
	void BossHP(float Percent);
	UFUNCTION(NetMulticast,Unreliable)
	void Net_BossHP(float Percent);
	
	void BossHPAfterimage(float Percent);
	UFUNCTION(NetMulticast,Unreliable)
	void Net_BossHPAfterimage(float Percent);

	void GoMain();
	void UnVisibleUI();
	UFUNCTION(NetMulticast,Unreliable)
	void Net_UnVisibleUI();


private:
	virtual void Callback_SetUIHp(float CurHp) override;
	
	
};
