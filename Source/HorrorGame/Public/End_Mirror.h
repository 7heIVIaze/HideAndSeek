// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "End_Mirror.generated.h"

UCLASS()
class HORRORGAME_API AEnd_Mirror : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnd_Mirror();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USceneComponent>RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UStaticMeshComponent> MirrorMesh;

	UPROPERTY()
		bool bIsCleared; // 레벨을 클리어한 상태인지 확인하는 변수

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
