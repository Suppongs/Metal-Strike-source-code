#include "Crazy6GameInstance.h"
#include "Crazy6GameInstanceSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineIdentityInterface.h" 


UCrazy6GameInstance::UCrazy6GameInstance()
{
}

void UCrazy6GameInstance::Logout_Implementation()
{
	mGameInstanceSubsystem->LogoutPlayer();
	UE_LOG(LogTemp, Log, TEXT("UCrazy6GameInstance::LogOUT"));
}

void UCrazy6GameInstance::Init()
{
	Super::Init();
}

void UCrazy6GameInstance::Shutdown()
{
	mGameInstanceSubsystem = GetSubsystem<UCrazy6GameInstanceSubsystem>();
	if(IsValid(mGameInstanceSubsystem))
	{
		Logout();
		if (TEXT("") != mGameInstanceSubsystem->GetCurrentSessionName())
		{
			 APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if(IsValid(PlayerController))
			{
				if(PlayerController->HasAuthority())
				{
					// Session destroy
					IOnlineSessionPtr mSessionInterface = mGameInstanceSubsystem->GetSessionInterface();
					mSessionInterface->DestroySession(mGameInstanceSubsystem->GetCurrentSessionName());
					UE_LOG(LogTemp, Log, TEXT("UCrazy6GameInstance::DestroySession"));	
				}
				else
				{
					FUniqueNetIdRepl UniqueIdRepl = PlayerController->PlayerState->GetUniqueId();
					TSharedPtr<const FUniqueNetId> UniqueNetId = nullptr;
					if (UniqueIdRepl.IsValid())
					{
						UniqueNetId = UniqueIdRepl.GetUniqueNetId();
					}
					mGameInstanceSubsystem->GetSessionInterface()->UnregisterPlayer
					(mGameInstanceSubsystem->GetCurrentSessionName(),*UniqueNetId);
					UE_LOG(LogTemp, Log, TEXT("UCrazy6GameInstance::UnregisterPlayerSession"));
				}
			}
		}
	}
	Super::Shutdown();
}
