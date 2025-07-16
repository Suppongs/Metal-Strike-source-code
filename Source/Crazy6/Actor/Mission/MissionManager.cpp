// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionManager.h"


#include "MissionActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "Crazy6/DebugMacros.h"

#include "Blueprint/UserWidget.h"
#include "Crazy6/Widget/MissionWidget.h"
#include "Crazy6/Widget/CinematicPlayWidget.h"

#include "Crazy6/Actor/Enemy/EnemyBase.h"
#include "Crazy6/GameMode/MainGameMode.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"

#include "Net/UnrealNetwork.h"


AMissionManager::AMissionManager()
{
	//NetWorking
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	// ======= CinematicWidget =======
	{
		static ConstructorHelpers::FClassFinder<UCinematicPlayWidget>
			CinematicAsset(TEXT(
				"/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Cinematic/CinematicPlayerWidget.CinematicPlayerWidget_C'"));

		if (CinematicAsset.Succeeded())
		{
			mCinematicWidgetClass = CinematicAsset.Class;
		}
	}
}

void AMissionManager::BeginPlay()
{
	Super::BeginPlay();
	// Delegate Bind (For UI)
	//mOnMissionChangeDelegate.AddUObject(this, &AMissionManager::Callback_MissionChange);

	if (HasAuthority())
	{
		// Mission Load
		if (false == mMissions.IsEmpty())
		{
			TObjectPtr<AMissionManager> manager = nullptr;

			for (auto It = mMissions.CreateIterator(); It; ++It)
			{

				AMissionActor* Mission = (*It);
				if (false == IsValid(Mission))
				{

					//	LOG_ERROR(TEXT("Failed to load mission"));
				}

				Mission->SetManager(this);
				manager = Mission->GetManager();

			}
			// Mission Start (if no start have basic vlaue -1)
			mCurrentMissionNumber = 0;
			mCurrentMission = mMissions[mCurrentMissionNumber];
			mCurrentMission->MissionStart();
		}
	}
	
}




void AMissionManager::MissionComplete()
{
	// Server Call
	++mCurrentMissionNumber;
	mCurrentMissionNumber = FMath::Clamp(mCurrentMissionNumber, 0, mMissions.Num());

	// ** Here Fix // 
	bool IsLastMission = (mMissions.Num() <= mCurrentMissionNumber);
	if (true == IsLastMission)
	{
		// UI
		if(HasAuthority())
		{
			FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
			if (CurrentLevelName != "Bridge_P")
			{
				CinematicPlay(); // for server
				mMissionCompleteUI = true;
				UE_LOG(LogTemp, Log, TEXT("Server: mMissionCompleteUI updated to True"));
			}
		}


		// All Missions Clear
		LOG_WARNING(TEXT("All Missions Complete"));
	}
	// If not, Next Mission
	else
	{
		NextMission();
	}
}

void AMissionManager::NextMission()
{
	// Server Call

	//LOG_WARNING(TEXT("Mission Change"));

	if (mMissions.Num() <= mCurrentMissionNumber)
	{
		//LOG_FATAL(TEXT("Mission Number is over"));
		return;
	}

	// TODO : KTH    (Not Use)
	// This is just interface. so, I plan to implement it later.
	//if (true == IsValid(mCurrentMission))
	//{
	//	mCurrentMission->EndMission();
	//}
	
	// if Mission Change Monster Delete
	if (HasAuthority())
	{
		for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
		{
			AEnemyBase* Actor = *It;
			if (IsValid(Actor))
			{
				Actor->Destroy();
			}
		}


		mCurrentMission = mMissions[mCurrentMissionNumber];

		if (true == IsValid(mCurrentMission))
		{
			mCurrentMission->MissionStart();
			mCurrentMission->ChangeWidget();

		}
	}
}

void AMissionManager::CinematicPlay()
{
	if (IsValid(mCinematicWidgetClass))
	{
		mCinematicWidget = CreateWidget<UCinematicPlayWidget>(GetWorld(), mCinematicWidgetClass);
		if (IsValid(mCinematicWidget))
		{
			mCinematicWidget->AddToViewport();
		}
	}
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AMissionManager::MapTravelFunction, 5.0f, false);
	//	mMediaPlayer->Play();
}

void AMissionManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMissionManager, mCurrentMissionNumber);
	DOREPLIFETIME(AMissionManager, mCurrentMission);
	DOREPLIFETIME(AMissionManager, mMissionCompleteUI);
}

void AMissionManager::OnRep_mMissionCompleteUI()
{
	// ===================
	// TODO : @KYJ

	// Add mission complete UI
	if (!GetWorld()->bIsTearingDown && IsValid(mCinematicWidgetClass))
	{

		mCinematicWidget = CreateWidget<UCinematicPlayWidget>(GetWorld(), mCinematicWidgetClass);
		if (IsValid(mCinematicWidget))
		{
			mCinematicWidget->AddToViewport();
		}
	}
}

void AMissionManager::OnRep_NewMissionWidgetChange()
{
	mCurrentMission->UpdateWidgetContent();

}

void AMissionManager::MapTravelFunction()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AMainGameMode* GM = Cast<AMainGameMode>(World->GetAuthGameMode());
			if(IsValid(GM))
			{
				GM->SetIsTraveling(true);
			}
			World->ServerTravel("Bridge_P?listen");
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
}


