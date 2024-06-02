// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "GlowStick_cpp.generated.h"

/*
 * 빛을 내는 아이템. 길 찾는 용도로 사용가능하도록 설정할 것 
 */
UCLASS()
class HIDEANDSEEK_API AGlowStick_cpp : public AItems
{
	GENERATED_BODY()
	
public:
	AGlowStick_cpp();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = Root)
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
		TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class USoundCue* UseSound;

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
		UMaterialInstanceDynamic* MaterialInstance;

public: // Functions
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
