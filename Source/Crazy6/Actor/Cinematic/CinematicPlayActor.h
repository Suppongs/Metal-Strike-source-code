// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CinematicPlayActor.generated.h"

UCLASS()
class CRAZY6_API ACinematicPlayActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACinematicPlayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void CinematicPlay();

public:
	TObjectPtr<class UMaterial> GetVideoMT() { return mCinematicVideoMT; }
	TObjectPtr<class UMediaPlayer> GetVideoPlayer() { return mMediaPlayer; }
	TObjectPtr<class UMediaSoundComponent> GetVideoSoundComp() { return mMediaSound; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Cinematic")
	// For Listen MediaPlayer Sound
	TObjectPtr<class UMediaSoundComponent> mMediaSound;

	UPROPERTY(BlueprintReadOnly, Category = "Cinematic")
	// For Listen MediaPlayer Sound
	TObjectPtr<class UMediaPlayer> mMediaPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Cinematic")
	TObjectPtr<UMaterial> mCinematicVideoMT;

public:
	UFUNCTION()
	void PlayEndController();

	UFUNCTION()
	void PlayController();





};
