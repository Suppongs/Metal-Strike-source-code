#include "BulletTrace.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletTrace::ABulletTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mDefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	static	ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Dev/Player/Weapons/BulletTrace/Mesh/St_Tracer_A.St_Tracer_A'"));

	if (MeshAsset.Succeeded())
	{
		mStaticMesh->SetStaticMesh(MeshAsset.Object);
	}
	
	SetRootComponent(mDefaultScene);
	mSphere->SetupAttachment(mDefaultScene);
	mStaticMesh->SetupAttachment(mSphere);

	InitialLifeSpan = 2.0f;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &ABulletTrace::OnBeginOverlap);
	mSphere->SetCollisionProfileName("PlayerBullet");
	mSphere->SetCanEverAffectNavigation(false);
	mSphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	mSphere->CanCharacterStepUpOn = ECB_No;
	
	mProjectileMovement->InitialSpeed = 6000.0f;
	mProjectileMovement->bInitialVelocityInLocalSpace = true;
	mProjectileMovement->MaxSpeed = 6000.0f;
	mProjectileMovement->ProjectileGravityScale = 0.0f;
	mProjectileMovement->bSimulationEnabled = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABulletTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletTrace::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();
}