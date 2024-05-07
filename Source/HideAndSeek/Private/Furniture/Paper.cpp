// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Furniture/Paper.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "HorrorGamePlayerController.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/CollectArchives.h"

// Sets default values
APaper::APaper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paper"));
	PaperMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void APaper::BeginPlay()
{
	Super::BeginPlay();
	
}

void APaper::OnInteract(class AHorrorGameCharacter* Player)
{
	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(Player->GetController());
	FCollectArchivesData* DocumentData = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(DocumentIndex), TEXT(""));

	//FString Key = *FTextInspector::GetKey(text); // FText의 Key 값을 가져옴
	//TArray<FString>Value;

	//Key.ParseIntoArray(Value, TEXT("#"), false);

	// FString Type = Value[0]; // 해당 문서의 타입
	FString Type = TEXT(""); // 해당 문서의 타입

	//int32 Number = FCString::Atoi(*Value[1]); // 해당 문서의 번호

	//PlayerController->OnDocumentWidget(text, Type);

	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		//if (Type.Equals("Article")) // 해당 문서의 타입이 기사면
		if(DocumentData->Type == EDocumentType::DOCS_Article)
		{
			Type = TEXT("Article");
			/*while (SaveData->Article.Num() < Number)
			{
				SaveData->Article.Add(TEXT("???"));
			}
			SaveData->Article[Number - 1] = text.ToString();*/
			//switch (Number)
			switch(DocumentData->Number) // 문서 번호
			{
			case 1:
				if (!SaveData->CollectArchives.Article1)
				{
					SaveData->CollectArchives.Article1 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article1", "Article dated January 4th\nis added in archive"));
				}
				break;
			case 2:
				if (!SaveData->CollectArchives.Article2)
				{
					SaveData->CollectArchives.Article2 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article2", "Article dated December 7th\nis added in archive"));
				}
				break;
			case 3:
				if (!SaveData->CollectArchives.Article3)
				{
					SaveData->CollectArchives.Article3 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article3", "Article dated December 18th\nis added in archive"));
				}
			case 4:
				if (!SaveData->CollectArchives.Article4)
				{
					SaveData->CollectArchives.Article4 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article4", "Article dated January 11th\nis added in archive"));
				}
				break;
			case 5:
				if (!SaveData->CollectArchives.Article5)
				{
					SaveData->CollectArchives.Article5 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article5", "Article dated February 6th\nis added in archive"));
				}
				break;
			}
		}
		
		//else if (Type.Equals("Diary")) // 해당 문서의 타입이 일기장이면
		else if (DocumentData->Type == EDocumentType::DOCS_Diary)
		{
			Type = TEXT("Diary");
			/*while (SaveData->Diary.Num() < Number)
			{
				SaveData->Diary.Add(TEXT("???"));
			}
			SaveData->Diary[Number - 1] = text.ToString();*/
			//switch (Number)
			switch (DocumentData->Number)
			{
			case 1:
				if (!SaveData->CollectArchives.Diary1)
				{
					SaveData->CollectArchives.Diary1 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary1", "Diary dated November 4th\nis added in archive"));
				}
				break;
			case 2:
				if (!SaveData->CollectArchives.Diary2)
				{
					SaveData->CollectArchives.Diary2 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary2", "Someone's Diary\nis added in archive"));
				}
				break;
			case 3:
				if (!SaveData->CollectArchives.Diary3)
				{
					SaveData->CollectArchives.Diary3 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary3", "Diary dated January 3rd\nis added in archive"));
				}
			case 4:
				if (!SaveData->CollectArchives.Diary4)
				{
					SaveData->CollectArchives.Diary4 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary4", "Diary dated December 17th\nis added in archive"));
				}
				break;
			case 5:
				if (!SaveData->CollectArchives.Diary5)
				{
					SaveData->CollectArchives.Diary5 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary5", "Diary dated February 6th\nis added in archive"));
				}
				break;
			}
		}

		//else if (Type.Equals("Letter")) // 해당 문서의 타입이 메시지면
		else if (DocumentData->Type == EDocumentType::DOCS_Letter)
		{
			Type = TEXT("Letter");
			/*while (SaveData->Message.Num() < Number)
			{
				SaveData->Message.Add(TEXT("???"));
			}
			SaveData->Message[Number - 1] = text.ToString();*/
			//switch (Number)
			switch (DocumentData->Number)
			{
			case 1:
				if (!SaveData->CollectArchives.Letter1_Berith1)
				{
					SaveData->CollectArchives.Letter1_Berith1 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter1", "Natty's Letter 1\nis added in archive"));
				}
				break;
			case 2:
				if (!SaveData->CollectArchives.Letter2_Berith2)
				{
					SaveData->CollectArchives.Letter2_Berith2 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter2", "Natty's Letter 2\nis added in archive"));
				}
				break;
			case 3:
				if (!SaveData->CollectArchives.Letter3_Berith3)
				{
					SaveData->CollectArchives.Letter3_Berith3 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter3", "Natty's Letter 3\nis added in archive"));
				}
				break;
			case 4:
				if (!SaveData->CollectArchives.Letter4_Berith4)
				{
					SaveData->CollectArchives.Letter4_Berith4 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter4", "Natty's Letter 4\nis added in archive"));
				}
				break;
			case 5:
				if (!SaveData->CollectArchives.Letter5_Berith5)
				{
					SaveData->CollectArchives.Letter5_Berith5 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter5", "Natty's Letter 5\nis added in archive"));
				}
				break;
			case 6:
				if (!SaveData->CollectArchives.Letter6_Berith6)
				{
					SaveData->CollectArchives.Letter6_Berith6 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter6", "Natty's Letter 6\nis added in archive"));
				}
				break;
			case 7:
				if (!SaveData->CollectArchives.Letter7_Gamigin1)
				{
					SaveData->CollectArchives.Letter7_Gamigin1 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter7", "Hongsul's Letter 1\nis added in archive"));
				}
				break;
			case 8:
				if (!SaveData->CollectArchives.Letter8_Gamigin2)
				{
					SaveData->CollectArchives.Letter8_Gamigin2 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter8", "Hongsul's Letter 2\nis added in archive"));
				}
				break;
			case 9:
				if (!SaveData->CollectArchives.Letter9_Gamigin3)
				{
					SaveData->CollectArchives.Letter9_Gamigin3 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter9", "Hongsul's Letter 3\nis added in archive"));
				}
				break;
			case 10:
				if (!SaveData->CollectArchives.Letter10_Gamigin4)
				{
					SaveData->CollectArchives.Letter10_Gamigin4 = true;
					Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter10", "Hongsul's Letter 4\nis added in archive"));
				}
				break;
			}
			
		}

		SaveData->SaveData();
	}

	PlayerController->OnDocumentWidget(DocumentData->Detail_EN, Type);
}