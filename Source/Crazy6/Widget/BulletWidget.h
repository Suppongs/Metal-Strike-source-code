// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletWidget.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UBulletWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mCurrentbullet;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mClipbullet;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMaxBullet;

protected:
	virtual void NativeOnInitialized() override;
public:
	// if bullet use, reloading
	void SetCurrentBullet(int32 mCurrentAmmo);
	void SetClipBullet(int32 mClipAmmo);
	void SetMaxBullet(int32 mMaxAmmo);



};
