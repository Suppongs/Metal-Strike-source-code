// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidgetData.h"
#include "UIHeader.h"

UDamageWidgetData::UDamageWidgetData()
{
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>
			Asset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/DamageFont.DamageFont_C'"));

		if (Asset.Succeeded())
		{
			mWidgetComponent = Asset.Class;
		}
	}
}


