// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "End_Mirror.generated.h"

UCLASS()
class HIDEANDSEEK_API AEnd_Mirror : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnd_Mirror();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USceneComponent>RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UStaticMeshComponent> MirrorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport")
		TObjectPtr<UStaticMeshComponent> CommonTeleportPosition; // 플레이어가 사신의 무기 6개 이상을 모으지 않았을 경우에 이동할 지점

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport")
		TObjectPtr<UStaticMeshComponent> HiddenTeleportPosition; // 플레이어가 사신의 무기 6개 이상을 모았을 경우에 이동할 지점

	UPROPERTY()
		bool bIsCleared; // 레벨을 클리어한 상태인지 확인하는 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chapter)
		int32 ClearChapterNum; // 클리어한 챕터 변수

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void OnInteract(class AHorrorGameCharacter* Player);

	UFUNCTION(BlueprintCallable)
		void SetIsCleared(const bool value);
};
