// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Image.h"



void UMinimapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UWorld* World = GetWorld();
    if (!World)
    {
        return; // ���尡 ��ȿ���� ������ ��ȯ
    }

    // ���� ���忡 �ִ� �÷��̾� ��Ʈ�ѷ� �� ��������
    int32 PlayerNum = World->GetNumPlayerControllers();

    for (int32 PlayerIndex = 0; PlayerIndex < PlayerNum; ++PlayerIndex)
    {
        // Ư�� �ε����� �÷��̾� ��Ʈ�ѷ� ��������
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);

        if (PlayerController)
        {
            APlayerBase* ControlledPlayer = Cast<APlayerBase>(PlayerController->GetCharacter());

            if (ControlledPlayer)
            {
               
                // PlayerController �̸����� �ε����� �����Ͽ� ��Ƽ���� ����
                FString PlayerControllerName = PlayerController->GetName();
                int32 ControllerId = -1;

                // "_" ���� ���ڸ� �����Ͽ� ControllerId�� ��ȯ
                FString ControllerIndexStr;
                if (PlayerControllerName.Split(TEXT("_"), nullptr, &ControllerIndexStr))
                {
                    ControllerId = FCString::Atoi(*ControllerIndexStr);
                }

                UMaterialInstance* MinimapMaterial = nullptr;

                switch (ControllerId)
                {
                    case 0:
                    {
              

                    }
                      
                       break;

                    case 1:
                    {
                        TObjectPtr<UCanvasRenderTarget2D> PlayerSceneCapture = ControlledPlayer->GetMinimapRenderTarget();
                    }
                        
                        break;

                    case 2:
                        MinimapMaterial = Cast<UMaterialInstance>(
                            StaticLoadObject(UMaterialInstance::StaticClass(), nullptr,
                                TEXT("/Game/Dev/UI/MiniMap/Basic/MTInst_MinimapCanvasRenderTarget2D_3.MTInst_MinimapCanvasRenderTarget2D_3"))
                        );
                        break;

                    case 3:
                        MinimapMaterial = Cast<UMaterialInstance>(
                            StaticLoadObject(UMaterialInstance::StaticClass(), nullptr,
                                TEXT("/Game/Dev/UI/MiniMap/Basic/MTInst_MinimapCanvasRenderTarget2D_4.MTInst_MinimapCanvasRenderTarget2D_4"))
                        );
                        break;

                    default:
                        UE_LOG(LogTemp, Warning, TEXT("No specific material for PlayerIndex %d"), PlayerIndex);
                        break;
                }

                // ��Ƽ������ ��ȿ�� ��� ĳ���ͳ� ������ ����
                if (MinimapMaterial)
                {
                    if (MinimapImage)
                    {
                        // �̹����� ��Ƽ���� ����
                        MinimapImage->SetBrushFromMaterial(MinimapMaterial);
                        UE_LOG(LogTemp, Log, TEXT("Applied material to player %d"), PlayerIndex);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("MinimapImage is null for player %d"), PlayerIndex);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Material not found for player %d"), PlayerIndex);
                }
     


            }
        }
    }



}
            //    // PlayerController �̸����� �ε����� �����Ͽ� SceneCaptureComponent ����
            //    FString PlayerControllerName = PlayerController->GetName();
            //    int32 ControllerId = -1;

            //    // "_" ���� ���ڸ� �����Ͽ� ControllerId�� ��ȯ
            //    FString ControllerIndexStr;
            //    if (PlayerControllerName.Split(TEXT("_"), nullptr, &ControllerIndexStr))
            //    {
            //        ControllerId = FCString::Atoi(*ControllerIndexStr);
            //    }

            //    if (ControllerId == PlayerIndex)
            //    {
            //        // �÷��̾� Ŭ������ �پ� �ִ� SceneCaptureComponent ��������
            //        USceneCaptureComponent2D* PlayerSceneCapture = ControlledPlayer->GetMinimapCaptureComp();
            //        if (PlayerSceneCapture)
            //        {

            //            if (MinimapImage)
            //            {
            //                // SceneCaptureComponent�� ĵ���� ���� Ÿ���� ����Ͽ� ���� ��Ƽ���� �ν��Ͻ��� ���� �� �̴ϸ� �̹����� ����
            //                UCanvasRenderTarget2D* RenderTarget = Cast<UCanvasRenderTarget2D>(PlayerSceneCapture->TextureTarget);

            //                if (RenderTarget)
            //                { // �귯���� ���� ĵ���� ���� Ÿ���� ����
            //                    FSlateBrush MinimapBrush;
            //                    MinimapBrush.SetResourceObject(RenderTarget);
            //                    MinimapBrush.ImageSize = FVector2D(32.0f, 32.0f); // �ʿ��� �̹��� ũ��� ����
            //                    MinimapImage->SetBrush(MinimapBrush);
            //                }





            //            }






            //        }
            //    }
            //}
        
    


    


	 // �÷��̾ ����´�.

		//���� �� �÷��̾� ��Ʈ�ѷ��� ���� ���̽��� �����Ѵ�. 
		// ĸó ������Ʈ�� ����. 
		// Set Mateiral �� �ٽ� ���ֱ�
		

//	int32 playerNum = GetWorld()->GetNumPlayerControllers();
	//for (FConstControllerIterator it = GetWorld()->GetControllerIterator(); it; ++it)
	//{
	//	AController* Controller = it->Get();

	//	if (Controller)
	//	{

	//		APlayerController* PlayerController = Cast<APlayerController>(Controller);

	//		if (PlayerController)
	//		{

	//		}


	//	}


	//}


	//for (int i = 1; i <= 12; ++i)
	//{
	//	FString ArmorSocketName = FString::Printf(TEXT("Leg%d_Armor"), i);
	//	mArmorHitBox = CreateDefaultSubobject<UBoxComponent>(*ArmorSocketName);
	//	mArmorHitBox->SetupAttachment(mMesh, *ArmorSocketName);
	//	mArmorHitBox->SetRelativeScale3D(FVector(0.01, 0.01, 0.01));
	//	mArmorHitBox->SetBoxExtent(FVector(40.0, 60.0, 10.0));
	//	mArmorHitBox->SetRelativeLocation(FVector(0.0, -0.665, -0.053));
	//	mArmorHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_BOSSARMOR);
	//	mArmorHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//}



	//MT_PlayerOne 
	//MT_PlayerTwo
	//MT_PlayerThree
	//MT_PlayerFour




