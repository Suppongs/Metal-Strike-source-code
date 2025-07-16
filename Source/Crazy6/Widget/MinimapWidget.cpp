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
        return; // 월드가 유효하지 않으면 반환
    }

    // 현재 월드에 있는 플레이어 컨트롤러 수 가져오기
    int32 PlayerNum = World->GetNumPlayerControllers();

    for (int32 PlayerIndex = 0; PlayerIndex < PlayerNum; ++PlayerIndex)
    {
        // 특정 인덱스의 플레이어 컨트롤러 가져오기
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);

        if (PlayerController)
        {
            APlayerBase* ControlledPlayer = Cast<APlayerBase>(PlayerController->GetCharacter());

            if (ControlledPlayer)
            {
               
                // PlayerController 이름에서 인덱스를 추출하여 머티리얼 적용
                FString PlayerControllerName = PlayerController->GetName();
                int32 ControllerId = -1;

                // "_" 뒤의 숫자만 추출하여 ControllerId로 변환
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

                // 머티리얼이 유효한 경우 캐릭터나 위젯에 적용
                if (MinimapMaterial)
                {
                    if (MinimapImage)
                    {
                        // 이미지에 머티리얼 적용
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
            //    // PlayerController 이름에서 인덱스를 추출하여 SceneCaptureComponent 적용
            //    FString PlayerControllerName = PlayerController->GetName();
            //    int32 ControllerId = -1;

            //    // "_" 뒤의 숫자만 추출하여 ControllerId로 변환
            //    FString ControllerIndexStr;
            //    if (PlayerControllerName.Split(TEXT("_"), nullptr, &ControllerIndexStr))
            //    {
            //        ControllerId = FCString::Atoi(*ControllerIndexStr);
            //    }

            //    if (ControllerId == PlayerIndex)
            //    {
            //        // 플레이어 클래스에 붙어 있는 SceneCaptureComponent 가져오기
            //        USceneCaptureComponent2D* PlayerSceneCapture = ControlledPlayer->GetMinimapCaptureComp();
            //        if (PlayerSceneCapture)
            //        {

            //            if (MinimapImage)
            //            {
            //                // SceneCaptureComponent의 캔버스 렌더 타겟을 사용하여 동적 머티리얼 인스턴스를 생성 및 미니맵 이미지에 적용
            //                UCanvasRenderTarget2D* RenderTarget = Cast<UCanvasRenderTarget2D>(PlayerSceneCapture->TextureTarget);

            //                if (RenderTarget)
            //                { // 브러쉬로 직접 캔버스 렌더 타겟을 적용
            //                    FSlateBrush MinimapBrush;
            //                    MinimapBrush.SetResourceObject(RenderTarget);
            //                    MinimapBrush.ImageSize = FVector2D(32.0f, 32.0f); // 필요한 이미지 크기로 설정
            //                    MinimapImage->SetBrush(MinimapBrush);
            //                }





            //            }






            //        }
            //    }
            //}
        
    


    


	 // 플레이어를 갖고온다.

		//갖고 온 플레이어 컨트롤러를 통해 베이스에 접근한다. 
		// 캡처 컴포넌트로 접근. 
		// Set Mateiral 로 다시 해주기
		

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




