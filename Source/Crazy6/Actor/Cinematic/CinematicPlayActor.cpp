// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicPlayActor.h"

#include "EngineUtils.h"

// MediaAssets ��⿡ �ִ� UMediaPlayer�� ������ ��
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"

#include "Kismet/GameplayStatics.h"


#include "Crazy6/Player/PlayerBase.h"
#include "Crazy6/Actor/Enemy/EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"


// Sets default values
// �ó׸�ƽ�� ���� ������ ���� �ְ�, ����� ��� �� ������ ��� �� ������ ���� 
// ������ ���� ���� �ñ�� ����� ������ Ŭ�������� ���� (������ ��Ƽ���� ��� ���� ��)
ACinematicPlayActor::ACinematicPlayActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �̵����� ����� ���ͷ� ��. �׷� �̵�� �÷��̾�� ��� ���̴°�.
	// �̵�� �÷��̾�� �������� ������� �� ����Ѵ�. �׷��Ƿ� ���� Ŭ���������� �ʿ� ����

	// ������ ������ ���°�? �̵���÷��̾��� �ؽ�ó�� mt�� �̹��� ������ ���ε���������Ѵ�.
	// �׷� ���⿡ �ʿ��Ѱ�? mt�� ���� ���� �ִ°�? 
	// �ٵ� ������ �ܼ��� ������ ������ְ� �װ� �����ִ� �뵵�̱� ������ mt�� ���� �ְ� �ϴ°� ���� �ȵȴ�. 
	// ������ ���� Ŭ�������� �����Ѵ�. 
	mMediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));
	SetRootComponent(mMediaSound);

	// ====== Media
	mMediaPlayer = CreateDefaultSubobject<UMediaPlayer>(TEXT("MediaPlayer"));

	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MediaPlayerFinder(
		TEXT("/Script/MediaAssets.MediaPlayer'/Game/Dev/Level/Cinematic/CinematicPlayer.CinematicPlayer'"));

	if (MediaPlayerFinder.Succeeded())
	{
		mMediaPlayer = MediaPlayerFinder.Object;
	}

	bReplicates = true;


}

// Called when the game starts or when spawned
void ACinematicPlayActor::BeginPlay()
{
	Super::BeginPlay();


	// ����� ���� �ҽ��� �ǰ� ���� ����� �Ҹ��� ����. 
	mMediaSound->SetMediaPlayer(mMediaPlayer);

	mCinematicVideoMT =
		LoadObject<UMaterial>(nullptr, TEXT("/Game/Dev/Level/Cinematic/MT_CinematicPlayerTex.MT_CinematicPlayerTex"));


}

// Called every frame
void ACinematicPlayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACinematicPlayActor::CinematicPlay()
{
	FName CurLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));

	if (CurLevelName == "Bridge_P")
	{
		UMediaSource* MediaSource = LoadObject<UMediaSource>(nullptr, TEXT("/Game/Dev/Level/Cinematic/MediaSource/BossIntro.BossIntro"));

		if (MediaSource)
		{
			mMediaPlayer->OpenSource(MediaSource);
		}

	}

	else
	{
		UMediaSource* MediaSource = LoadObject<UMediaSource>(nullptr, TEXT("/Game/Dev/Level/Cinematic/MediaSource/LevelOneSoundFix.LevelOneSoundFix"));

		if (MediaSource)
		{
			mMediaPlayer->OpenSource(MediaSource);

		}
	}

	// ����� ���� �ҽ��� �ǰ� ���� ����� �Ҹ��� ����. 
	mMediaSound->SetMediaPlayer(mMediaPlayer);


}

void ACinematicPlayActor::PlayEndController()
{
	for (APlayerBase* player : TActorRange<APlayerBase>(GetWorld()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(player->GetController()))
		{
			player->EnableInput(PlayerController);
		}
	}


	APawn* foundPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass()));

	AAIController* AIController = Cast<AAIController>(foundPawn->GetController());

	if (IsValid(AIController))
	{
		AIController->SetActorTickEnabled(true);
	}



}

void ACinematicPlayActor::PlayController()
{
	for (APlayerBase* player : TActorRange<APlayerBase>(GetWorld()))
	{
		if (IsValid(player))
		{
			player->DisableInput(nullptr);
		}
	}

	for (AEnemyBase* Monster : TActorRange<AEnemyBase>(GetWorld()))
	{
		AAIController* AIController = Cast<AAIController>(Monster->GetController());

		if (IsValid(AIController))
		{
			AIController->BrainComponent->StopLogic("");
		}
	}


}

