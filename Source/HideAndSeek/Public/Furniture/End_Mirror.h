// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/InteractableActor.h"
#include "End_Mirror.generated.h"

UCLASS()
class HIDEANDSEEK_API AEnd_Mirror : public AInteractableActor
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

	UPROPERTY(VisibleAnywhere, Category = Boolean)
	bool bIsCleared; // 레벨을 클리어한 상태인지 확인하는 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chapter)
	int32 ClearChapterNum; // 클리어한 챕터 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> InteractSound; // 순간이동 소리

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	void SetIsCleared(const bool value);

};
