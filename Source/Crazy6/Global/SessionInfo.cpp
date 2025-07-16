// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfo.h"

void USessionObject::Initialize(TSharedPtr<FSessionInfo> SessionInfo)
{
    if (SessionInfo.IsValid())
    {
        MySessionData = SessionInfo;
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(3, 15, FColor::Blue, FString::Printf(TEXT("Initialize Error")));
    }
}
