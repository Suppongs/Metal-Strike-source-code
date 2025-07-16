// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget.h"
#include "Components/Image.h"
#include "Crazy6/Setting/LoadingSettings.h"

void ULoadingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mBack = Cast<UImage>(GetWidgetFromName(TEXT("Back")));
}

void ULoadingWidget::SetTexture(UTexture2D* RandomTexture)
{
	mBack->SetBrushFromTexture(RandomTexture);
	mBack->InvalidateLayoutAndVolatility();
}
