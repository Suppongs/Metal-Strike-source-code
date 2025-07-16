// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LoadingSettings.generated.h"

/**
 * 
 */
UCLASS(config = CustomLoading, DefaultConfig, meta = (DisplayName = "CustomLoading"))
class CRAZY6_API ULoadingSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Textrue")
	TArray<TSoftObjectPtr<UTexture2D>> mTextureArray;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Texture")
	TSubclassOf<UUserWidget> mWidgetClass;

	//UPROPERTY(Config, EditAnywhere, Category = "Texture")
	//float mLoadingImageChangeTime;	
	
};
