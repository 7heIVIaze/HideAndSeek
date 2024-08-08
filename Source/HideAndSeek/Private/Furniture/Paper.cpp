// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Paper.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/HorrorGamePlayerController.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/CollectArchives.h"

// Sets default values
APaper::APaper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Tick 함수 호출을 없앰.
	PrimaryActorTick.bCanEverTick = false;

	// Root Component를 SceneComponent로 생성한 RootComp로 초기화함.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	// 종이 메시를 만들어 RootComp에 부착해놓음. 메시 세팅이랑 세부적인 위치는 블루프린트를 통해 수정.
	PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paper"));
	PaperMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void APaper::BeginPlay()
{
	Super::BeginPlay();
	
}

// 플레이어가 해당 액터를 클릭했을 때 호출할 함수.
void APaper::OnInteract(class AHorrorGameCharacter* Player)
{
	// 플레이어의 컨트롤러를 HorrorGamePlayerController로 캐스팅해 가져옴. 
	// 플레이어의 컨트롤러는 HorrorGamePlayerController이기 때문에 캐스팅 에러는 없음.
	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(Player->GetController());

	// Document Index에 할당된 인덱스에 맞는 아카이브 데이터를 CollectArchiveData로부터 가져옴. DocumentIndex는 각 Level에 배치된 인스턴스에서 변경된 값임.
	FCollectArchivesData* DocumentData = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(DocumentIndex), TEXT(""));

	////////////////
	// DEPRECATED //
	////////////////
	/*
		FString Key = *FTextInspector::GetKey(text); // FText의 Key 값을 가져옴
		TArray<FString>Value;

		Key.ParseIntoArray(Value, TEXT("#"), false);

		FString Type = Value[0]; // 해당 문서의 타입
		
		int32 Number = FCString::Atoi(*Value[1]); // 해당 문서의 번호
		
		PlayerController->OnDocumentWidget(text, Type);
	*/

	FString Type = TEXT(""); // 해당 문서의 타입 변수로 초기화해줌.

	// Document Widget에 불러온 Document Data를 띄우기 전, Save Data 파일에 저장되었는지 확인하는 절차를 거침.
	// SaveGame 파일을 UHorrorGameSaveGame 클래스로 캐스팅한 후 가져옴
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		// Document Data 구조체의 Type 값이 Article일 경우 아래의 코드를 수행함.
		//if (Type.Equals("Article")) // 해당 문서의 타입이 기사면
		if(DocumentData->Type == EDocumentType::DOCS_Article)
		{
			Type = TEXT("Article"); // 문서의 타입을 Article이라고 할당함.
			
			/*while (SaveData->Article.Num() < Number)
			{
				SaveData->Article.Add(TEXT("???"));
			}
			SaveData->Article[Number - 1] = text.ToString();*/
			//switch (Number)

			// 문서 번호에 따라 아래의 경우를 수행함.
			switch(DocumentData->Number) // 문서 번호
			{
				case 1: // 문서 번호가 1번이면
				{
					// Save Data에 Article1이 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Article1)
					{
						// SaveData에 저장시키고(true로 지정), Player의 SetArchiveGetText 메서드에 Article 1을 획득했다고 알림.
						SaveData->CollectArchives.Article1 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article1", "Article dated January 4th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 2: // 문서 번호가 2번이면
				{
					// Save Data에 Article2가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Article2)
					{
						// SaveData에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Article 2를 획득했다고 알림.
						SaveData->CollectArchives.Article2 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article2", "Article dated December 7th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 3: // 문서 번호가 3번이면
				{
					// Save Data에 Article3이 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Article3)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Article 3을 획득했다고 알림.
						SaveData->CollectArchives.Article3 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article3", "Article dated December 18th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 4: // 문서 번호가 4번이면
				{
					// Save Data에 Article4가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Article4)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Article 4를 획득했다고 알림.
						SaveData->CollectArchives.Article4 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article4", "Article dated January 11th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 5: // 문서 번호가 5번이면
				{
					// Save Data에 Article5가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Article5)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Article 5를 획득했다고 알림.
						SaveData->CollectArchives.Article5 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Article5", "Article dated February 6th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
			}
		}
		// Document Data 구조체의 Type 값이 Diary일 경우, 아래의 코드를 수행함.
		//else if (Type.Equals("Diary")) // 해당 문서의 타입이 일기장이면
		else if (DocumentData->Type == EDocumentType::DOCS_Diary)
		{
			Type = TEXT("Diary"); // Type이 Diary라고 할당함.

			/*while (SaveData->Diary.Num() < Number)
			{
				SaveData->Diary.Add(TEXT("???"));
			}
			SaveData->Diary[Number - 1] = text.ToString();*/
			//switch (Number)

			// 문서 번호에 따라 아래의 경우를 수행함.
			switch (DocumentData->Number)
			{
				case 1: // 문서 번호가 1번이면
				{
					// Save Data에 Diary1이 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Diary1)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Diary 1을 획득했다고 알림.
						SaveData->CollectArchives.Diary1 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary1", "Diary dated November 4th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 2: // 문서 번호가 2번이면
				{ 
					// Save Data에 Diary2가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Diary2)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Diary 2를 획득했다고 알림.
						SaveData->CollectArchives.Diary2 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary2", "Someone's Diary\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 3: // 문서 번호가 3번이면
				{
					// Save Data에 Diary3이 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Diary3)
					{
						// Save Data에 저장시키고 (true로 변경), Player의 SetArchiveGetText 메서드에 Diary 3을 획득했다고 알림,
						SaveData->CollectArchives.Diary3 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary3", "Diary dated January 3rd\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 4: // 문서 번호가 4번이면
				{
					// Save Data에 Diary4가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Diary4)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Diary 4를 획득했다고 알림.
						SaveData->CollectArchives.Diary4 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary4", "Diary dated December 17th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 5: // 문서 번호가 5번이면
				{
					// Save Data에 Diary5가 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Diary5)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Diary 5를 획득했다고 알림.
						SaveData->CollectArchives.Diary5 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Diary5", "Diary dated February 6th\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
			}
		}
		// Document Data 구조체의 Type 값이 Letter일 경우, 아래의 코드를 수행함.
		//else if (Type.Equals("Letter")) // 해당 문서의 타입이 메시지면
		else if (DocumentData->Type == EDocumentType::DOCS_Letter)
		{
			Type = TEXT("Letter"); // 타입값을 Letter로 설정함.

			/*while (SaveData->Message.Num() < Number)
			{
				SaveData->Message.Add(TEXT("???"));
			}
			SaveData->Message[Number - 1] = text.ToString();*/
			//switch (Number)

			// 문서 번호에 따라 아래의 경우를 수행함.
			switch (DocumentData->Number)
			{
				// Berith는 Natty이고 Gamigin은 Hongsul임.
				case 1: // 문서 번호가 1번이면
				{
					// Save Data에 Letter1 From Berith1이 저장되어있지 않다면(false라면)
					if (!SaveData->CollectArchives.Letter1_Berith1)
					{
						// Save Data에 저장시키고(true로 변경), Player의 SetArchiveGetText 메서드에 Letter 1 From Berith 1을 획득했다고 알림.
						SaveData->CollectArchives.Letter1_Berith1 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter1", "Natty's Letter 1\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 2: // 문서 번호가 2번이면
				{
					// Save Data에 Letter2 From Berith2가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter2_Berith2)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 2 from Berith 2를 획득했다고 알림.
						SaveData->CollectArchives.Letter2_Berith2 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter2", "Natty's Letter 2\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안되기 때문에 break로 탈출.
				}
				case 3: // 문서 번호가 3번이면
				{
					// Save Data에 Letter 3 From Berith3이 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter3_Berith3)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 3 from Berith 3를 획득했다고 알림.
						SaveData->CollectArchives.Letter3_Berith3 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter3", "Natty's Letter 3\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 4: // 문서 번호가 4번이면
				{
					// Save Data에 Letter 4 From Berith 4가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter4_Berith4)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 4 from Berith 4를 획득했다고 알림.
						SaveData->CollectArchives.Letter4_Berith4 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter4", "Natty's Letter 4\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 5: // 문서 번호가 5번이면
				{
					// Save Data에 Letter 5 From Berith 5가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter5_Berith5)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 5 from Berith 5를 획득했다고 알림.
						SaveData->CollectArchives.Letter5_Berith5 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter5", "Natty's Letter 5\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 6: // 문서 번호가 6번이면
				{
					// Save Data에 Letter 6 From Berith 6가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter6_Berith6)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 6 from Berith 6를 획득했다고 알림.
						SaveData->CollectArchives.Letter6_Berith6 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter6", "Natty's Letter 6\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 7: // 문서 번호가 7번이면
				{
					// Save Data에 Letter 7 From Gamigin 1이 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter7_Gamigin1)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 7 from Gamigin 1을 획득했다고 알림.
						SaveData->CollectArchives.Letter7_Gamigin1 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter7", "Hongsul's Letter 1\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 8: // 문서 번호가 8번이면
				{
					// Save Data에 Letter 8 From Gamigin 2가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter8_Gamigin2)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 8 from Gamigin 2를 획득했다고 알림.
						SaveData->CollectArchives.Letter8_Gamigin2 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter8", "Hongsul's Letter 2\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 9: // 문서 번호가 9번이면
				{
					// Save Data에 Letter 9 From Gamigin 3이 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter9_Gamigin3)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 9 from Gamigin 3을 획득했다고 알림.
						SaveData->CollectArchives.Letter9_Gamigin3 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter9", "Hongsul's Letter 3\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
				case 10: // 문서 번호가 10번이면
				{
					// Save Data에 Letter 10 From Gamigin 4가 저장되어 있지 않다면
					if (!SaveData->CollectArchives.Letter10_Gamigin4)
					{
						// Save Data에 저장시키고, Player의 SetArchiveGetText 메서드에 Letter 10 from Berith 4를 획득했다고 알림.
						SaveData->CollectArchives.Letter10_Gamigin4 = true;
						Player->SetArchiveGetText(NSLOCTEXT("APaper", "Get_Letter10", "Hongsul's Letter 4\nis added in archive"));
					}

					break; // 이 함수에서 나가면 안 되기 때문에 break로 탈출.
				}
			}
			
		}
		// 그 후 Save Data를 Save Game 파일에 저장함. 
		SaveData->SaveData();
	}
	// 위의 과정을 거친 후, 플레이어가 보려는 문서 내용을 DocumentWidget을 통해 출력함.
	PlayerController->OnDocumentWidget(DocumentData->Detail_EN, Type);
}